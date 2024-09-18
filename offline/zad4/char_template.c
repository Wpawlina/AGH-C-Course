#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
	*nl = 0;
    *nw = 0;
    *nc = 0;
	int inWord=0;
	char c=fgetc(stdin);
	while(!feof(stdin)) {
        if(c == '\n')
            (*nl)++;
        if(inWord == 0 && c >= FIRST_CHAR && c <= LAST_CHAR){
            inWord = 1;
            (*nw)++;
        }

        if(inWord != 0 && (c == ' ' || c == '\n' || c == '\t')){
            inWord = 0;
        }

        (*nc)++;
        c = fgetc(stdin);
    }




}
void bubbleSort(char A1[],int A2[],int size)
{
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size-1;j++)
        {
            if(A2[A1[j]-FIRST_CHAR]<A2[A1[j+1]-FIRST_CHAR])
            {
                char buffer;
                buffer=A1[j];
                A1[j]=A1[j+1];
                A1[j+1]=buffer;
            }

        }
    }

}

void char_count(int char_no, int *n_char, int *cnt) {
    *n_char=0;
    *cnt=0;
    char char_array[MAX_CHARS] = {0};
    for (int i = 0; i < MAX_CHARS; i++){
        char_array[i] = FIRST_CHAR + i;
    }
    char c = fgetc(stdin);
    while (!feof(stdin)){
        if (c >= FIRST_CHAR && c <= LAST_CHAR-1)
            count[c - FIRST_CHAR]++;

        c = fgetc(stdin);
    }
    bubbleSort(char_array,count,MAX_CHARS);
    *n_char = (char_array[char_no - 1]);
    *cnt = count[*n_char-FIRST_CHAR];


}


void bigram_count(int bigram_no, int bigram[]) {
    int digrams[MAX_BIGRAMS]={0};
    for(int i=0;i<MAX_CHARS;i++)
    {
      for(int j=0;j<MAX_CHARS;j++)
      {
         digrams[i*MAX_CHARS+j]=i*MAX_CHARS + j ;
      }
    }
    char c1=fgetc(stdin);
    char c2=fgetc(stdin);
    while(!feof(stdin))
    {
         if(c1 != ' ' && c1 != '\n' && c1 != '\t' && c2 != ' ' && c2 != '\n' && c2 != '\t')
            {
                count[(int)(c1-FIRST_CHAR)*MAX_CHARS+(int)(c2-FIRST_CHAR)]++;

            }
        c1=c2;
        c2=fgetc(stdin);
    }
    qsort(digrams,MAX_BIGRAMS,sizeof (int),cmp_di);
    bigram[0] = digrams[bigram_no - 1] / MAX_CHARS + FIRST_CHAR;
    bigram[1] = digrams[bigram_no - 1] % MAX_CHARS + FIRST_CHAR;
    bigram[2] = count[digrams[bigram_no - 1]];
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
     *line_comment_counter = 0;
    *block_comment_counter = 0;

    char c1 = fgetc(stdin);
    char c2 = fgetc(stdin);

    int flag = 0;

    do {
        if(flag == 0) {
            if (c1 == '/' && c2 == '/') {
                (*line_comment_counter)++;
                flag = IN_LINE_COMMENT;
                c1 = c2;
                c2 = fgetc(stdin);
            }
            else if(c1 == '/' && c2 == '*'){
                (*block_comment_counter)++;
                flag = IN_BLOCK_COMMENT;
                c1 = c2;
                c2 = fgetc(stdin);
            }
        } else {
            if (    (flag == IN_LINE_COMMENT && (c1 == '\n' || c2 == '\n')) ||
                    (flag == IN_BLOCK_COMMENT && (c1 == '*' && c2 == '/'))){
                flag = 0;
            }
        }

        c1 = c2;
        c2 = fgetc(stdin);
    } while(!feof(stdin));
}

#define MAX_LINE 128

int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

