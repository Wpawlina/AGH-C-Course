#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) { 
	if(n==MAX_REL_SIZE)
	{
		return n;
	}
	for(int i=0;i<n;i++)
	{
		if(tab[i].first==new_pair.first && tab[i].second==new_pair.second)
		{
			return n;
		}
	}
	tab[n]=new_pair;
	return n+1;
	

}

 int cmp_pair(const void* p1,const void* p2){
    return *(int*)p1-*(int*)p2;
}

int insert_int (int *tab, int n, int new_element) {
    int check=1;
    for (int i=0; i<n; i++){
        if (tab[i]==new_element){
            check=0;
            break;
        }
    }
    return check;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) { 
	int n;
	scanf("%d",&n);
	int a,b;
	for(int i=0;i<n;i++)
	{
		scanf("%d",&a);
		scanf("%d",&b);
		add_relation(relation,n,(pair){a,b});
	}
	return n;

}

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

//------------------------------------------------

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *tab, int n) {
	int i,j,elem,check;
    for (i=0; i<n; i++){
        elem=tab[i].first;
        check=0;
        for (j=0; j<n; j++){
            if (tab[j].second==elem){
                check=1;
                break;
            }
        }
        if (!check) return 0;
    }
    return 1;


}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *tab, int n) {
	return 1-is_reflexive(tab,n);
 }

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *tab, int n) { 
	int i,j,fir,sec,check;
    for (i=0; i<n; i++){
        fir=tab[i].first;
        sec=tab[i].second;
        check=0;
        for (j=0; j<n; j++){
            if (tab[j].first==sec && tab[j].second==fir){
                check=1;
                break;
            }
        }
        if (!check) return 0;
    }
    return 1;


}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *tab, int n) {
	int i,j,fir,sec,check;
    for (i=0; i<n; i++){
        fir=tab[i].first;
        sec=tab[i].second;
        check=1;
        for (j=0; j<n; j++){
            if (tab[j].first==sec && tab[j].second==fir){
                if (fir==sec) check=1;
                else check=0;
                break;
            }
        }
        if (!check) return 0;
    }
    return 1;
 }

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *tab, int n) { 
	int check1=is_antisymmetric(tab,n);
    int check2=is_irreflexive(tab,n);
    return check1*check2;

}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *tab, int n) { 
	int i,j,k,fir,sec,third,check;
    for (i=0; i<n; i++){
        fir=tab[i].first;
        sec=tab[i].second;
        check=1;
        for (j=0; j<n; j++){
            if (tab[j].first==sec){
                third=tab[j].second;
                check=0;
                for (k=0; k<n; k++){
                    if (tab[k].first==fir && tab[k].second==third){
                        check=1;
                        break;
                    }
                }
                break;
            }
        }
        if (!check) return 0;
    }
    return 1;




}

//------------------------------------------------

// Case 2:


int get_domain(pair *tab, int n, int *domain) {
	int elem1,elem2,i,cnt=0;
    for (i=0; i<n; i++){
        elem1=tab[i].first;
        elem2=tab[i].second;
        if (insert_int(domain,cnt,elem1))
            domain[cnt++]=elem1;
        if (!(elem1==elem2))
            if (insert_int(domain,cnt,elem2))
                domain[cnt++]=elem2;
    }
    qsort(domain,cnt,sizeof(int),cmp_pair);
    return cnt;
 }

 int is_connected(pair *tab, int n) {
	int i,j,k,check,elem1,elem2,tmp_domain[n];
    int cnt=get_domain(tab,n,tmp_domain);
    for (i=0; i<cnt; i++){
        for (j=0; j<cnt; j++){
            elem1=tmp_domain[i];
            elem2=tmp_domain[j];
            check=0;
            for (k=0; k<n; k++){
                if ((elem1==tab[k].first && elem2==tab[k].second) || (elem1==tab[k].second && elem2==tab[k].first)){
                    check=1;
                    break;
                }
            }
            if (!check)
                return 0;
        }
    }
    return 1;
 }


// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *tab, int n) { 
	return is_reflexive(tab,n)*is_antisymmetric(tab,n)*is_transitive(tab,n);
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair *tab, int n) { 
return is_partial_order(tab,n)*is_connected(tab,n);
}

int find_max_elements(pair *tab, int n, int *max_elements) { // tab - partial order
 int i,j,elem,check,cnt=0;
    for (i=0; i<n; i++){
        elem=tab[i].second;
        check=1;
        for (j=0; j<n; j++){
            if (tab[j].first==elem){
                if (tab[j].second!=elem){
                    check=0;
                    break;
                }
            }
        }
        if (check){
            if (insert_int(max_elements,cnt,elem))
                max_elements[cnt++]=elem;
        }
    }
    qsort(max_elements,cnt,sizeof(int),cmp_pair);
    return cnt;
 
}

int find_min_elements(pair *tab, int n, int *min_elements) { // tab - strong partial order
int i,j,elem,check,cnt=0;
    for (i=0; i<n; i++){
        elem=tab[i].first;
        check=1;
        for (j=0; j<n; j++){
            if (tab[j].second==elem){
                if (tab[j].first!=elem){
                    check=0;
                    break;
                }
            }
        }
        if (check){
            if (insert_int(min_elements,cnt,elem))
                min_elements[cnt++]=elem;
        }
    }
    qsort(min_elements,cnt,sizeof(int),cmp_pair);
    return cnt;
}





// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)







//------------------------------------------------

// Case 3:

// x(S o R)z iff exists y: xRy and ySz
int composition (pair *rel_1, int n1, pair *rel_2, int n2, pair *rel_3) { }

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	
	return 0;
}

