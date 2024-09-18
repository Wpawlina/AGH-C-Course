
#include <stdio.h>
int main()
{
    int n=0,a=0,b=0;
    printf("Enter n: ");
    scanf("%d", &n);
    a=0;
    b=1;

    while (a <= n)
    {
        if(a*b==n)
        {
           printf("TRUE");
           return 0;
        }
        int bufor;
        bufor=a;
        a=b;
        b=bufor+b;
    }
    printf("FALSE");
    return 0;
}