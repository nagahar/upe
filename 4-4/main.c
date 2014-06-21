#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_CHAR 256
#define WORD_NUM 1000

struct key{
	char *wd;
	int c;
}w[WORD_NUM];

int getword(char *word, int lim)
{
	int c, getch(void);
	void ungetch(int);
	char *w=word;

	while(isspace(c=getch()))
		;
	if(c!=EOF)
		*w++=c;
	if(!isalpha(c)){
		*w='\0';
		return c;
	}
	for(;--lim>0;w++)
		if(!isalnum(*w=getch())){
			ungetch(*w);
			break;
		}
	*w='\0';
	return word[0];
}

int main()
{

	int i;
	char word[MAX_CHAR];
	int count=0;

	while(getword(word,MAX_CHAR)!=EOF)
		if(isalpha(word[0])){
			for(i=0;i<strlen(word);i++)
				word[i]=tolower(word[i]);
			for(i=0;i<count;i++)
				if(strcmp(word,w[i].wd)==0){
					w[i].c++;
					break;
				}
			if(i==count){
				char *p;
				p=malloc(strlen(word));
				strcpy(p,word);
				w[i].wd=p;
				w[i].c++;
				if(++count>=WORD_NUM)
					break;
			}
		}
	for(i=0;i<count;i++)
		printf("%4d %s\n",w[i].c,w[i].wd);

	return 0;
}
