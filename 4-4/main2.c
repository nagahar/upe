#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_CHAR 256
#define WORD_NUM 1000
#define DICT "/usr/share/dict/words"

struct key{
	char *wd;
	int c;
	int check;
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
	int count=0;
	char word[MAX_CHAR];
	char line[MAX_CHAR];
	FILE *fp;

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
	if((fp=fopen(DICT,"r"))==NULL){
		fprintf(stderr,"can't open %s",DICT);
		exit(1);
	}
	while(fgets(line,MAX_CHAR,fp)!=NULL){
		line[strlen(line)-1]='\0';
		for(i=0;i<count;i++)
			if(strcmp(line,w[i].wd)==0){
				w[i].check=1;
				break;
			}
	}

	for(i=0;i<count;i++)
		if(w[i].check!=1)
			printf("%s\n",w[i].wd);

	return 0;
}
