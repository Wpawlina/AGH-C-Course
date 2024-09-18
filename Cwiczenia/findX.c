#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main(void)
{
    srand(time(NULL));
    int X=rand()%101;
    int proba=1;
    int n=-1;
    int L=0,P=100;
    while(1==1)
    {
        n=L+rand()%(P+1-L);
        if (n==X)
        {
            printf("Znaleziono X=%d za %d proba!!!",n,proba);
            break;
        }
        else if(n>X)
        {
            P=n-1;
        }
        else
        {
            L=n+1;
        }
        proba++;

    }
    return 0;



}