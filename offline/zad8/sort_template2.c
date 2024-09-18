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
    Date * d11=(Date*)d1;
    Date * d22=(Date*)d2;
    if( d11->year<d22->year)
        return -1;
    if( d11->year>d22->year)
        return 1;
    if( d11->month<d22->month)
        return -1;
    if( d11->month>d22->month)
        return 1;
    if( d11->day<d22->day)
        return -1;
    if( d11->day>d22->day)
        return 1;
    return 0;
}
//Pomocnicze funkcja do printowania podczas debugowania
void printF(Food f){
    printf("%s %.2f %d %02d.%02d.%d\n",f.name,f.price,f.amount
            ,f.valid_date.day,f.valid_date.month,f.valid_date.year);
}
//Pomocnicze funkcja do printowania podczas debugowania
void printFT(Food* f,int n){
    for(int i=0;i<n;i++)
        printf("%s+%.2f+%d+%02d.%02d.%d+|+",f[i].name,f[i].price,f[i].amount
                ,f[i].valid_date.day,f[i].valid_date.month,f[i].valid_date.year);
}
int cmp(const void *a, const void *b) {
    Food * a1=(Food*)a;
    Food * b1=(Food*)b;
    int r;
    if((r=strcmp(a1->name,b1->name))!=0){
        return r;
    }
    if(a1->price>b1->price)
        return 1;
    if(a1->price<b1->price)
        return -1;
    return cmp_date(&(a1->valid_date),&(b1->valid_date));
}

void* bsearch2 (const void *key, const void *base, size_t nitems,
                int(compar)(const void, const void*), char *result){
    int l=0,r=nitems;
    Food tab=(Food)base;
    while(l<r){
        int mid=(l+r)/2;
        if(compar(&tab[mid],(Food*)key)>=0)
            r=mid;
        else
            l=mid+1;
    }
    if((-1<l<nitems && compar(&tab[l],key)==0))
        *result=-1;
    else
        *result=l;
    return tab+l;
}

void print_art(Food *p, const int n, const char *art) {
    //printf("%d",n);
    for(int i=0;i<n;i++){
        if(strcmp(p[i].name,art)==0){
            printf("%.2f %d %02d.%02d.%d\n",p[i].price,p[i].amount
                    ,p[i].valid_date.day,p[i].valid_date.month,p[i].valid_date.year);
        }
    }
}

Food* add_record(Food *tab, int *np, const ComparFp compar, const Food *new) {

    char result=0;
    if(*np==0){
        tab[0]=*new;
        *np=*np+1;
        return tab;
    }
    Food * r=bsearch2(new,tab,*np,compar,&result);
    if(result==-1){
        r->amount=r->amount+new->amount;
    }else{
        *np=*np+1;
        Food temp=*r;
        *r=*new;
        r=r+1;
        Food temp2=temp;
        result=result+1;

        while((int)result<*np){
            temp=*r;
            *r=temp2;
            r=r+1;
            temp2=temp;
            result=result+1;
        }
    }
    return r;
}

int read_goods(Food *tab, const int no, FILE *stream, const int sorted) {
    int n=0;
    for(int i =0;i<no;i++){
        Food td;
        scanf("%s %f %d %d.%d.%d.",&td.name,&td.price
                ,&td.amount,&td.valid_date.day,&td.valid_date.month,&td.valid_date.year);
        add_record(tab,&n,cmp,&td);
        //     printf("----------------------------\n");
        //     printF(td);
        //     printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        //     printFT(tab,n);
        //     printf("----------------------------\n");
    }
    return n;
}

int cmp_qs(const void *a, const void *b) {
    const Food fa = (Food)a, fb = (Food)b;
    return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
    const Date pd = (Date)a;
    const Food fb = (Food)b;
    return cmp_date(pd, &fb->valid_date);
}

float value(Food *food_tab, const size_t n, const Date curr_date, const int days) {
    qsort(food_tab,n,sizeof(Food),cmp_qs);

    struct tm tm_find={.tm_year=curr_date.year-1900,.tm_mon=curr_date.month-1,.tm_mday=curr_date.day};;
    tm_find.tm_mday+=days;
    mktime(&tm_find);
    Date to_find_date={.year=tm_find.tm_year+1900,.month=tm_find.tm_mon+1,.day=tm_find.tm_mday};
    int l=0,r=n-1;
    while(l<r){
        int mid=(l+r)/2;
        if(cmp_bs(&to_find_date,&food_tab[mid])<=0)
            r=mid;
        else
            l=mid+1;
    }
    float val=0.0f;
    while(cmp_date(&(food_tab[l].valid_date),&to_find_date)==0){
        val+=food_tab[l].price*food_tab[l].amount;
        l++;
    }
    return val;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
    enum Sex sex:1;
    enum BOOL in_line:1;
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
    char *D;
    int index;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(const Person *person1, const Person *person2) {
    if (person1->bits.sex == person2->bits.sex) return 0;
    if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
    if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
    return person2->bits.sex - person1->bits.sex;
}
int cmp_parent(const void *a, const void *b) {
    Person aa=(Person)a;
    Person bb=(Person)b;
    if(aa->parent==NULL)
        return -1;
    if(bb->parent==NULL)
        return 1;
    return strcmp(aa->parent,bb->parent);
}
int comp_birth(const void *a, const void *b) {
    Person aa=(Person)a;
    Person bb=(Person)b;
    return cmp_date(&(aa->born),&(bb->born));
}
int cmp_person(const void *a, const void *b) {
    int ret=cmp_parent(a,b);
    if (ret==0){
        ret=cmp_primo(a,b);
        if(ret==0)
            ret=comp_birth(a,b);
    }
    return ret;
}

int fill_indices_tab(Parent *idx_tab, const Person *pers_tab, const int size) {
    for(int i=0;i<size;i++){
        //dla kazdej osoby wyszukujemy binary searchem najstrasze dziecko
        int l=1,r=size;
        while(l<r){
            int mid=(l+r)/2;
            if(strcmp(pers_tab[i].name,pers_tab[mid].parent)<=0)
                r=mid;
            else
                l=mid+1;
        }
        idx_tab[i].D=pers_tab[i].parent;
        //Sprawdzamy czy osoba ma dziecko jesli nies ustawiamy na -1, jesli tak dajemy indeks najsatrszego dziecka
        if(pers_tab[l].parent!=pers_tab[i].name)
            idx_tab[i].index=-1;
        else
            idx_tab[i].index=l;
    }

}
//Pomocnicza struktura
typedef struct {
    Person person;
    int index;
} PersonInd;
void persons_shifts(Person *person_tab, const int size, Parent *idx_tab, const int no_parents) {
    PersonInd temp_person_tab[size+1];
    temp_person_tab[0].person=person_tab[0];
    temp_person_tab[0].index=0;
    int curr_size=1;//ilosc osob w posortowanej lini sukcesyjnej
    for(int i=0;i<size;i++) {
        //Mamy rodzica
        //szukamy indeks najstarszeszego dziecka
        int j = idx_tab[temp_person_tab[i].index].index;//znajdujemy indeks najstarszego dziecka osoby ktora jest juz umieszczona w lini sukcesyjnej
        if(j==-1)//jesli nie ma rodzica
            continue;
        //Kopiujemy osoby po rodzicu 
        PersonInd temp[size+1];
        int temp_size=curr_size-i-1;
        if(temp_size!=0)
            memcpy(temp,temp_person_tab+i+1,temp_size*sizeof(PersonInd));
        //zalatwaimy najstarsze dziecko
        int ii=i;
        curr_size++;
        ii++;
        temp_person_tab[ii].person = person_tab[j];//wstawiamy najstarsze dziecko do lini sukcesyjnej
        temp_person_tab[ii].index = j;//wstawiamy indeks z oryginalnej tablicy
        //dodajemy reszte dzieci
        while (j+1< size && strcmp(person_tab[j + 1].parent, temp_person_tab[i].person.name) == 0){//dopoki te dzieci maja parenta wspolnego
            temp_person_tab[++ii].person = person_tab[++j];
            temp_person_tab[ii].index = j;
            curr_size++;
        }
        //dodajemy reszte skopowianej rodziny po rodzicu
        if(temp_size!=0)
            memmove(temp_person_tab+ii+1,temp,temp_size*sizeof(PersonInd));
    }
    for(int i=0;i<size;i++){
        person_tab[i]=temp_person_tab[i].person;
    }
    
}


void print_person(const Person *p) {
    printf("%s", p->name);
}
int cleaning(Person *person_tab, int n) {
    for(int i=0;i<n;i++) {
        if(person_tab[i].bits.in_line==no){
            memmove(person_tab+i,person_tab+i+1,(n-i-1)*sizeof(Person));
            person_tab[n-1]=(const Person){ 0 };//initalizing with deafult valeus
            i--;
            n--;
        }
    }
}

int create_list(Person *person_tab, int n) {
    //1 sort by parent name,sex and birth
    qsort(person_tab,n,sizeof(Person),cmp_person);
    //2 filling indicing
    Parent idx_tab[n+1];
    fill_indices_tab(idx_tab,person_tab,n);
    //3 shifting to right place
    persons_shifts(person_tab,n,idx_tab,0);
    //4 cleaning not in succesion line people
    cleaning(person_tab,n);

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