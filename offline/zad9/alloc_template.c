#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

// 1
////////////////////////////////////////////////////////////

int get(int cols, int row, int col, const int* A) {
	return A[row*cols+col];
}

void set(int cols, int row, int col, int* A, int value) {
	A[row*cols+col]=value;
}


void prod_mat(int rowsA, int colsA, int colsB, int* A, int* B, int* AB) {
	for(int i=0;i<rowsA;i++)
	{
		for(int j=0;j<colsB;j++)
		{
			int sum=0;
			for(int k=0;k<colsA;k++)
			{
				sum+=get(colsA,i,k,A)*get(colsB,k,j,B);

			}
			set(colsB,i,j,AB,sum);
		}
	}

}

void read_mat(int rows, int cols, int* t) {
	 for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            scanf("%i", &t[i*cols + j]);
        }
    }
}

void print_mat(int rows, int cols, int* t) {
	 for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            printf("%i ", t[i*cols + j]);
        }
        printf("\n");
    }
}

// 2
/////////////////////////////////////////////////////////////

int read_ints_from_line(char* c_buf, int *int_buf) {
	char c=fgetc(stdin);
	c_buf=malloc(BUF_SIZE*sizeof(char));
	int_buf=malloc(TAB_SIZE*sizeof(int));
	int sizeInt=0;
	int cbufIndex=0;
	while (!feof(stdin)&& c!='\n')
	{
		if(c==' ')
		{
			int num=atoi(c_buf);

			memset(c_buf, 0, BUF_SIZE);
			int_buf[sizeInt]=num;
			cbufIndex=0;
			sizeInt++;
		}
		else
		{
			c_buf[cbufIndex]=c;
			cbufIndex++;
		}
		c=fgetc(stdin);
		
	}
	int num=atoi(c_buf);
	free(c_buf);
	int_buf[sizeInt]=num;
	sizeInt++;
	return sizeInt;
}

int read_int_lines_cont(int* ptr_array[]) {
	int linesIndex=0;
	int* intTab=NULL;
	char* line=NULL;
	while (!feof(stdin))
	{
		int size=read_ints_from_line(line,intTab);
		ptr_array[linesIndex]=intTab;
		intTab=NULL;
		free(line);
		linesIndex++;	
	}
	ptr_array[linesIndex]=NULL;
	return linesIndex;
	




}

void write_int_line_cont(int *ptr_array[], int n) {
	int* temp_ptr = ptr_array[n];

    while(temp_ptr != ptr_array[n + 1]){
        printf("%i ", *temp_ptr);
        temp_ptr++;
    }
	
}

// 3
///////////////////////////////////////////////////////////

int read_char_lines(char *array[]) {
	char* buff=malloc(BUF_SIZE*sizeof(char));
	int linesIndex=0;
	while (!feof(stdin))
	{
		fgets(buff,BUF_SIZE,stdin);
		int length=strlen(buff);
		array[linesIndex]=malloc((length+1)*sizeof(char));
		strcpy(array[linesIndex],buff);
		linesIndex++;
	}
	array[linesIndex]=NULL;
	return linesIndex;

	
}

void write_char_line(char *array[], int n) {
	 printf("%s \n", array[n]);
}

void delete_lines(char *array[]) {
	int index = 0;

    while(array[index] != NULL)
        free(array[index++]);
}

// 4
////////////////////////////////////////////////

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
	int linesIndex=0;
	int* intTab=NULL;
	char* line=NULL;
	while (!feof(stdin))
	{
		printf("check 1 \n");
		int size=read_ints_from_line(line,intTab);
		printf("%d \n",size);
		
		printf("check 3 \n");
		lines_array[linesIndex].len=size;
		lines_array[linesIndex].values=(int*)malloc(size*sizeof(int));
		printf("check 4 \n");

		double avg=0;
		for(int i=0;i<size;i++)
		{
			printf("%d %d \n",i,size,intTab[i] );
			avg+=intTab[i];
			lines_array[linesIndex].values[i] = intTab[i];
		}
		avg=avg/size;
		lines_array[linesIndex].average=avg;
		printf("%d %d \n",lines_array[linesIndex].len,lines_array[linesIndex].average);
		linesIndex++;
		free(line);
		free(intTab);
		line=NULL;
		intTab=NULL;
	}
	return linesIndex;
}

void write_int_line(line_type lines_array[], int n) {
	 for(int i = 0; i < lines_array[n].len; i++){
        printf("%i ", lines_array[n].values[i]);
    }

}

void delete_int_lines(line_type array[], int line_count) {
	for(int i = 0; i < line_count; i++)
        free(array[i].values);

}

int cmp (const void *a, const void *b) {
	line_type a1=*((line_type*)a);
	line_type b1=*((line_type*)b);
	if(a1.average>b1.average)
	{
		return 1;
	}
	else if(b1.average>a1.average)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

void sort_by_average(line_type lines_array[], int line_count) {
	qsort(lines_array,line_count,sizeof(line_type),cmp);
}

// 5
///////////////////////////////////////////////////////////

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet* triplet_array, int n_triplets) {
	for(int i = 0; i < n_triplets; i++){
        scanf("%i %i %i", &triplet_array[i].r, &triplet_array[i].c, &triplet_array[i].v);
    }

    return n_triplets;
}

int cmp_triplets(const void* t1, const void* t2) {
	triplet a = *((triplet*)t1);
    triplet b = *((triplet*)t2);

    if(a.r < b.r)
        return -1;
    if(a.r > b.r)
        return 1;
    if(a.c < b.c)
        return -1;
    if(a.c > b.c)
        return 1;

    return 0;

}

void make_CSR(triplet* triplet_array, int n_triplets, int rows, int* V, int* C, int* R) {
	qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);

    memset(R, 0, (rows+1)*sizeof(int));
    for(int i = 0; i < n_triplets; i++){
        V[i] = triplet_array[i].v;
        C[i] = triplet_array[i].c;
        for(int k = triplet_array[i].r + 1; k < rows + 1; k++)
            R[k] += 1;
    }
	
}

void multiply_by_vector(int rows, const int* V, const int* C, const int* R,const int* x, int* y) 
{
	for(int i = 0; i < rows; i++){
        y[i] = 0;
        for(int j = R[i]; j <= R[i+1] - 1; j++){
            y[i] += V[j]*x[C[j]];
        }
    }
}


void read_vector(int* v, int n) {
	 for(int i = 0; i < n; i++){
        scanf("%i", &v[i]);
    }
}

void write_vector(int* v, int n) {
	 for(int i = 0; i < n; i++)
        printf("%i ", v[i]);

    printf("\n");

}

// auxiliary
////////////////////////////////////////////////////////////

int read_int(void) {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char* char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int* ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

