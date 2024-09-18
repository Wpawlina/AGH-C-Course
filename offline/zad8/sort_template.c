#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day; 
	int month; 
	int year;
} Date;

// 1 bsearch2

#define NAME_MAX  20   // max. length of name + 1
#define FOOD_MAX  30   // max. number of goods
#define RECORD_MAX 64  // max. line length

typedef struct {
	char name[NAME_MAX];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

int cmp_date(const void *d1, const void *d2) {
	Date a = *((Date*)d1);
    Date b = *((Date*)d2);

    if(a.year < b.year)
        return -1;
    else if(a.year > b.year)
        return 1;
    else {
        if(a.month < b.month)
            return -1;
        else if(a.month > b.month)
            return 1;
        else{
            if(a.day < b.day)
                return -1;
            else if(a.day > b.day)
                return 1;
            else
                return 0;
        }
    }

}

int cmp(const void *a, const void *b) {
	Food *fir=(Food *)a;
    Food *sec=(Food *)b;
    int f=strcmp(fir->name,sec->name);
    if (f!=0) return f;
    double d=fir->price-sec->price;
    if (d>0) return 1;
    else if (d<0) return -1;
    return cmp_date(a,b);

}

void* bsearch2 (const void *key,  void *base, const size_t nmemb,
		const size_t size, const ComparFp compar, char *result) 
{
		int P=0,K=nmemb-1;
		int middle=(P+K)/2;
		while (P<=K)
		{
			void *middle_ptr=base+middle*size;
			int cmpr=compar(middle_ptr,key);
			if(cmpr==0)
			{
				*result=1;
				return middle_ptr;
			}
			else if(cmpr>0)
			{
				K=middle-1;
			}
			else
			{
				P=middle+1;
			}	
		}
		*result=0;
		return base+P*size;
		

}

void print_art(Food *p, const int n, const char *art) {
	 for(int i = 0; i < n; i++){
        if(strcmp(p[i].name, art) == 0)
            printf("%.2f %d %i %i %i \n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month,
                   p[i].valid_date.year);
    }
}

Food* add_record( Food *tab, int *np, const ComparFp compar, const Food *new) {
	char result;
	Food *result_ptr=(Food *)bsearch2(new,tab,*np,sizeof(Food),compar,&result);
	if(result)
	{
		result_ptr->amount+=new->amount;
		return result_ptr;
	}
	*np=*np+1;
	if(*np=FOOD_MAX)
	{
		return NULL;
	}
	for (Food *j=tab+*np;j>result_ptr;j--){
        *j=*(j-1);
    }
    *result_ptr=*new;
    return result_ptr;
	





}

int read_goods(Food *tab, const int no, FILE *stream, const int sorted) {
	int curr_size=0;
    Food new_f;
    for (int i=0; i<no; i++){
        fscanf(stream,"%s %f %d %d %d %d",new_f.name,&new_f.price,&new_f.amount,&new_f.valid_date.day,&new_f.valid_date.month,&new_f.valid_date.year);
        add_record(tab,&curr_size,cmp,&new_f);
    }
    return curr_size;
}

int cmp_qs(const void *a, const void *b) {
	const Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
	const Date *pd = (Date*)a;
	const Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}

float value(Food *food_tab, size_t n, Date curr_date, int anticip) {
    time_t raw_time;
    time(&raw_time);

    struct tm* target_date = localtime(&raw_time);
    target_date->tm_mday = curr_date.day;
    target_date->tm_mon = curr_date.month - 1;
    target_date->tm_year = curr_date.year - 1900;

    mktime(target_date);

    target_date->tm_mday += anticip;

    mktime(target_date);

    Date valid_date = (Date){target_date->tm_mday, target_date->tm_mon + 1, target_date->tm_year + 1900};

    qsort(food_tab, n, sizeof(Food), cmp_date);
    Food f = {
            .valid_date = valid_date,
    };
    char res;
    Food* address = bsearch2(&f, food_tab, n, sizeof(Food), cmp_date, &res);

    if(res == 0)
        return 0;

    float result = 0;

    int index = (int)(address - food_tab);
    int tmp = index;

    while(tmp >= 0 && cmp_date(&food_tab[tmp].valid_date, &valid_date) == 0){
        result += food_tab[tmp].price * food_tab[tmp].amount;
        tmp--;
    }
    tmp = index + 1;

    while(tmp < n && cmp_date(&food_tab[tmp].valid_date, &valid_date) == 0){
        result += food_tab[tmp].price * food_tab[tmp].amount;
        tmp++;
    }

    return result;
}

/////////////////////////////////////////////////////////////////
// 8.3 Sukcesja

#define CHILD_MAX  20   // maksymalna liczba przesuwanej w tablicy grupy osób (elementów tablicy)

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
    enum Sex sex:1;
    enum BOOL pretendent:1;   // =no - nie pretenduje (panuje albo nie żyje) ale jest w zbiorze dla spójności relacji.
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
    char *par_name;
    int index_first;
    int index_last;
} Parent;    // strukturę można rozbudować o pole liczby dzieci

const Date primo_date = { 28, 10, 2011 }; // Data zmiany kolejności sukcesji

int person_cmp_parent(const void* _a, const void* _b){
    Person a = *((Person*)_a);
    Person b = *((Person*)_b);

    if(a.parent == NULL)
        return 1;
    if(b.parent == NULL)
        return -1;

    return strcmp(a.parent, b.parent);
}

int parent_cmp(const void* _a, const void* _b){
    Parent a = *((Parent*)_a);
    Parent b = *((Parent*)_b);

    return strcmp(a.par_name, b.par_name);
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
    int no_parents = 0;

    for(int i = 0; i < size; i++){
        int found = 0;

        for(int u = 0; u < no_parents; u++){
            if(strcmp(pers_tab[i].name, idx_tab[u].par_name) == 0){
                found = 1;
                break;
            }
        }

        if(!found){
            idx_tab[no_parents].par_name = pers_tab[i].name;

            Person key = {
                    .parent = pers_tab[i].name
            };

            Person* address = bsearch(&key, pers_tab, size, sizeof(Person), person_cmp_parent);

            if(address == NULL)
                continue;

            int index = (int)(address - pers_tab);
            int tmp = index;

            while(tmp > 0 && person_cmp_parent(&pers_tab[tmp], &pers_tab[tmp-1]) == 0)
                tmp--;

            idx_tab[no_parents].index_first = tmp;
            tmp = index;

            while(tmp < size - 1 && person_cmp_parent(&pers_tab[tmp], &pers_tab[tmp+1]) == 0)
                tmp++;

            idx_tab[no_parents].index_last = tmp;

            no_parents++;
        }
    }

    qsort(idx_tab, no_parents, sizeof(Parent), parent_cmp);

    return no_parents;
}

void persons_shiftings(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
    int index = 0;

    Person tmp[33];

    while (index < size){
        Parent key = {.par_name = person_tab[index].name};
        Parent* address = bsearch(&key, idx_tab, no_parents, sizeof(Parent), parent_cmp);

        if(address != NULL) {
            int to_copy = address->index_last - address->index_first + 1;

            memmove(tmp, &person_tab[address->index_first], to_copy * sizeof(Person));
            memmove(&person_tab[index + to_copy + 1], &person_tab[index + 1],
                    ((address->index_first) - index - 1) * sizeof(Person));
            memmove(&person_tab[index + 1], tmp, to_copy * sizeof(Person));

            for(int i = 0; i < no_parents; i++){
                if(idx_tab[i].index_first < address->index_first) {
                    idx_tab[i].index_first += to_copy;
                    idx_tab[i].index_last += to_copy;
                }
            }
        }

        index += 1;
    }
}

int cleaning(Person *person_tab, int n) {
    for(int i = 0; i < n; i++){
        if(person_tab[i].bits.pretendent == no){
            memmove(&person_tab[i], &person_tab[i + 1], (n - i)*sizeof(Person));
            i--;
            n--;
        }
    }

    return n;
}

void print_person(const Person *p) {
    printf("%s\n", p->name);
}

void print_persons(const Person *person_tab, int n) {
    for(int i=1; i<=n; ++i, ++person_tab) printf("%2d %12s %s\n", i, person_tab->name, person_tab->parent);
}

int compare(const void* _a, const void* _b){
    Person a = *((Person*)_a);
    Person b = *((Person*)_b);

    if(a.parent == NULL)
        return -1;
    if(b.parent == NULL)
        return 1;

    int parent = strcmp(a.parent, b.parent);

    if(parent < 0)
        return -1;
    else if(parent > 0)
        return 1;
    else {
        int born_date = cmp_date(&a.born, &b.born);

        if(cmp_date(&a.born, &primo_date) < 0 && cmp_date(&b.born, &primo_date) < 0) {
            if (a.bits.sex == M && b.bits.sex == F)
                return -1;
            else if (a.bits.sex == F && b.bits.sex == M)
                return 1;
        }

        if(born_date < 0)
            return -1;
        else if(born_date > 0)
            return 1;
        else {
            if (a.bits.sex > b.bits.sex)
                return 1;
            else if (a.bits.sex < b.bits.sex)
                return -1;
            else
                return 0;
        }
    }
}

int create_list(Person *person_tab, int n) {
    qsort(person_tab, n, sizeof(Person), compare);

    Parent parents_tab[33];

    int no_parents = fill_indices_tab(parents_tab, person_tab, n);

    persons_shiftings(person_tab, n, parents_tab, no_parents);

    return cleaning(person_tab, n);
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1926}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"},
		{"Ernest", {M,yes}, {30, 5, 2023}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[RECORD_MAX];
	fgets(buff, RECORD_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, RECORD_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, RECORD_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d", &no);
			const int no_persons = sizeof(person_tab) / sizeof(Person);
			create_list(person_tab,no_persons);
			print_person(person_tab + no - 1);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}

