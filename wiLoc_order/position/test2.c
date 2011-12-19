#include <stdio.h>
#include <dirent.h>
int dirn(char *path,char *suffix)
{
	DIR *dirp;
	struct dirent *dir;	
	int N=0;
	dirp=opendir(path);
	if(dirp==NULL)
	{
		printf("error open");
		return;
	}
	int nsuffix=strlen(suffix);
	while((dir=readdir(dirp))!=NULL)
	{
		if(!strcmp(dir->d_name,".")||!strcmp(dir->d_name,".."))
		{}
		else
		{
			char *p=dir->d_name;
			if(!strncmp((p+strlen(p)-nsuffix),suffix,nsuffix))
				N++;			
		}
	}
	return N;
}
void main(int argc,char *argv[])
{
	printf("%d\n",dirn("../position",".info"));
}
