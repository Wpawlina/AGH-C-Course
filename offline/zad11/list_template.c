#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
	struct ListElement *next;
	void *data;
} ListElement;

typedef struct {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	printf("malloc error\n");
	exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
	void *ptr = strdup(string);
	if(ptr) return ptr;
	printf("strdup error\n");
	exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
		CompareDataFp compare_data, DataFp modify_data) {
			p_list->head=NULL;
			p_list->tail=NULL;
			p_list->dump_data=dump_data;
			p_list->free_data=free_data;
			p_list->compare_data=compare_data;
			p_list->modify_data=modify_data;
}

// Print elements of the list
void dump_list(const List* p_list) {
	if(p_list->dump_data == NULL) return;
    ListElement *i;
    for(i=p_list->head; i!=0; i=i->next) p_list->dump_data(i->data);
}
	

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
	ListElement *i=NULL;
	for(i=p_list->head;i!=0;i=i->next)
	{
		if(p_list->compare_data(i->data,data)==0)
		{
			p_list->dump_data(i->data);
		}
	}


	

}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete) {
	free_data(to_delete);
}

// Free all elements of the list
void free_list(List* p_list) {
	ListElement *current = 0;
    current = p_list->head;
    while(current) {
        ListElement *todelete = current;
        current = current->next;
        if(p_list->free_data) p_list->free_data(todelete->data); // zwolnienie pamięci struktury data
        free(todelete);              // zwolnienie pamięci elementu
    }
    p_list->head = NULL;
    p_list->tail = NULL;
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data) {
	ListElement *new=safe_malloc(sizeof(ListElement));
	new->data=data;
	new->next=p_list->head;
	p_list->head=new;
	if (p_list->tail==NULL)
	{
		p_list->tail=new;
	}


}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
	ListElement* list_element = safe_malloc(sizeof(ListElement));
    list_element->next = NULL;
    list_element->data = data;

    if(p_list->head == NULL)
        p_list->head = list_element;
    else
	{
  		p_list->tail->next = list_element;
	}
      
    p_list->tail = list_element;


}

// Remove the first element
void pop_front(List *p_list) {
	if(p_list->head==NULL)
        return;
	if(p_list->head==p_list->tail)
	{
		p_list->tail==NULL;
	}
    ListElement* temp = p_list->head->next;
    p_list->free_data(p_list->head);
    p_list->head = temp;


}

// Reverse the list
void reverse(List *p_list) {
	ListElement *warden=safe_malloc(sizeof(ListElement));
	ListElement *i;
	i=p_list->head;
	p_list->tail=i;
	while(i!=NULL)
	{
		ListElement *temp1=warden->next;
		ListElement *temp2=i->next;
		warden->next=i;
		i->next=temp1;
		i=temp2;
	}
	p_list->head=warden->next;
	p_list->tail->next=NULL;



}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
	ListElement* insertion_position = NULL;
    ListElement* current_position = p_list->head;

    while(current_position != NULL){
        if(p_list->compare_data(current_position->data, p_element->data) <= 0)
            insertion_position = current_position;

        current_position = current_position->next;
    }

    return insertion_position;

}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
	ListElement *new=safe_malloc(sizeof(ListElement));
	new->data=data;

	if (previous!=NULL)
	{
		
		ListElement *temp=previous->next;
		new->next=temp;
		previous->next=new;
	}
	else
	{
		push_front(p_list,new->data);
		free(new);
		
	}
	

	
	
	
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) 
{
	ListElement *new=safe_malloc(sizeof(ListElement));
	new->data=p_data;
	ListElement *prev=NULL;
	prev=find_insertion_point(p_list,new);
	if(prev!=NULL)
	{
		 if(p_list->compare_data(prev->data, new->data) == 0)
		 {
            if(p_list->modify_data != NULL)
			{
                p_list->modify_data(prev->data);
            }
            p_list->free_data(p_data);
		}
		else
		{
			if(p_list->tail == prev)
                p_list->tail = new;
			ListElement *temp=prev->next;
			new->next=temp;
			prev->next=new;
		}
		
	}
	else
	{
		push_front(p_list,new->data);
		free(new);
	}
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d) {
	int* _d=(int*)d;
	printf("%d ",*_d);

}

void free_int(void *d) {
	free((int*)d);
}

int cmp_int(const void *a, const void *b) 
{
	int _a = *(int*)a;
    int  _b = *(int*)b;

    if(_a < _b)
        return -1;
    if (_a > _b)
        return 1;
    return 0;
}



int *create_data_int(int v) {
	int* address = safe_malloc(sizeof(int));
    *address = v;

    return address;

}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d) {
	 DataWord* data = (DataWord*)d;

    printf("%s ", data->word);

}

void free_word(void *d) {
	 free((DataWord*)d);
}

int cmp_word_alphabet(const void *a, const void *b) {
	DataWord* _a = (DataWord*)a;
    DataWord* _b = (DataWord*)b;

    return strcmp(_a->word, _b->word);

}

int cmp_word_counter(const void *a, const void *b) {
	DataWord* _a = (DataWord*)a;
    DataWord* _b = (DataWord*)b;

    if(_a->counter < _b->counter)
        return -1;
    if(_a->counter > _b->counter)
        return 1;
    return 0;

}

void modify_word(void *p) {
	DataWord* data = (DataWord*)p;
    data->counter++;

}

void *create_data_word(const char *string, int counter) {
	DataWord* new_data = safe_malloc(sizeof(DataWord));

    new_data->word = safe_strdup(string);
    new_data->counter = counter;

   

    return new_data;


}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
		DataWord *data;
		char *p, buf[BUFFER_SIZE];
		const char delimits[] = " \r\t\n.,?!:;-";
		if(cmp) p_list->compare_data = cmp;
		while(fgets(buf,BUFFER_SIZE,stream)) {
			p = strtok(buf, delimits);
			while(p) {
				data = create_data_word(p, 1);
				if(cmp) insert_in_order(p_list,data);
				else    push_back(p_list, data);
				p = strtok(NULL,delimits);
			}
		}

}

// test integer list
void list_test(List *p_list, int n) {
	char op;
	int v;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		switch (op) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int, cmp_int, NULL);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word, NULL, NULL);
			stream_to_list(&list, stdin, NULL);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word, free_word, NULL, modify_word);
			stream_to_list(&list, stdin, cmp_word_alphabet);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			dump_list_if(&list, &data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

