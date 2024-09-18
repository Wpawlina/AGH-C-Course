#include <stdio.h>
int desc(int n)
{
    int x=n,prevdig=10,dig=-1;
    while(x!=0)
    {
        dig=x%10;
        if(dig > prevdig)
        {
            return 0;
        }
        prevdig=dig;
        x=x/10;

    }
    return 1;
}

int main(void)
{
    printf("%d",desc(145678));

    return 0;
}