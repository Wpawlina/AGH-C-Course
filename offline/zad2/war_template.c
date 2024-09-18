#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NUMBER_OF_CARDS 52
#define NUMBER_OF_COLORS 4


int cbuff_push(int cbuff[],int cli_nr,int *out,int *len) { // client with number cli_nr enters the queue
	
	int position=(*out+*len)% NUMBER_OF_CARDS;
	cbuff[position]=cli_nr;
	(*len)++;
}

int cbuff_pop(int cbuff[],int *len,int *out) { // longest waiting client leaves the queue
	(*len)--;
	int temp=*out;
	*out=(*out+1)% NUMBER_OF_CARDS;
	return cbuff[temp];
}
void cbuff_print(int cbuff[],int *len,int *out) {
	int i=*out;
	for(int j=0;j<*len;j++)
	{
		printf("%d ",cbuff[i]);
		i=(i+1)%NUMBER_OF_CARDS;
	}
}





int random_from_interval(int a, int b) {
	if (a>b)
	{
		return INT_MIN;
	}
	else if((b-a)>RAND_MAX)
	{
		return INT_MAX;
	}
	else if(a==b)
	{
		return a;
	}
	else
	{
		return a+rand()%(b-a+1);
	}
}
void swap (int array[], int i, int k) {
	int temp=array[i];
	array[i]=array[k];
	array[k]=temp;
}

void shuffle(int t[], int size) {
	if(size>=0)
	{
		for(int i=0;i<size;i++)
		{
			t[i]=i;
		}
		for(int i=0;i<size-1;i++)
		{
			int k=random_from_interval(i,size-1);
			swap(t,i,k);
		}
	}
}
void game(int n, int player_A[], int player_B[], int max_conflicts, int simplified_mode) {
	int cur_conflicts=0;
	int pout_A[NUMBER_OF_CARDS];
	int pout_B[NUMBER_OF_CARDS];
	for(int i=0;i<NUMBER_OF_CARDS;i++)
	{
		pout_A[i]=0;
		pout_B[i]=0;
	}
	int outA=0,lenA=NUMBER_OF_CARDS/2;
	int outB=0,lenB=NUMBER_OF_CARDS/2;
	int outTA=0,lenTA=0;
	int outTB=0,lenTB=0;




	
	while ((cur_conflicts<max_conflicts)&&(lenA>0)&&(lenB>0))
	{
		int cardA=cbuff_pop(player_A,&lenA,&outA);
		int cardB=cbuff_pop(player_B,&lenB,&outB);
		if((cardA/NUMBER_OF_COLORS)>(cardB/NUMBER_OF_COLORS))
		{
			cbuff_push(player_A,cardA,&outA,&lenA);
			cbuff_push(player_A,cardB,&outA,&lenA);
			cur_conflicts++;
		}
		else if((cardA/NUMBER_OF_COLORS)<(cardB/NUMBER_OF_COLORS))
		{
			cbuff_push(player_B,cardB,&outB,&lenB);
			cbuff_push(player_B,cardA,&outB,&lenB);
			cur_conflicts++;
		}
		else
		{ 	if(simplified_mode==0)
			{
				cur_conflicts++;
				if (cur_conflicts==max_conflicts)
					{
						printf("0\n");
						printf("%d ",lenA+1+lenTA);
						printf("%d ",lenB+1+lenTB);
						return;	
					}
				do
				{
					if (cur_conflicts==max_conflicts)
					{
						printf("0\n");
						printf("%d ",lenA+1+lenTA);
						printf("%d ",lenB+1+lenTB);
						return;	
					}
					if ((lenA>=2)&&(lenB>=2))
					{
						
						cbuff_push(pout_A,cardA,&outTA,&lenTA);
						cbuff_push(pout_B,cardB,&outTB,&lenTB);
						cardA=cbuff_pop(player_A,&lenA,&outA);
						cardB=cbuff_pop(player_B,&lenB,&outB);
						cbuff_push(pout_A,cardA,&outTA,&lenTA);
						cbuff_push(pout_B,cardB,&outTB,&lenTB);
						cardA=cbuff_pop(player_A,&lenA,&outA);
						cardB=cbuff_pop(player_B,&lenB,&outB);
						cur_conflicts++;

					}
					else
					{
						printf("1\n");
						printf("%d ",lenA+1+lenTA);
						printf("%d ",lenB+1+lenTB);
						return;
					}
				}while((cardA/NUMBER_OF_COLORS)==(cardB/NUMBER_OF_COLORS));
				cbuff_push(pout_A,cardA,&outTA,&lenTA);
				cbuff_push(pout_B,cardB,&outTB,&lenTB);
				
				
				if((cardA/NUMBER_OF_COLORS)>(cardB/NUMBER_OF_COLORS))
				{
					while(lenTA>0)
					{
						int card=cbuff_pop(pout_A,&lenTA,&outTA);
						cbuff_push(player_A,card,&outA,&lenA);
						
					}
					while(lenTB>0)
					{
						int card=cbuff_pop(pout_B,&lenTB,&outTB);
						cbuff_push(player_A,card,&outA,&lenA);	
					}	
				}
				else
				{
					while(lenTB>0)
					{
						int card=cbuff_pop(pout_B,&lenTB,&outTB);
						cbuff_push(player_B,card,&outB,&lenB);	
					}	
					while(lenTA>0)
					{
						int card=cbuff_pop(pout_A,&lenTA,&outTA);
						cbuff_push(player_B,card,&outB,&lenB);
						
					}
					
				}
				

			}
			else
			{
				cbuff_push(player_A,cardA,&outA,&lenA);
				cbuff_push(player_B,cardB,&outB,&lenB);
				cur_conflicts++;
			}
		}
			
	}
	if(lenB==0)
	{
		printf("2\n");
		printf("%d ",cur_conflicts);
		
		
	}
	else if(lenA==0)
	{
		printf("3\n");
		cbuff_print(player_B,&lenB,&outB);
		

	}
	else
	{
		printf("0\n");
		printf("%d ",lenA);
		printf("%d ",lenB);
		
		
	}

	





}
/*void first_takes_cards(int number_of_cards_on_table, int player_1[], int *pout_1, int *p_cards_1,
		const int player_2[], int *pout_2, int *p_cards_2, int size) {
}
void both_take_cards(int player_1[], int *pout_1, const int *p_cards_1,
		int player_2[], int *pout_2, const int *p_cards_2, int size) {
}*/

int main(void) {
	int player_A[NUMBER_OF_CARDS], player_B[NUMBER_OF_CARDS];
	int deck[NUMBER_OF_CARDS];
	int max_conflicts;
	int simplified_mode;
	int seed;
	scanf("%d", &seed);
	scanf("%d", &simplified_mode);
	scanf("%d", &max_conflicts);

	for(int i = 0; i < NUMBER_OF_CARDS; i++) deck[i] = i;
	srand((unsigned) seed);
	shuffle(deck, NUMBER_OF_CARDS);
	for(int i = 0; i < NUMBER_OF_CARDS / 2; i++) {
		player_A[i] = deck[i];
		player_B[i] = deck[i + NUMBER_OF_CARDS / 2];
	}
	game(NUMBER_OF_CARDS, player_A, player_B, max_conflicts, simplified_mode);
	return 0;
}

