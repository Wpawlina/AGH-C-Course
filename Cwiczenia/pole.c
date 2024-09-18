#include <stdio.h>
#include <math.h>

int main(void)
{
    double a=0,b=10*3.14,N=99999999;
    double deltax=(b-a)/N;
    double i=a;
    double area=0;
    while(i<b)
    {
        double ci=(i+i+deltax)/2;
        double y=sin(ci);
        area=area+deltax*y;
        i+=deltax;
    }
    printf("Pole pod funkcja sinx na przedzila [%f,%f] wynosi %f",a,b,area);





    return 0;
}