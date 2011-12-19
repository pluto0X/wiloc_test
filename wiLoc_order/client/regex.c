#include <pcre.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define OVERCOUNT 10
#define PCRE_ANCHORED 1
int main(int argc, char* argv[]){
    pcre *re;
    const char *error;
    int erroroffset;
    int overctor[OVERCOUNT];
    int rc;
    char *regex="(hello) wo.?ld";
    char *data="hello world hello woild";
    re=pcre_compile(regex,0,&error,&erroroffset,NULL);
    if(!re){
	fprintf(stderr,"PCRE compile error");
	return 1;
    }
    rc=pcre_exec(re, NULL,data,strlen(data),0,0,overctor,OVERCOUNT);
    if(rc<0){
	switch(rc){
	    case PCRE_ERROR_NOMATCH:
		printf("No match found in text\n");
		break;
	    default:
		printf("error class %d\n",rc);
	}
	return 1;
    }
    printf("%d matches are found in the data\n",rc);
    int i;
    for(i=0;i<rc*2;i=i+2){
	printf("%d\n",overctor[i]);
	printf("%d\n",overctor[i+1]);
	printf("%.*s\n",overctor[i+1]-overctor[i],data+overctor[i]);
    }
}
