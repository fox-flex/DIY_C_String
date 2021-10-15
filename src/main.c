// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
//! Remember to include ALL the necessary headers!
//! Check and clear errors/warnings "implicit declaration of function <...>" --
//! this is the sign of missed header or misspelled function name in the C code.

#include "c_string.h"

void printStr(my_str_t* str) {
    const char* cstr = my_str_get_cstr(str);
    const size_t len = my_str_size(str);
    for (int i=0; i<len; ++i) {
        printf("%c", cstr[i]);
    }
    printf("\n");
}

int find_biggest_word(my_str_t* str, my_str_t* biggest_word, FILE* file) {
    int err = my_str_read_file(str, file);
    if (err) return err;

    size_t space = my_str_find_c(str, ' ', 0);
    if (space == NOT_FOUND_CODE) {
        my_str_copy(str, biggest_word, 0);
        return 0;
    }
    my_str_t next_word;
    my_str_create(&next_word, 10);
    size_t next_space = my_str_find_c(str, ' ', space + 1);

    my_str_substr(str, biggest_word, 0, space);
    while (next_space != NOT_FOUND_CODE) {
        my_str_substr(str, &next_word, space + 1, next_space);
        int comp = my_str_cmp(biggest_word, &next_word);
        if (comp == -1) {
            my_str_copy(&next_word, biggest_word, 0);
        }
        space = next_space;
        next_space = my_str_find_c(str, ' ', space+1);
    }
    my_str_free(&next_word);
    return 0;
}

int simplePredicate(char symb){ return symb == 'H';}
int main() {
    my_str_t string1, string2;
    my_str_create(&string1, 20);
    my_str_create(&string2, 20);
    char cstring1[] = "Hello, world";
    char cstring2[20];
    char cstring3[] = "llo";

    printf("Test my_str_free\n");
    my_str_free(&string1);
    printf("isEmpty            - %d\n", my_str_empty(&string1));
    printf("my_str_size        - %zu\n", my_str_size(&string1));
    printf("my_str_capacity    - %zu\n\n", my_str_capacity(&string1));

    printf("Test my_str_from_cstr (cstr = 'Hello, world')\n");
    my_str_from_cstr(&string1, cstring1, 20);
    printf("My str: ");
    printStr(&string1);
    printf("\n");

    printf("Test info functions\n");
    printf("my_str_empty       - %d\n", my_str_empty(&string1));
    printf("my_str_size        - %zu\n", my_str_size(&string1));
    printf("my_str_capacity    - %zu\n\n", my_str_capacity(&string1));

    printf("Test putters and getters\n");
    printf("Character at 4: %c\n", my_str_getc(&string1, 4));
    printf("Putting at 4 'O': ");
    my_str_putc(&string1, 4, 'O');
    printf("%s\n\n", my_str_get_cstr(&string1));

    printf("Test my_str_copy\n");
    my_str_copy(&string1, &string2, 0);
    printf("Copy from: ");
    printStr(&string1);
    printf("Copy to: ");
    printStr(&string2);
    printf("\n");

    printf("Test my_str_clear\n");
    my_str_clear(&string1);
    printf("my_str_size after clear - %zu\n\n", my_str_size(&string1));
    my_str_from_cstr(&string1, cstring1, 20);
    my_str_from_cstr(&string2, cstring1, 20);

    printf("Test my_str_insert_c, my_str_insert and my_str_insert_cstr\n");
    printf("Inserting '|' at 6 position: ");
    my_str_insert_c(&string1, '|', 6);
    printStr(&string1);
    printf("Inserting 'Hello, world' as my_str at 6 position: ");
    my_str_insert(&string1, &string2, 6);
    printStr(&string1);
    printf("Inserting 'Hello, world' as cstr at 6 position: ");
    my_str_insert_cstr(&string1, cstring1, 6);
    printStr(&string1);
    my_str_from_cstr(&string1, cstring1, 20);
    printf("\n");

    printf("Test my_str_append_c, my_str_append and my_str_append_cstr\n");
    printf("Appending '|': ");
    my_str_append_c(&string1, '|');
    printStr(&string1);
    printf("Appending 'Hello, world' as my_str: ");
    my_str_append(&string1, &string2);
    printStr(&string1);
    printf("Appending 'Hello, world' as cstr: ");
    my_str_append_cstr(&string1, cstring1);
    printStr(&string1);
    my_str_from_cstr(&string1, cstring1, 20);
    printf("\n");

    printf("Test my_str_substr and my_str_substr_cstr\n");
    printf("Getting my_str from 'Hello, world' from 2 to 8: ");
    my_str_substr(&string1, &string2, 2, 8);
    printStr(&string2);
    printf("Getting cstr from 'Hello, world' from 2 to 8: ");
    my_str_substr_cstr(&string1, cstring2, 2, 8);
    printf("%s\n\n", cstring2);

    printf("Test my_str_erase\n");
    printf("Erasing from 2 to 8: ");
    my_str_erase(&string1, 2, 8);
    printStr(&string1);
    printf("\n");
    my_str_from_cstr(&string1, cstring1, 20);

    printf("Test my_str_popback\n");
    printf("Poping from my_str: %c\n", my_str_popback(&string1));
    printStr(&string1);
    printf("\n");
    my_str_from_cstr(&string1, cstring1, 20);

    printf("Test my_str_reserve\n");
    printf("Reserving to 25\n");
    my_str_reserve(&string1, 25);
    printf("New capacity: %zu\n\n", my_str_capacity(&string1));

    printf("Test my_str_shrink_to_fit\n");
    printf("Shrinking my_str\n");
    my_str_shrink_to_fit(&string1);
    printf("New capacity: %zu\n\n", my_str_capacity(&string1));

    printf("Test my_str_resize\n");
    printf("Resizing my_str to 5\n");
    my_str_resize(&string1, 5, '!');
    printf("New my_str:");
    printStr(&string1);
    printf("Resizing my_str to 20\n");
    my_str_resize(&string1, 20, '!');
    printf("New my_str:");
    printStr(&string1);
    printf("\n");

    printf("Test my_str_find\n");
    my_str_from_cstr(&string1, cstring1, 20);
    my_str_from_cstr(&string2, cstring3, 20);
    printf("Finding 'llo' from 0: %zu\n", my_str_find(&string1, &string2, 0));
    printf("Finding 'llo' from 6: %d\n\n", my_str_find(&string1, &string2, 6));

    printf("Test my_str_cmp\n");
    printf("Comparing 'Hello, world' and 'llo': %d\n\n", my_str_cmp(&string1, &string2));

    printf("Test my_str_cmp_cstr\n");
    printf("Comparing 'Hello, world' and 'Hello, world' as cstr: %d\n\n", my_str_cmp_cstr(&string1, cstring1));

    printf("Test my_str_find_c\n");
    printf("Finding 'w' from 0: %zu\n", my_str_find_c(&string1, 'w', 0));
    printf("Finding '!' from 0: %d\n\n", my_str_find_c(&string1, '!', 0));

    printf("Test my_str_find_if\n");
    printf("Finding 'H' from 0: %zu\n\n",my_str_find_if(&string1, 0, &simplePredicate));

    my_str_free(&string1);
    my_str_free(&string2);


//  Finding biggest word in lexicographical order
    my_str_t str;
    my_str_t biggest_word;
    my_str_create(&str, 10);
    my_str_create(&biggest_word, 10);
    FILE *fptr;
    char path_to_file[100];
    scanf("%s", path_to_file);
    fptr = fopen(path_to_file,"r");
//    fptr = fopen("/path/to/dir/some/paths/AAAAAAAA/lib/src/AAAA.txt","r");
    find_biggest_word(&str, &biggest_word, fptr);
    fclose(fptr);
    printStr(&biggest_word);
    my_str_free(&str);
    my_str_free(&biggest_word);
    return 0;
}
