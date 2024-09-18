#include <stdio.h>
#include <string.h>

int main(void)
{
    int k=5,len=0;
    printf("Podaj dlugosc zdania do zaszyfrowania: ");
    scanf("%d",&len);
    getchar();
    char str[len+1];
    printf("Podaj zdanie do zaszyfrowania: ");
    fgets(str,sizeof(str),stdin);
    for(int i=0;str[i]!='\0';i++)
    {
        int dig=int(str[i]);
        if ((dig>=65)&&(dig<=90))
        {
            if (dig+k<=90)
            {
                str[i]=char(dig+k);
            }
            else
            {
                str[i]=char(65+k-1);
            }
        }
        if ((dig>=97)&&(dig<=122))
        {
            if (dig+k<=122)
            {
                str[i]=char(dig+k);
            }
            else
            {
                str[i]=char(97+k-1);
            }
        }
    }
    printf("%s",str);
    




    return 0;
}

