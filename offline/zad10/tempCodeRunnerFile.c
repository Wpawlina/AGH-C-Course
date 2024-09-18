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

typedef int(cmp_ptr)(const void *, const void *);
typedef int(predicate_ptr)(const void *);
typedef void(read_ptr)(void *);
typedef void(print_ptr)(const void *);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
	vector->data = calloc(block_size, element_size);
	vector->size = 0;
	vector->capacity = block_size;
	vector->element_size = element_size;
}

// If new_capacity is greater than the current capacity,
// new data is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
	if (new_capacity > vector->capacity) {
		vector->data = realloc(vector->data, (vector->element_size) * new_capacity);
		vector->capacity = new_capacity;
	}
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.
// If the current size is less than new_size,
// additional zero-initialized elements are appended.
void resize(Vector *vector, size_t new_size) {
	if (new_size < vector->size) {
		vector->size = new_size;
	} else if (new_size > vector->size) {
		while (new_size > vector->capacity) {
			reserve(vector, 2 * vector->capacity);
		}
		memset((char *)vector->data + vector->size * vector->element_size, 0,
		       (new_size - vector->size) * vector->element_size);
		vector->size = new_size;
	}
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
	if (vector->size + 1 >= vector->capacity) {
		reserve(vector, 2 * vector->capacity);
	}
	memcpy((char *)vector->data + vector->size * vector->element_size, value, vector->element_size);
	vector->size++;
}

// Remove all elements from the vector
void clear(Vector *vector) {
	free(vector->data);
	vector->data = NULL;
	vector->size = 0;
	vector->capacity = 0;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
	if (index == vector->size) {
		push_back(vector, value);
	} else {
		if (vector->size + 1 >= vector->capacity) {
			reserve(vector, 2 * vector->capacity);
		}
		memmove((char *)vector->data + (index + 1) * vector->element_size, 
		        (char *)vector->data + index * vector->element_size, 
		        vector->element_size * (vector->size - index));
		memcpy((char *)vector->data + index * vector->element_size, value, vector->element_size);
		vector->size++;
	}
}

// Erase element at position index
void erase(Vector *vector, size_t index) { // 0 <= index < size
	memmove((char *)vector->data + index * vector->element_size, 
	        (char *)vector->data + (index + 1) * vector->element_size, 
	        vector->element_size * (vector->size - index - 1));
	vector->size--;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp) {
	void *curr_elem;
	for (size_t i = 0; i < vector->size; i++) {
		curr_elem = (char *)vector->data + vector->element_size * i;
		if (cmp(value, curr_elem) == 0) {
			erase(vector, i);
			i--;
		}
	}
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, predicate_ptr predicate) {
	void *curr_elem;
	for (size_t i = 0; i < vector->size; i++) {
		curr_elem = (char *)vector->data + vector->element_size * i;
		if (predicate(curr_elem)) {
			erase(vector, i);
			i--;
		}
	}
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
	if (vector->size < vector->capacity) {
		vector->data = realloc(vector->data, vector->element_size * vector->size);
		vector->capacity = vector->size;
	}
}

// ---------------------------------------------------

typedef struct {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

void print_int(const void *v) {
	printf("%d ", *(int *)v);
}

void print_char(const void *v) {
	printf("%c ", *(char *)v);
}

void print_person(const void *v) {
    Person a = *(Person*)v;
    printf("%i %s %s\n", a.age, a.first_name, a.last_name);
}

void print_vector(const Vector *vector, print_ptr print) {
	printf("%ld\n", vector->capacity);
	void *curr_elem = vector->data;
	for (size_t i = 0; i < vector->size; i++) {
		print(curr_elem);
		curr_elem = (char *)curr_elem + vector->element_size;
	}
}

int int_cmp(const void *v1, const void *v2) {
	return *(const int *)v1 - *(const int *)v2;
}

int char_cmp(const void *v1, const void *v2) {
	return *(const char *)v1 - *(const char *)v2;
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

int is_even(const void *value) {
	return (*(const int *)value) % 2 == 0;
}

int is_vowel(const void *value) {
	char a = *(const char *)value;
	return a == 'a' || a == 'e' || a == 'i' || a == 'o' || a == 'u' || a == 'y' ||
	       a == 'A' || a == 'E' || a == 'I' || a == 'O' || a == 'U' || a == 'Y';
}

int is_older_than_25(void *person) {
    Person p = *(Person*)person;
    return p.age > 25;
}

void read_int(void *value) {
	scanf("%d", (int *)value);
}

void read_char(void *value) {
	scanf(" %c", (char *)value);
}

void read_person(void* value) {
    Person* ptr = (Person*)value;
    scanf("%i %s %s", &ptr->age, ptr->first_name, ptr->last_name);
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
                 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void *v = malloc(vector->element_size);
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
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
			            is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
			            is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
			            person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
