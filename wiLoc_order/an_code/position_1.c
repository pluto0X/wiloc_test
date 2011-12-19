#include <dirent.h>
#include "wiLoc.h"


//calculate how many .idx file in ./idx/
int dirn_1(char *path,char *suffix)
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
	closedir(dirp);
	return N;
}
double score_cal_1(AP_SCORE *score,AP_info *info,char *path,char *suffix,int n,int m,char *result)
{
	DIR *dirp;
	struct dirent *dir;	
	FILE *fp;
	int i=0,j=0;
	double max=0;
	int index=0;
    	long long int mac;
        long long int  power[POWER_NUMBER]={0};
	char  tmpPath[256]={0};
	
	dirp=opendir(path);
	if(dirp==NULL)
	{
		printf("error open");
		return;
	}
	int nsuffix=strlen(suffix);
	i=0;
	while((dir=readdir(dirp))!=NULL)
	{
		if(!strcmp(dir->d_name,".")||!strcmp(dir->d_name,".."))
		{}
		else
		{
			char *p=dir->d_name;
			if(!strncmp((p+strlen(p)-nsuffix),suffix,nsuffix))
			{
				if(i>=n)
					break;
				
				sprintf(tmpPath,"%s%s",path,p);
				fp=fopen(tmpPath,"r");
				if(fp==NULL)
				{
					printf("read %s fail\n",p);
        		    		continue;
      				}
			    	while(fscanf(fp,"%lld",&mac)!=EOF){
            				for(j=0;j<20;j++)
                				fscanf(fp,"%lld",&(power[j]));
            				fscanf(fp,"\n");
            				for(j=0;j<m;j++){
                				if(mac==info[j].mac){
//							printf("%lld:",info[j].mac);
                    					index=-1*info[j].level/5;
                    					if(index<POWER_NUMBER)
							{
								if(power[index]>0)
									score[i].score+=log10(power[index]);
  //              						printf("index:%d power: %lld %llx",index,power[index],power[index]);
							}
//							printf("\n");
						}
            				}
       			 	}
				//printf("i=%d %s score %4.15f\n",i,p,score[i]);
				strncpy(score[i].name,p,strlen(p));
				if(score[i].score>max)
				{
					max=score[i].score;
					strncpy((result+strlen(result)+1),p,(strlen(p)-8));
					//printf("max location:%s maxscore: %d\n",result+1,max);
				}
				if(i<n)
					i++;

				fclose(fp);
			}			
		}
	}
	closedir(dirp);
	return max;
}
int wiLoc_position_1(AP_info *ap,int apn,char *result){

	//apn means m AP >m lines in per *.idx,nidx means n location n *.idx 
	int mAP=sizeof(AP_info)*apn;
	int i=0;
	AP_info *info=malloc(mAP);
	memcpy(info,ap,mAP);
	//copy data complete.send signal to server.
	printf("\nnormalized data\n");

	int nidx=dirn_1("./idx_1",".idx");
	int nidxSize=sizeof(AP_SCORE)*nidx;
	AP_SCORE *score=malloc(nidxSize);

	memset(score,0,nidxSize);
/*	int i=0,no=0;
	for(i=0;i<nidx;i++)
		score[i].=0.0;
*//////////////////////////////////////

    	printf("%d candidates read\n", nidx);
	result[0]='1';
	double mymax=score_cal_1(score,info,"./idx_1/",".idx",nidx,apn,result);
	printf("max:%s,%f\n",result+strlen(result)+1,mymax);	
	for(i=0;i<nidx;i++)
	{

		printf("%s scores %f\n",score[i].name,score[i].score);
	}

	free(score);
	free(info);
}
