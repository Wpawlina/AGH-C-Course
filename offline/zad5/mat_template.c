#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
	for(int i=0;i<n;i++)
	{
		for(int w=0;w<m;w++)
		{
			for(int k=0;k<p;k++)
			{
				AB[w][i]+=A[w][k]*B[k][i];
			}
		}
	}
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
	double det=1.0;
	for(int i=0;i<n;i++)
	{
		if(A[i][i]==0.0)
		{
			return NAN;
		}
		det*=A[i][i];
		for(int j=i+1;j<n;j++)
		{
			double wsp=A[j][i]/A[i][i];
			A[j][i]=0.0;
			for(int k=i+1;k<n;k++)
			{
				A[j][k]-=wsp*A[i][k];
			}
		}

	}
	return det;
}
double gauss_simplified2(double A[][SIZE], double b[], size_t n, double eps) {
    double det=1.0,el,prop;
    for (size_t i=0; i<n; i++){
        el=A[i][i];
        if (fabs(el)<eps) return 0.0;
        det*=el;
        for (size_t j=i+1; j<n; j++){
            prop=A[j][i]/el;
            A[j][i]=0.0;
            for (size_t k=i+1; k<n; k++)
                A[j][k]-=prop*A[i][k];
            b[j]-=prop*b[i];
        }
    }
    return det;
}


void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.


void set_perm(const double A[][SIZE], int perm[], size_t n){
    int temp;
    for (size_t i=0; i<n; i++)
        for (size_t j=0; j<n-i-1; j++)
            if (fabs(A[perm[j]][perm[j]])>fabs(A[perm[j+1]][perm[j+1]])){
                temp=perm[j];
                perm[j]=perm[j+1];
                perm[j+1]=temp;
            }
}

double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps) {
    int temp,perm[SIZE];
    for (size_t i=0; i<n; i++) perm[i]=i;
    set_perm(A,perm,n);
    double B[SIZE][SIZE];
    double c[SIZE];
    temp=0;
    for (size_t i=0; i<n; i++){
        for (size_t j=0; j<n; j++)
            B[i][j]=A[perm[i]][j];
        c[i]=b[perm[i]];
        temp+=abs(perm[i]-i);
    }
    double suma=0.0,det=pow(-1,temp/2)*gauss_simplified2(B,c,n,eps);
    if (det==0.0) return 0.0;
    for (int i=n-1; i>=0; i--){
        for (size_t j=0; j<n-1-i; j++)
            suma-=B[i][n-j-1]*x[n-j-1];
        suma+=c[i];
        x[i]=suma/B[i][i];
        suma=0.0;
    }
    return (-1)*det;
}


double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps) {
    int temp,perm[SIZE];
    for (size_t i=0; i<n; i++) perm[i]=i;
    set_perm(A,perm,n);
    double C[SIZE][SIZE];
    double D[SIZE][SIZE];
    for (size_t i=0; i<n; i++){
        for (size_t j=0; j<n; j++)
            B[i][j]=0.0;
        B[i][i]=1.0;
    }
    temp=0;
    for (size_t i=0; i<n; i++){
        for (size_t j=0; j<n; j++){
            C[i][j]=A[perm[i]][j];
            D[i][j]=B[perm[i]][j];
        }
        temp+=abs(perm[i]-i);
    }
    double det=pow(-1,temp/2)*1.0,el1,el2,prop;
    for (size_t i=0; i<n; i++){
        el1=C[i][i];
        el2=D[i][i];
        if (fabs(el1)<eps) return 0.0;
        det*=el1;
        for (size_t j=i+1; j<n; j++){
            prop=C[j][i]/el1;
            C[j][i]=0.0;
            for (size_t k=i+1; k<n; k++)
                C[j][k]-=prop*C[i][k];
            for (size_t k=0; k<n; k++)
                D[j][k]-=prop*D[i][k];
        }
    }
    for (size_t i=0; i<n; i++){
        prop=C[i][i];
        for (size_t j=0; j<n; j++){
            C[i][j]/=prop;
            D[i][j]/=prop;
        }
    }
    det*=prop/fabs(prop);
    for (int i=n-1; i>=0; i--){
        el1=C[i][i];
        el2=D[i][i];
        for (int j=i-1; j>=0; j--){
            prop=C[j][i]/el1;
            C[j][i]=0.0;
            for (int k=i-1; k>=0; k--)
                C[j][k]-=prop*C[i][k];
            for (int k=n-1; k>=0; k--)
                D[j][k]-=prop*D[i][k];
        }
    }
    for (size_t i=0; i<n; i++)
        for (size_t j=0; j<n; j++)
            B[i][j]=D[i][j];
    return det;
}




// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.




// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.


int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

