#include "SimpleList.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// List는 구조체이기 때문에 함수 매개변수로 넘겨줄때 포인터로 넘겨주지 않으면 메모리가 낭비된다.
void read_file(List* plist) {
    char filename[TSIZE];

    /* Reading file */
    printf("Please input filename to read and press enter.\n>> ");
    if (scanf("%[^\n]%*c", filename) != 1) {
        printf("Wrong input. Terminating.\n");
        exit(1);
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error : Cannot open file.\n");
        exit(1);
    }

    int num;
    if (fscanf(file, "%d%*c", &num) != 1) {
        printf("Wrong file format.\n");
        exit(1);
    }

    // Scan from file to structure
    for (int i = 0; i < num; ++i) {
        Item new_item;
        
        if (fscanf(file, "%[^\n]%*c", new_item.title) != 1 ||
            fscanf(file, "%f%*c", &new_item.rating) != 1) {
            printf("ERROR : Wrong File format.\n");
            exit(1);
        }

        /* Abstraction
        A function of adding a new_item to plist */
        AddItem(new_item, plist);
    }
    
    fclose(file);

    printf("%d items have been read from the file.\n\n", num);
}

unsigned int count_items(const List* plist) {
    return CountItems(plist);
}

void write_an_item(FILE* const file, const Item item) {
    fprintf(file, "%s\n", item.title);
    fprintf(file, "%.1f\n", item.rating);
}

void write_file(const List* const plist) {
    char filename[TSIZE];

    printf("Please input filename to write and press enter.\n>> ");
    if (scanf("%[^\n]%*c", filename) != 1) {
        printf("Wrong input. Terminating.\n");
        exit(1);
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error : Cannot open file.\n");
        exit(1);
    }

    unsigned int num = count_items(plist);

    fprintf(file, "%d\n", num);

    unsigned int count = WriteAllItems(plist, file, &write_an_item);

    assert(count == num);
    
    fclose(file);

    printf("%d items have been saved to the file.\n", count);
}

int input_int() {
    int input;
    
    while (1) {
        if (scanf("%d%*c", &input) == 1) return input;
        else {
            printf("Please input an integer and press enter.\nTry again.\n");
            while(getchar() != '\n') continue;
        }
    }
}

int input_menu() {
    while (1) {
        printf("Please select an option and press enter.\n");
        printf("1. Print all items \t2. Print an item\n");
        printf("3. Edit an item \t4. Add an item\n");
        printf("5. Insert an item \t6. Delete an item\n");
        printf("7. Delete all items \t8. Save file\n");
        printf("9. Search by name \t10. Quit\n>> ");

        int input = input_int();

        if (input >= 1 && input <= 10) return input;
        else 
            printf("Please input a number between 1 ~ 10\n");        
    }
}

void print_an_item(Item item) {
    printf("\"%s\", %.1f\n", item.title, item.rating);
}

void print_all(const List* plist) {
    PrintAllItems(plist, &print_an_item);
}

// note: renamed from print_an_item.
void find_and_print_an_item(const List* plist) {
    printf("Input the index of item to print.\n");
    int index = input_int();

    Item* pitem;
    const bool flag = FindItemByIndex(plist, index, &pitem);

    if (flag == true) {
        printf("%d : ", index);
        print_an_item(*pitem);
    } else  
        printf("Invalid Item.\n\n");
}

void edit_an_item(List* plist) {
    printf("Input the index of item to edit.\n");
    int index = input_int();
    
    Item* pitem;
    const bool flag = FindItemByIndex(plist, index, &pitem);

    if (flag == true) {
        printf("%d : ", index);
        print_an_item(*pitem);
        
        Item new_item;
        printf("Input new title and press enter.\n>> ");
        int f = scanf("%[^\n]%*c", new_item.title);
        printf("Input new rating and press enter.\n>> ");
        f = scanf("%f%*c", &new_item.rating);

        *pitem = new_item;

        printf("%d : ", index);
        print_an_item(*pitem);
    } else 
        printf("Invalid Item.\n");
}

void add_an_item(List* plist) {
    Item new_item;
    unsigned int num = CountItems(plist);

    printf("%d items in current list.\n", num);
    printf("Please input title of new item and press enter.\n>> ");
    int f = scanf("%[^\n]%*c", new_item.title);
    printf("Please input rating of new item and press enter.\n>> ");
    f = scanf("%f%*c", &new_item.rating);

    AddItem(new_item, plist);
    printf("%d : ", CountItems(plist) - 1);
    print_an_item(new_item);
}

void insert_an_item(List* plist) {
    printf("Input the index of item to insert.\n");
    int index = input_int();

    Item* pitem;    //Not actually used
    const bool flag = FindItemByIndex(plist, index, &pitem);
    
    if (flag == false) {
        printf("Invalid index.\n");
        return;
    }

    Item new_item;
    printf("Input title of new item and press enter.\n>> ");
    int f = scanf("%[^\n]%*c", new_item.title);
    printf("Input rating of new item and press enter.\n>> ");
    f = scanf("%f%*c", &new_item.rating);

    InsertByIndex(new_item, plist, index);

    printf("%d : ", index);
    print_an_item(new_item);
}

int check_delete() {
    char confirm;
    while (1) {
        scanf("%c%*[^\n]%*c", &confirm);

        if (confirm == 'Y' || confirm == 'y') {
            break;
        } else if (confirm == 'N' || confirm == 'n') {
            return 1;
        } else {
            printf("Please input Y(y) of N(n)\n>> ");
        }
    } 

    return 0;
}

void delete_an_item(List* plist) {
    printf("Please input the index to delete.\n");
    int index = input_int();

    Item* pitem;
    const bool flag = FindItemByIndex(plist, index, &pitem);

    if (flag == false) {
        printf("Invalid index.\n");
        return;
    }

    printf("Do you really want to delete %d : \"%s\", %.1f ?? \nY/N >> ", index, pitem->title, pitem->rating);

    if (check_delete())
        return;

    RemoveByIndex(plist, index);
}

void delete_all_items(List* plist) {    
    printf("Do you really want to delete all items?\n>> ");

    if (check_delete())
        return;

    unsigned int num = ClearList(plist);

    printf("%d items have been deleted.\n\n", num);
}

bool compare_items(Item a, Item b) {
    if (strstr(a.title, b.title) != NULL)
        return true;
    else 
        return false;
}

void search_by_name(const List* plist) {
    Item item_to_find;

    printf("Please input a keyword.\n>> ");
    if (scanf("%[^\n]%*c", item_to_find.title) != 1) {
        printf("Wrong input.\n");
        return;
    }

    Item item_found;
    int index;
    if (Find(plist, item_to_find, &index, &item_found, compare_items) == false) {
        printf("No movie found : %s\n", item_to_find.title);
        return;
    } else 
        printf("%d : \"%s\", %.1f\n", index, item_found.title, item_found.rating);
}


int main() {
    List movie_list;

    InitializeList(&movie_list);

    read_file(&movie_list);
    
    while (1) {
        printf("\n");

        int user_choice = input_menu();

        switch(user_choice) {
            case 1 :
                print_all(&movie_list);
                break;
            case 2 :
                find_and_print_an_item(&movie_list);
                break;
            case 3 :
                edit_an_item(&movie_list);
                break;
            case 4 :
                add_an_item(&movie_list);
                break;
            case 5 :
                insert_an_item(&movie_list);
                break;
            case 6 :
                delete_an_item(&movie_list);
                break;
            case 7 :
                delete_all_items(&movie_list);
                break;
            case 8 :
                write_file(&movie_list);
                break;
            case 9 :
                search_by_name(&movie_list);
                break;
            case 10 :
                printf("Bye!.\n");
                // delete_all_items(&movie_list);
                exit(0);
            default :
                printf("%d is not implemented.\n", user_choice);
        }
    }


    return 0;
}
