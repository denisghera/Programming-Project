#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Product {
    int quantity, ID;
    char *name;
    char *category;
} Product;

struct Product createProduct(int ID, char *name, char *category, int quantity){
    struct Product product_info;
    product_info.ID = ID;
    product_info.quantity = quantity;
    product_info.name = malloc(strlen(name) + 1);
    strcpy(product_info.name, name);
    product_info.category = malloc(strlen(category) + 1);
    strcpy(product_info.category, category);
    return product_info;
}

typedef struct Node{
    struct Product product_info;
    struct Node *next;
} Node;

typedef struct List{
    Node *head;
    Node *tail;
    int length;
} List;

void append(struct List *l, struct Product info)
{
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->product_info = info;
    new_node->next = NULL;
    if(l->tail == NULL)
    {
        l->head = new_node;
        l->tail = new_node;
    }
    else
    {
        l->tail->next = new_node;
        l->tail = new_node;
    }
    l->length++;
}
Node* search(List l, int id)
{
    Node *current = l.head;
    while(current != NULL)
    {
        if(current->product_info.ID == id) return current;
        current = current->next;
    }
    return NULL;
}
void save_data(char* filename, List l)
{
    FILE* fp = fopen(filename, "w");
    Node *current = l.head;
    if (!fp) printf("Can't open file\n");
    else
        while(current != NULL)
        {
            fprintf(fp, "%d,%s,%s,%d\n", current->product_info.ID,
                    current->product_info.name, current->product_info.category,
                    current->product_info.quantity);
            current = current->next;
        }
    fclose(fp);
}

void retrieve_data(char* filename, List *l)
{
    FILE* fp = fopen(filename, "r");
    if (!fp) printf("Can't open file\n");
    else {
        char line[100];
        while (fgets(line, sizeof(line), fp)) {
            int id, quantity, result;
            char name[50], category[50];
            result = sscanf(line, "%d,%[^,],%[^,],%d", &id, name, category, &quantity);
            if (result == 4) {
                Product info = createProduct(id, name, category, quantity);
                append(l,info);
            }
        }
        fclose(fp);
    }
}

int input()
{
    int number;
    scanf("%d", &number);
    return number;
}

void display_all_products(List l)
{
    Node *current = l.head;
    int k = 0;
    printf("\nList of all products:\n");
    while(current != NULL)
    {
        printf("%d: %s (%s) x%d\n", current->product_info.ID, current->product_info.name,
               current->product_info.category, current->product_info.quantity);
        current = current->next;
        k++;
    }
    if (k == 0) printf("Empty list :(\n");
}

void display_low_quantity_products(List l)
{
    Node *current = l.head;
    int k = 0;
    printf("\nList of all products:\n");
    while(current != NULL)
    {
        if(current->product_info.quantity <= 5)
        {
            printf("%d: %s (%s) x%d\n", current->product_info.ID, current->product_info.name,
                   current->product_info.category, current->product_info.quantity);
            k++;
        }
        current = current->next;

    }
    if (k == 0) printf("Empty list :(\n");
}

void search_product_by_name(List l)
{
    char name[50];
    printf("\nEnter product name: ");
    scanf("%s", name);
    Node *current = l.head;
    int k = 0;
    printf("\nSearch results for '%s':\n", name);
    while(current != NULL)
    {
        if (strcmp(current->product_info.name, name) == 0)
        {
            printf("%d: %s (%s) x%d\n", current->product_info.ID, current->product_info.name,
                   current->product_info.category, current->product_info.quantity);
            k++;
        }
        current = current->next;
    }
    if (k == 0) printf("No results found :(\n");
}

void search_product_by_category(List l)
{
    char category[50];
    printf("\nEnter product category: ");
    scanf("%s", category);
    Node *current = l.head;
    int k = 0;
    printf("\nSearch results for '%s':\n", category);
    while(current != NULL)
    {
        if (strcmp(current->product_info.category, category) == 0)
        {
            printf("%d: %s (%s) x%d\n", current->product_info.ID, current->product_info.name,
                   current->product_info.category, current->product_info.quantity);
            k++;
        }
        current = current->next;
    }
    if (k == 0) printf("No results found :(\n");
}

void add_product(struct List *l)
{
    printf("Enter product ID: ");
    int id = input();

    printf("Enter product name: ");
    char *name = malloc(255);
    scanf("%s", name);

    printf("Enter product category: ");
    char *category = malloc(255);
    scanf("%s", category);

    printf("Enter product quantity: ");
    int quantity = input();

    struct Product info = createProduct(id, name, category, quantity);
    append(l, info);
    printf("The product '%d: %s (%s) x%d' was added!\n", info.ID, info.name, info.category,
           info.quantity);
}

void update_product(List *l)
{
    int id;
    printf("\nEnter product ID: ");
    id = input();

    Node *product = search(*l, id);

    if (product == NULL)
    {
        printf("Product with ID %d does not exist.\n", id);
        return;
    }
    else printf("Updating product '%d: %s (%s) x%d'!\n", id, product->product_info.name,
                product->product_info.category, product->product_info.quantity);

    printf("Enter new product name: ");
    scanf("%s", product->product_info.name);

    printf("Enter new product category: ");
    scanf("%s", product->product_info.category);

    printf("Enter new product quantity: ");
    product->product_info.quantity = input();

    printf("The product '%d: %s (%s) x%d' was updated!\n", id, product->product_info.name,
           product->product_info.category, product->product_info.quantity);
}

void remove_product(List *l)
{
    int id;
    printf("\nEnter product ID: ");
    id = input();

    Node *tmp = search(*l, id);
    if (tmp == NULL)
    {
        printf("Product with ID %d does not exist.\n", id);
        return;
    }
    printf("Are you sure you want to delete '%d: %s (%s) x%d'? (Y/N) ", id,
           tmp->product_info.name, tmp->product_info.category, tmp->product_info.quantity);
    char confirmation;
    scanf(" %c", &confirmation);

    if (confirmation == 'Y' || confirmation == 'y')
    {
        printf("The product '%d: %s (%s) x%d' was removed!\n", id,
               tmp->product_info.name, tmp->product_info.category, tmp->product_info.quantity);
        if (l->head->product_info.ID == id) {
            Node *temp = l->head;
            l->head = l->head->next;
            if (l->head == NULL) {
                l->tail = NULL;
            }
            free(temp);
            l->length--;
            return;
        }
        Node *prev_node = l->head;
        Node *curr_node = l->head->next;
        while (curr_node != NULL) {
            if (curr_node->product_info.ID == id) {
                prev_node->next = curr_node->next;
                if (curr_node == l->tail) {
                    l->tail = prev_node;
                }
                free(curr_node);
                l->length--;
                return;
            }
            prev_node = curr_node;
            curr_node = curr_node->next;
        }
    }
    else
    {
        printf("The product '%d: %s (%s) x%d' was not removed.\n", id,
               tmp->product_info.name, tmp->product_info.category, tmp->product_info.quantity);
    }
}

void merge(List *dest, List *l1, List *l2)
{
    Node *current1 = l1->head;
    Node *current2 = l2->head;
    while (current1 != NULL || current2 != NULL)
    {
        if(current1 == NULL)
        {
            append(dest, current2->product_info);
            current2 = current2->next;
        }
        else if(current2 == NULL)
        {
            append(dest, current1->product_info);
            current1 = current1->next;
        }
        else if(current1->product_info.ID > current2->product_info.ID)
        {
            append(dest, current2->product_info);
            current2 = current2->next;
        }
        else if(current2->product_info.ID > current1->product_info.ID)
        {
            append(dest, current1->product_info);
            current1 = current1->next;
        }
        else if(current1->product_info.ID == current2->product_info.ID)
        {
            append(dest, current1->product_info);
            append(dest, current2->product_info);
            current1 = current1->next;
            current2 = current2->next;
        }
    }
}

void split(List *li, List *lj, List l) {
    int count = l.length;
    int middle = count / 2;
    Node *temp = l.head;

    li->head = li->tail = NULL;
    lj->head = lj->tail = NULL;

    for (int i = 0; i < middle; i++) {
        append(li, temp->product_info);
        temp = temp->next;
    }

    for (int i = middle; i < count; i++) {
        append(lj, temp->product_info);
        temp = temp->next;
    }

    li->length = middle;
    lj->length = count - middle;
}

void sort(List *dest, List l) {
    if (l.length <= 1) {
        *dest = l;
        return;
    }

    List li, lj;
    split(&li, &lj, l);

    sort(&li, li);
    sort(&lj, lj);

    dest->head = NULL;
    dest->tail = NULL;
    dest->length = 0;
    merge(dest, &li, &lj);
}

int switch_fun(struct List *inventory_list)
{
    int choice;
    printf("\n- - Main Menu - -\nEnter your choice: ");
    choice = input();

    switch (choice)
    {
        case 1:
        {
            display_all_products(*inventory_list);
            switch_fun(inventory_list);
            break;
        }
        case 2:
        {
            add_product(inventory_list);
            sort(inventory_list, *inventory_list);
            switch_fun(inventory_list);
            break;
        }
        case 3:
        {
            update_product(inventory_list);
            switch_fun(inventory_list);
            break;
        }
        case 4:
        {
            remove_product(inventory_list);
            switch_fun(inventory_list);
            break;
        }
        case 5:
        {
            search_product_by_name(*inventory_list);
            switch_fun(inventory_list);
            break;
        }
        case 6:
        {
            search_product_by_category(*inventory_list);
            switch_fun(inventory_list);
            break;
        }
        case 7:
        {
            display_low_quantity_products(*inventory_list);
            switch_fun(inventory_list);
            break;
        }
        case 8:
        {
            save_data("..\\data.csv", *inventory_list);
            printf("Exiting...");
            return 0;
        }
        default:
        {
            printf("wrong Input\n");
            switch_fun(inventory_list);
            break;
        }
    }
}

int main()
{
    List inventory_list;
    inventory_list.head = NULL;
    inventory_list.tail = NULL;
    inventory_list.length = 0;
    retrieve_data("..\\data.csv", &inventory_list);
    printf("Press 1 to see the list of products\n");
    printf("Press 2 to add a product\n");
    printf("Press 3 to update a product\n");
    printf("Press 4 to delete a product\n");
    printf("Press 5 to search a product by name\n");
    printf("Press 6 to search a product by category\n");
    printf("Press 7 to display products with a low quantity\n");
    printf("Press 8 to exit!");

    switch_fun(&inventory_list);

    return 0;
}
