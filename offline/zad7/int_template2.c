#include <stdio.h>
#include <math.h>

#define RECURS_LEVEL_MAX  10
#define N_MAX             10

// pointer to function of one variable
typedef double (*Func1vFp)(double);

// example functions of one variable
double f_poly(double x) {
    return 2*pow(x,5)-4*pow(x,4)+3.5*pow(x,2)+1.35*x-6.25;// polynomial a[0] + a[1]x + ... + a[n]x^n
}

double f_rat(double x) {
    return 1/(pow(x-0.5,2)+0.01);
}

double f_exp(double x) {
    return 2*x*exp(-1.5*x)-1;
}

double f_trig(double x) {
    return x* tan(x);
}

// Quadratures

// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) {
    double quad=0.0;
    double h=(b-a)/(double)n;
    for(int i=0;i<n;i++){
        quad+=f1(a+i*h);
    }
    return quad*h;
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
    double quad=0.0;
    double h=(b-a)/(double)n;
    for(int i=0;i<n;i++){
        quad+=f1(a+i*h+h);
    }
    return quad*h;
}

// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
    double quad=0.0;
    double h=(b-a)/(double)n;
    for(int i=0;i<n;i++){
        quad+=f1(a+i*h+(h/2.0));
    }
    return quad*h;
}

// trapezoidal rule
double quad_trap(Func1vFp func, double a, double b, int n) {
    double quad=0.0;
    double h=(b-a)/(double)n;
    for(int i=0;i<n;i++){
        quad+=0.5*(func(a+i*h)+func(a+i*h+h));/// you need to pay attention
    }
    return quad*h;
}

// Simpson's rule
double quad_simpson(Func1vFp func, double a, double b, int n) {
    double quad=0.0;
    double h=(b-a)/(double)n;
    for(int i=0;i<n;i++){
        quad+=(func(a+i*h)+4*func(a+i*h+ h/2 )+func(a+i*h+h))/6;
    }
    return quad*h;
}

// pointer to quadrature function
typedef double (*QuadratureFp)(Func1vFp,double,double,int) ;

// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
	quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
    return  (quad_tab[quad_no])((func_tab[fun_no]),a,b,n);
}

// adaptive algorithm
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
    double S1 = quad(f,a,(b+a)/2.0,1);
    double S2= quad(f,(b+a)/2.0,b,1);
    if(fabs((S1+S2)-S)<=delta)
        return S1+S2;
    if(level>RECURS_LEVEL_MAX)
        return NAN;
    return recurs(f,a,(a+b)/2.0,S1,delta/2.0,quad,level+1)+ recurs(f,(a+b)/2.0,b,S2,delta/2.0,quad,level+1);
}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
    double curr_S = quad(f, a, b, 1);
    return recurs(f, a, b, curr_S, delta, quad, 1);
}
// double integrals

// pointer to function of two variables
typedef double (*Func2vFp)(double,double);

double func2v_2(double x, double y) {
	return 2 - x*x - y*y*y;
}

// sample functions to define the normal domain

double lower_bound2(double x) {
	return 0.7*exp(-2*x*x);
}
double upper_bound2(double x) {
	return sin(10*x);
}

// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {\
    double quad=0.0;
    double h1=(x2-x1)/(double)nx;
    double h2=(y2-y1)/(double)ny;
    for(int i=0;i<nx;i++ ){
        for(int j=0;j<ny;j++){
            quad+=f(x1+i*h1,y1+j*h2);
        }
    }
    return quad*h1*h2;
}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy,
						   Func1vFp fg, Func1vFp fh) {
    double quad=0.0;
    double h1=(x2-x1)/(double)nx;
    for(int i=0;i<nx;i++ ){
        double quad2=0.0;
        double xi=x1+h1/2.0+i*h1;
        double y1=fg(xi);
        int ny=ceil((fh(xi)-fg(xi))/hy);
        double h2 = (fh(xi)-fg(xi))/ny;
        for(int j=0;j<ny;j++){
            quad2+=f(xi,y1+h2/2.0+j*h2);
        }
        quad+=quad2*h2;
    }
    return quad*h1;
}

// rectangle rule (leftpoint) - double integral over multiple normal
// domains with respect to the x-axis
double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2,
		int ny, Func1vFp fg, Func1vFp fh)  {
 double result = 0, start, end, dx = (x2 - x1) / nx, dy = (y2 - y1) / ny;

	double x = x1, y, hy;
	for (int i = 0; i < nx; i++) {
		start = fmax(y1, fg(x));
		end = fmin(y2, fh(x));

		if (start < end) {
			ny = ceil((end - start) / dy);
			hy = (end - start) / ny;
			y = start;
			for (int j = 0; j < ny; j++) {
				result += f(x, y) * hy;
				y += hy;
			}
		}

		x += dx;
	}

	return result * dx;

}

// multiple quadratures

typedef double (FuncNvFp)(const double, int);
typedef int (BoundNvFp)(const double, int);

// sample function of three variables
double func3v(const double v[], int n) {
	return v[0] - v[1] + 2*v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
	return v[0] > 0 && v[0] < 0.5 && v[1]*v[1] + (v[2]-1)*(v[2]-1) < 1;
}

// multiple integrals over a cuboid with predicate (if boundary != NULL)
// rectangular rule (rightpoint)
double trpl_quad_rect(FuncNvFp f, double variable_lim[][2], const int tn[], BoundNvFp boundary) {
    double dx = (variable_lim[0][1] - variable_lim[0][0])/tn[0];
    double dy = (variable_lim[1][1] - variable_lim[1][0])/tn[1];
    double dz = (variable_lim[2][1] - variable_lim[2][0])/tn[2];

    double integral = 0;

    double xi = variable_lim[0][0];
    for(int x = 0; x < tn[0]; x++){
        xi += dx;
        double yi = variable_lim[1][0];

        for(int y = 0; y < tn[1]; y++){
            yi += dy;
            double zi = variable_lim[2][0];
            for(int z = 0; z < tn[2]; z++){
                zi += dz;

                double v[3] =  {xi, yi, zi};

                if((boundary != NULL && boundary(v, 3) != 0 ) || boundary == NULL)
                    integral += dx*dy*dz*f(v, 3);
            }
        }
    }

    return integral;
}

int main(void) {
int to_do, n, nx, ny, integrand_fun_no, method_no, n_tests;
	int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
	int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
	double a, b, x1, x2, y1, y2, hy, delta;
//	double t_variable[N_MAX], variable_lim[N_MAX][2];
	double variable_lim[N_MAX][2];
	int tn[N_MAX];

	scanf("%d", &to_do);
	switch (to_do) {
		case 1: // loop over quadratures and integrands
			scanf("%lf %lf %d", &a, &b, &n);
			for(int q = 0; q < no_quads; ++q) {
				for(int f = 0; f < no_funcs; ++f) {
					printf("%.5f ",quad_select(f, q, a, b, n));
				}
				printf("\n");
			}
			break;
		case 2: // adaptive algorithm
			scanf("%d",&n_tests);
			for (int i = 0; i < n_tests; ++i) {
				scanf("%d %d", &integrand_fun_no, &method_no);
				scanf("%lf %lf %lf", &a, &b, &delta);
				printf("%.5f\n", init_recurs(func_tab[integrand_fun_no], a, b, delta, quad_tab[method_no]));
			}
			break;
		case 3: // double integral over a rectangle
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
			break;
		case 4: // double integral over normal domain
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf", &hy);
			printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
			break;
		case 5: // double integral over multiple normal domains
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
			break;
		case 6: // triple integral over a cuboid
			scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
			scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn+1);
			scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn+2);
//			scanf("%d", &flag);
			printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, NULL));
			printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, bound3v));
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
	return 0;
}