#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

#pragma GCC diagnostic error "-Wpointer-arith"

typedef struct Vector {
    void *data;
    size_t element_size;
    size_t size;
    size_t capacity;
} Vector;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void init_vector(Vector *vector, size_t block_size, size_t element_size) {
    vector->data = safe_malloc(block_size * element_size);
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = block_size;
}

void reserve(Vector *vector, size_t new_capacity) {
    if (new_capacity > vector->capacity) {
        vector->data = realloc(vector->data, new_capacity * vector->element_size);
        vector->capacity = new_capacity;
    }
}

void resize(Vector *vector, size_t new_size) {
    if (new_size > vector->size) {
        if (new_size > vector->capacity) {
            reserve(vector, new_size);
        }
        memset((char*)vector->data + vector->size * vector->element_size, 0, (new_size - vector->size) * vector->element_size);
    }
    vector->size = new_size;
}

void push_back(Vector *vector, void *value) {
    if (vector->size == vector->capacity) {
        reserve(vector, vector->capacity * 2);
    }
    memcpy((char*)vector->data + vector->size * vector->element_size, value, vector->element_size);
    vector->size++;
}

void clear(Vector *vector) {
    vector->size = 0;
}

void insert(Vector *vector, size_t index, void *value) {
    if (vector->capacity == vector->size) {
        reserve(vector, vector->capacity * 2);
    }
    memmove((char*)vector->data + vector->element_size * (index + 1),
            (char*)vector->data + vector->element_size * index,
            (vector->size - index) * vector->element_size);
    memcpy((char*)vector->data + vector->element_size * index, value, vector->element_size);
    vector->size++;
}

void erase(Vector *vector, size_t index) {
    memmove((char*)vector->data + index * vector->element_size,
            (char*)vector->data + (index + 1) * vector->element_size,
            (vector->size - index - 1) * vector->element_size);
    vector->size--;
}

void erase_value(Vector *vector, void *value, cmp_ptr cmp) {
    for (size_t i = 0; i < vector->size; i++) {
        if (cmp((char*)vector->data + i * vector->element_size, value) == 0) {
            erase(vector, i);
            i--;
        }
    }
}

void erase_if(Vector *vector, int (*predicate)(void *)) {
    for (size_t i = 0; i < vector->size; i++) {
        if (predicate((char*)vector->data + i * vector->element_size) == 1) {
            erase(vector, i);
            i--;
        }
    }
}

void shrink_to_fit(Vector *vector) {
    vector->data = realloc(vector->data, vector->size * vector->element_size);
    vector->capacity = vector->size;
}

typedef struct {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;

void print_int(const void *v) {
    int a = *(int*)v;
    printf("%i ", a);
}

void print_char(const void *v) {
    char a = *(char*)v;
    printf("%c ", a);
}

void print_person(const void *v) {
    Person a = *(Person*)v;
    printf("%i %s %s\n", a.age, a.first_name, a.last_name);
}

void print_vector(Vector *vector, print_ptr print) {
    printf("%zu\n", vector->capacity);
    for (size_t i = 0; i < vector->size; i++) {
        print((char*)vector->data + i * vector->element_size);
    }
}

int int_cmp(const void *v1, const void *v2) {
    int a = *(int*)v1;
    int b = *(int*)v2;
    return (a > b) - (a < b);
}

int char_cmp(const void *v1, const void *v2) {
    char a = *(char*)v1;
    char b = *(char*)v2;
    return (a > b) - (a < b);
}

int person_cmp(const void *p1, const void *p2) {
    Person a = *(Person*)p1;
    Person b = *(Person*)p2;

    int age = int_cmp(&a.age, &b.age);
    if (age != 0) return -age;

    int first_name = strcmp(a.first_name, b.first_name);
    if (first_name != 0) return first_name;

    int last_name = strcmp(a.last_name, b.last_name);
    return last_name;
}

int is_even(void *value) {
    int a = *(int*)value;
    return a % 2 == 0;
}

int is_vowel(void *value) {
    char chr = *(char*)value;
    return strchr("aeiouAEIOU", chr) != NULL;
}

int is_older_than_25(void *person) {
    Person p = *(Person*)person;
    return p.age > 25;
}

void read_int(void* value) {
    scanf("%i", (int*)value);
}

void read_char(void* value) {
    scanf(" %c", (char*)value);
}

void read_person(void* value) {
    Person* ptr = (Person*)value;
    scanf("%i %s %s", &ptr->age, ptr->first_name, ptr->last_name);
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
                 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
    init_vector(vector, block_size, elem_size);
    void *v = safe_malloc(vector->element_size);
    size_t index, size;
    for (int i = 0; i < n; ++i) {
        char op;
        scanf(" %c", &op);
        switch (op) {
            case 'p': // push_back
                read(v);
                push_back(vector, v);
                break;
            case 'i': // insert
                scanf("%zu", &index);
                read(v);
                insert(vector, index, v);
                break;
            case 'e': // erase
                scanf("%zu", &index);
                erase(vector, index);
                break;
            case 'v': // erase_value
                read(v);
                erase_value(vector, v, cmp);
                break;
            case 'd': // erase_if
                erase_if(vector, predicate);
                break;
            case 'r': // resize
                scanf("%zu", &size);
                resize(vector, size);
                break;
            case 'c': // clear
                clear(vector);
                break;
            case 'f': // shrink_to_fit
                shrink_to_fit(vector);
                break;
            case 's': // sort
                qsort(vector->data, vector->size, vector->element_size, cmp);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
    print_vector(vector, print);
    free(vector->data);
    free(v);
}

int main(void) {
    int to_do, n;
    Vector vector_int, vector_char, vector_person;

    scanf("%d%d", &to_do, &n);

    switch (to_do) {
        case 1:
            vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp, is_even, print_int);
            break;
        case 2:
            vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp, is_vowel, print_char);
            break;
        case 3:
            vector_test(&vector_person, 2, sizeof(Person), n, read_person, person_cmp, is_older_than_25, print_person);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }

    return 0;
}
