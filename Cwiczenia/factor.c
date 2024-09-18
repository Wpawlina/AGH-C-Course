#include <stdio.h>


int main()
{
    int n=0,sum=1;
    printf("Enter n:");
    scanf("%d", &n);
    for(int i=1; i <= n ; i++)
    {
        sum=sum*i;
    }
    printf("%d! wynosi %d\n",n,sum);

}

