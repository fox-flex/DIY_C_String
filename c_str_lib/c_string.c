// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <c_string.h>

// ALL of functions implementation should be here.

/*
 * Creates empty dynamic string (my_str_t)
 * !important! user should always use my_str_create before using ANY other function
 * str: pointer to my_str_t struct that user wants to create
 * buf_size: size of mem (in bytes) that user wants to allocate for buffer
 * return:
 *      0 if OK
 *      NULL_PTR_ERR if str points to NULL
 *      MEMORY_ALLOCATION_ERR if there was an error when allocating memory for buffer
 */
int my_str_create(my_str_t* str, size_t buf_size) { // Sasha
    if (!str) return NULL_PTR_ERR;
    str->data = (char*) malloc(buf_size + 1);
    if (!str->data) return MEMORY_ALLOCATION_ERR;
    str->size_m = 0;
    str->capacity_m = buf_size;
    return 0;
}

/*
 * frees all data from given my_str_t structure
 * return:
 *     0 always
 */
int my_str_free(my_str_t* str) { // Sasha
    if (str) {
        free(str->data);
        str->data = NULL;
        str->size_m = 0;
        str->capacity_m = 0;
    }
    return 0;
}

/*
 * makes content of given my_str-string the same as given c-string.
 * cstr: c-string from which data should be taken
 * buf_size: new size of buffer for given; if buf_size == 0 then new buffer size = length of c-string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or cstr is NULL
 *      BUFF_SIZE_ERR if buf_size if incorrect (too small)
 *      MEMORY_ALLOCATION_ERR if there occurred error while mem allocation
 */
int my_str_from_cstr(my_str_t* str, const char* cstr, size_t buf_size) { // Sasha
    if (!str || !cstr) return NULL_PTR_ERR;
    size_t len = 0;
    while(cstr[len]) {
        ++len;
    }
    if (buf_size == 0) {
        int err = my_str_resize(str, len, '&');
        if (err) return err;
        err = my_str_shrink_to_fit(str);
        if (err) return err;
    } else if (buf_size < len) {
        return BUFF_SIZE_ERR;
    } else {
        if (len > str->capacity_m) {
            int err = my_str_reserve(str, buf_size);
            if (err) return err;
        }
    }
    for (size_t i=0; i<len; ++i) {
        (str->data)[i] = cstr[i];
    }
    str->size_m = len;
    return 0;
}

/*
 * returns actual size of my_str-string
 * if str == NULL than size = 0
 */
size_t my_str_size(const my_str_t* str) { // Mykola
    return str ? str->size_m : 0;
}

/*
 * returns size of allocated memory - 1 for my_str-cstring
 * if str == NULL than capacity = 0
 */
size_t my_str_capacity(const my_str_t* str) { // Mykola
    return str ? str->capacity_m : 0;
}

/*
 * returns 1 if string is empty
 * if str == NULL than it is empty
 */
int my_str_empty(const my_str_t* str) { // Mykola
    return !str || str->size_m == 0;
}

/*
 * returns symbol on given index in my_str-string
 * return:
 *      NULL_PTR_ERR if str == NULL
 *      RANGE_ERR if index is bad
 */
int my_str_getc(const my_str_t* str, size_t index) { // Mykola
    return !str ? NULL_PTR_ERR : str->size_m <= index ? RANGE_ERR : (str->data)[index];
}

/*
 * puts given symbol on given position in my_str-string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or c is NULL
 *      RANGE_ERR if index is bad
 */
int my_str_putc(my_str_t* str, size_t index, char c) { // Mykola
    if (!str || !c) return NULL_PTR_ERR;
    if (str->size_m <= index) return RANGE_ERR;
    str->data[index] = c;
    return 0;
}

/*
 * returns c-string with the same content as my_str-string
 * result c-string is constant and can be incorrect after changing my_str content
 * returns NULL if str is NULL
 */
const char* my_str_get_cstr(my_str_t* str) { // Mykhailo
    if (!str) return NULL;
    str->data[str->size_m] = '\0';
    return str->data;
}

/*
 * copies content of one my_str-string to other
 * after manipulations with second string, its buffer will be like capacity of first
 * if reserve == 1 else if reserve == 0 size of first
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if one of string or both are NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_copy(const my_str_t* from,  my_str_t* to, int reserve) { // Sasha
    if (!from || !to) return NULL_PTR_ERR;
    int err = 0;
    if (to->capacity_m < from->size_m) {
        if (reserve)
            err = my_str_reserve(to, from->capacity_m);
        else
            err = my_str_reserve(to, from->size_m);
    }
    if (err) return err;
    for (size_t i=0; i<from->size_m; ++i) {
        to->data[i] = from->data[i];
    }
    to->size_m = from->size_m;
    return 0;
}

/*
 *  clears content of string, does nothing if string is NULL
 *  return:
 *      always 0
 */
int my_str_clear(my_str_t* str) { // Mykola
    if (str) {
        str->size_m = 0;
    }
    return 0;
}

/*
 * inserts given char in the given position in my_str-cstring
 * if needed increases buffer
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or c is NULL
 *      RANGE_ERR if position is bad
 *      MEMORY_ALLOCATION_ERR if occurred error during memory allocation
 */
int my_str_insert_c(my_str_t* str, char c, size_t pos) { // Mykhailo
    if (!str || !c) return NULL_PTR_ERR;
    if (pos > str->size_m) return RANGE_ERR;
    if (str->size_m >= str->capacity_m) {
        int err = my_str_reserve(str, str->capacity_m ? INC_PARAM * (str->capacity_m) : INC_PARAM);
        if (err) return err;
    }
    if (str->size_m) {
        for (size_t i = str->size_m - 1; i >= pos; --i) {
            str->data[i + 1] = str->data[i];
            if (i == 0) break;
        }
    }
    str->data[pos] = c;
    ++str->size_m;
    return 0;
}

/*
 * inserts given my_str-string into another one at given position
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or from is NULL
 *      RANGE_ERR if position is bad
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_insert(my_str_t* str, const my_str_t* from, size_t pos) { // Mykhailo
    if (!str || !from) return NULL_PTR_ERR;
    if (pos > str->size_m) return RANGE_ERR;
    if (str->size_m + from->size_m > str->capacity_m) {
        int err = my_str_reserve(str, INC_PARAM * (str->size_m + from->size_m));
        if (err) return err;
    }
    if (str->size_m) {
        for (size_t i = str->size_m - 1; i >= pos; --i) {
            str->data[i + from->size_m] = str->data[i];
            if (i == 0) break;
        }
    }
    for (size_t i = pos; i < pos + from->size_m; ++i) {
        str->data[i] = from->data[i-pos];
    }
    str->size_m += from->size_m;
    return 0;
}

/*
 * inserts given c-string into given my_str-string\
 * return:
 *      the same as in my_str_insert(...) function
 */
int my_str_insert_cstr(my_str_t* str, const char* from, size_t pos) { // Mykhailo
    if (!str || !from) return NULL_PTR_ERR;
    if (pos > str->size_m) return RANGE_ERR;
    size_t sizeFrom = 0;
    while (from[sizeFrom]) {
        ++sizeFrom;
    }
    if (str->size_m + sizeFrom > str->capacity_m) {
        int err = my_str_reserve(str, INC_PARAM * (str->size_m + sizeFrom));
        if (err) return err;
    }
    if (str->size_m) {
        for (size_t i = str->size_m - 1; i >= pos; --i) {
            str->data[i + sizeFrom] = str->data[i];
            if (i == 0) break;
        }
    }
    for (size_t i = pos; i < pos + sizeFrom; ++i) {
        str->data[i] = from[i-pos];
    }
    str->size_m += sizeFrom;
    return 0;
}

/*
 * appends one my_str-string to another
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if from or str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_append(my_str_t* str, const my_str_t* from) { // Mykhailo
    if (!str || !from) return NULL_PTR_ERR;
    if (str->size_m + from->size_m > str->capacity_m) {
        int err = my_str_reserve(str, INC_PARAM * (str->size_m + from->size_m));
        if (err) return err;
    }
    for (size_t i=0; i < from->size_m; ++i) {
        str->data[i+str->size_m] = from->data[i];
    }
    str->size_m += from->size_m;
    return 0;
}

/*
 * appends c-string to my_str-string
 * return: the same as in my_str_append(...) function
 */
int my_str_append_cstr(my_str_t* str, const char* from) { // Mykhailo
    if (!str || !from) return NULL_PTR_ERR;
    size_t sizeFrom = 0;
    while (from[sizeFrom]) {
        ++sizeFrom;
    }
    if (str->size_m + sizeFrom > str->capacity_m) {
        int err = my_str_reserve(str, INC_PARAM * (str->capacity_m + sizeFrom));
        if (err) return err;
    }
    for (size_t i=0; i < sizeFrom; ++i) {
        str->data[i+str->size_m] = from[i];
    }
    str->size_m += sizeFrom;
    return 0;
}


/*
 * pushes given char on the end of my_str-string
 * if needed increases buffer by INC_PARAM (2)
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or c is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during allocating memory for buffer
 */
int my_str_append_c(my_str_t* str, char c) { // Mykhailo
    if (!str || !c) return NULL_PTR_ERR;
    if (str->size_m >= str->capacity_m) {
        int err = my_str_reserve(str, str->capacity_m ? INC_PARAM*(str->capacity_m) : INC_PARAM);
        if (err) {
            return err;
        }
    }
    str->data[str->size_m++] = c;
    return 0;
}

/*
 * saves substring of my_str-string in given bounds to given my_str-string
 * return:
 *      0  if oK
 *      NULL_PTR_ERR if from or to is NULL
 *      RANGE_ERR if boundaries are bad
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_substr(const my_str_t* from, my_str_t* to, size_t beg, size_t end) { // Mykhailo
    if (!from || !to) return NULL_PTR_ERR;
    if (beg < 0 || beg > from->size_m || end < 0 || beg > end) return RANGE_ERR;
    if (from->size_m<end) {
        end = from->size_m;
    }
    size_t len = end-beg;
    if (end - beg > to->capacity_m) {
        int err = my_str_reserve(to, INC_PARAM * len);
        if (err) return err;
    }
    for (size_t i = beg; i < end; ++i) {
        to->data[i-beg] = from->data[i];
    }
    to->size_m = len;
    return 0;
}

/*
 * saves my_str-string substring in given bounds to given c-string
 * !important! given c-string should be big enough
 * return:
 *      0  if oK
 *      NULL_PTR_ERR if from or to is NULL
 *      RANGE_ERR if boundaries are bad
 */
int my_str_substr_cstr(const my_str_t* from, char* to, size_t beg, size_t end) { // Mykhailo
    if(!from || !to) return NULL_PTR_ERR;
    if (beg < 0 || beg > from->size_m || end < 0 || beg > end) return RANGE_ERR;
    if (from->size_m<end) {
        end = from->size_m;
    }
    size_t len = 0;
    while (to[len]) {
        ++len;
    }
    for(size_t i = beg; i < end; ++i){
        to[i-beg] = from->data[i];
    }
    to[end-beg] = '\0';
    return 0;
}

/*
 * return symbols with indexes [beg, end), shifts reminder to the left
 * end > size is not an error - erase all possible
 * beg > size is an error
 * return:
 *      0 if OK
 *      RANGE_ERR if boundaries are bad
 *      MEMORY_ALLOCATION_ERROR if there was an error during memory allocation
 *      NULL_PTR_ERR if from is NULL
 */
int my_str_erase(my_str_t* str, size_t beg, size_t end) { // Mykhailo
    if (!str) return NULL_PTR_ERR;
    if (str->size_m < beg || beg > end) return RANGE_ERR;
    if (str->size_m < end) {
        end = str->size_m;
    }
    for (size_t i=end; i<str->size_m; ++i) {
        str->data[i-end+beg] = str->data[i];
    }
    str->size_m -= end-beg;
    return 0;
}

/*
 * pops last symbol in the my_str-string
 * return:
 *      (int) lastChar if OK
 *      RANGE_ERR if size of string is already 0
 *      NULL_PTR_ERR if str is NULL
 *      else - popped symbol
 */
int my_str_popback(my_str_t* str) { // Mykola
    if (!str) return NULL_PTR_ERR;
    if (!str->size_m) return RANGE_ERR;
    return (int) str->data[--str->size_m];
}

/*
 * increases given my_str-string's buffer to given value.
 * if buf_size <= my_str-string capacity, does nothing
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_reserve(my_str_t* str, size_t buf_size) { // Mykhailo
    if (!str) return NULL_PTR_ERR;
    if (str->capacity_m < buf_size) {
        char* new = (char*) malloc(buf_size + 1);
        if (!new) return MEMORY_ALLOCATION_ERR;
        for (size_t i=0; i < str->size_m; ++i) {
            new[i] = str->data[i];
        }
        free(str->data);
        str->data = new;
        str->capacity_m = buf_size;
    }
    return 0;
}

/*
 * decreases buffer of given my_str-string to size of string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if from ot str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_shrink_to_fit(my_str_t* str) { // Mykhailo
    if (!str) return NULL_PTR_ERR;
    if (str->size_m < str->capacity_m) {
        char* new = (char*) malloc(str->size_m + 1);
        if (!new) return MEMORY_ALLOCATION_ERR;
        for (size_t i=0; i < str->size_m; ++i) {
            new[i] = str->data[i];
        }
        free(str->data);
        str->data = new;
        str->capacity_m = str->size_m;
    }
    return 0;
}

/*
 * resizes my_str-string to given size.
 * if given size > size of string than fills next bytes with given chars
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_resize(my_str_t* str, size_t new_size, char sym) { // Mykhailo
    if (!str) return NULL_PTR_ERR;
    if (str->size_m < new_size) {
        if (str->capacity_m < new_size) {
            int err = my_str_reserve(str, INC_PARAM*new_size);
            if (err) return err;
        }
        for (size_t i=str->size_m; i < new_size; ++i) {
            str->data[i] = sym;
        }
    }
    str->size_m = new_size;
    return 0;
}

/*
 * returns position of given my_str-substring in the other my_str-string if it is there, else -1
 * starts search from given position
 * return:
 *      position of substring if it is in the given my_str-string
 *      NOT_FOUND_CODE if there is no given substring
 *      NULL_PTR_ERR if str or to find is NULL
 */
size_t my_str_find(const my_str_t* str, const my_str_t* toFind, size_t from) { // Sasha
    if(!str || ! toFind) return NULL_PTR_ERR;
    if(toFind->size_m == 0) return NOT_FOUND_CODE;
    size_t len = str->size_m;
    while (from < len) {
        size_t curPos = from;
        while (curPos-from < toFind->size_m && curPos < len &&
               str->data[curPos] == toFind->data[curPos-from]) {
            ++curPos;
        }
        if (curPos - from == toFind->size_m) return from;
        if (curPos >= len) return NOT_FOUND_CODE;
        ++from;
    }
    return NOT_FOUND_CODE;
}

/*
 * compares two my_str-strings like conventional c-strings (in lexicographical order)
 * return:
 *      0 if str1 == str2
 *      -1 if str1 < str2
 *      1  if str1 > str2
 *      NULL_PTR_ERR if str1 or str2 is NULL
 */
int my_str_cmp(const my_str_t* str1, const my_str_t* str2) { // Mykola
    if (!str1 || !str2) return NULL_PTR_ERR;
    size_t maxCompare = str1->size_m < str2->size_m ? str1->size_m : str2->size_m;
    size_t i = 0;
    while (i < maxCompare && str1->data[i] == str2->data[i]) {
        ++i;
    }
    if (i == maxCompare) {
        return str1->size_m < str2->size_m ? -1 : str1->size_m > str2->size_m ? 1 : 0;
    } else {
        return str1->data[i] < str2->data[i] ? -1 : 1;
    }
}

/*
 * compares my_str-strings and c-string lexicographical order
 * return: the same as in my_str_cmp
 */
int my_str_cmp_cstr(const my_str_t* str1, const char* cstr2) { // Mykola
    if (!str1 || !cstr2) return NULL_PTR_ERR;
    size_t i = 0;
    while(i < str1->size_m && cstr2[i] != '\0' && cstr2[i] == str1->data[i]) {
        ++i;
    }
    if (i == str1->size_m || cstr2[i] == '\0') {
        return i == str1->size_m && cstr2[i] == '\0' ? 0 : i == str1->size_m ? -1 : 1;
    } else {
        return str1->data[i] < cstr2[i] ? -1 : 1;
    }
}

/*
 * returns position of given symbol in my_str-string, -1 if it's not there
 * starts search from given position
 * return:
 *      position of char symbol if its in string
 *      NOT_FOUND_CODE if it's not in string
 *      NULL_PTR_ERR if str is NULL
 */
size_t my_str_find_c(const my_str_t* str, char toFind, int from) { // Sasha
    if (!str) return NULL_PTR_ERR;
    for (size_t i=from; i < str->size_m; ++i) {
        if (str->data[i] == toFind)
            return i;
    }
    return NOT_FOUND_CODE;
}

/*
 * returns position of symbol that predicate on ot returns 1, if there is no symbol like that than -1
 * return:
 *      position of char symbol that satisfies predicate
 *      NOT_FOUND_CODE if it's not in string
 *      NULL_PTR_ERR if str or predicate is NULL
 */
size_t my_str_find_if(const my_str_t* str, int beg, int (*predicate)(int)) { // Sasha
    if(!str || !predicate) return NULL_PTR_ERR;
    size_t len = str->size_m;
    for (size_t i = beg; i < len; ++i) {
        if(predicate(str->data[i]))
            return i;
    }
    return NOT_FOUND_CODE;
}

/*
 * reads file and saves its content into given my_str-string
 * resizes string's buffer if needed
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or file is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_read_file(my_str_t* str, FILE* file) { // Sasha
    if(!str || !file) return NULL_PTR_ERR;
    const int n = 128;
    char buf[n];
    str->size_m = 0;
    while (fgets(buf, n, file) != NULL) {
        int err = my_str_append_cstr(str, buf);
        if (err) return err;
    }
    if(ferror(file)) return IO_READ_ERR;
    return 0;
}

/*
 * reads content from stdin and saves to my_str-string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during increasing buffer
 *      IO_READ_ERR if there was error while reading
 */
int my_str_read(my_str_t* str) { // Mykola
    int err = my_str_read_file(str, stdin);
    if (err) return err;
    --str->size_m;
    return 0;
}

/*
 * reads file and saves its content into my_str-string, stops when reached delimiter or EOF
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or file is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_read_file_delim(my_str_t* str, FILE* file, char delimiter) { // Sasha
    if(!str || !file) return NULL_PTR_ERR;
    const int n = 128;
    char buf[n];
    str->size_m = 0;
    while (fgets(buf, n, file) != NULL) {
        size_t i = 0;
        while (i < n) {
            if(buf[i] == delimiter) {
                buf[i]='\0';
                break;
            }
            i++;
        }
        int err = my_str_append_cstr(str, buf);
        if (err) return err;
    }
    if(ferror(file)) return IO_READ_ERR;
    return 0;
}

/*
 * writes content of given my_str-string to given file
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or file is NULL
 *      IO_WRITE_ERR if error occurred while writing
 */
int my_str_write_file(const my_str_t* str, FILE* file) { // Sasha
    if(!str || !file) return NULL_PTR_ERR;
    for(size_t i = 0; i < str->size_m; ++i)
        if(putc(str->data[i], file) == EOF) return IO_WRITE_ERR;
    return 0;
}

/*
 * writes content of given my_str-string into console
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      IO_WRITE_ERR if error occurred while writing
 */
int my_str_write(const my_str_t* str) { // Sasha
    if(!str) return NULL_PTR_ERR;
    for(size_t i = 0; i < str-> size_m; ++i)
        if(putc(str->data[i], stdout) == EOF) return IO_WRITE_ERR;
    return 0;
}
