#include <dirent.h>
#include "wiLoc.h"


//calculate how many .idx file in ./idx/
int dirn_o(char *path,char *suffix)
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
double score_cal_o(AP_SCORE *score,AP_info *info,char *path,char *suffix,int n,int m,char *result)
{
	DIR *dirp;
	struct dirent *dir;	
	FILE *fp;
	int i=0,j=0,count=0;
	double max=0;
	int index=0;
	char  tmpPath[256]={0};

	int *seqp=malloc(sizeof(int)*m);
	int readn=0;
	AP_order order;
	int tmpscore=0;
	char ch;	
	int x=0,y=0;
	int xyflag=0;

	dirp=opendir(path);
	if(dirp==NULL)
	{
		printf("error open");
		return;
	}
	int nsuffix=strlen(suffix);
	i=0;count=0;
	while((dir=readdir(dirp))!=NULL)
	{
		if(!strcmp(dir->d_name,".")||!strcmp(dir->d_name,".."))
		{}
		else
		{
			char *p=dir->d_name;
			if(!strncmp((p+strlen(p)-nsuffix),suffix,nsuffix))
			{
				if(count>=n)
					break;
				
				
				sprintf(tmpPath,"%s%s",path,p);
				fp=fopen(tmpPath,"r");
				if(fp==NULL)
				{
					printf("read %s fail\n",p);
        		    		continue;
      				}
// deal *************************************************
				tmpscore=0;
				readn=0;
				for(i=0;i<m;i++)
					seqp[i]=-1;
			      	
        			for(i=0;i<MAX_AP_NUMBER;i++)
        			{
        			        order.mac[i]=0;
        			        for(j=0;j<MAX_AP_NUMBER;j++)
                			        order.order[i][j]=0;
        			}
//				printf("step 1: newn\n"); 
                               	while(!feof(fp)&&(ch=fgetc(fp))!='\n')
                                        if(ch==',')
                                                readn++;
//                              printf("read n:%d\n",readn);
                                fseek(fp,0L,0);
                                for(i=0;i<readn;i++)//read n
                                {
                                        fscanf(fp,"%lld ",&order.mac[i]);
                                        for(j=0;j<readn;j++)
                                                fscanf(fp,"%d,",&order.order[i][j]);
                                }
                                fseek(fp,0L,0);

				
//				printf("step 2:get seqp, read n=%d\n",readn); 
                                for(i=0;i<m;i++)
				{
					for(j=0;j<readn;j++)
					{
						if(info[i].mac==order.mac[j])
						{
							seqp[i]=j;
							break;
						}
					}
			    	}
				//debug info
/*			      //printf("file:%s\n",p);
				for(i=0;i<m;i++)
				{
					printf("%d ",seqp[i]);
				}
				printf("\n");
				for(i=0;i<readn;i++){
					printf("%lld:",order.mac[i]);
					for(j=0;j<readn;j++)
						printf(" %d",order.order[i][j]);
					printf("\n");
				}
*/				//
//				printf("step 3:cal score\n");	
				for(i=0;i<m;i++)
				{
					if(seqp[i]>-1)
					{
						for(j=0;j<i;j++)
						{
							if(seqp[j]>-1)
							{
                                                                if(seqp[i]>seqp[j]) 
                                                                { 
                                                                        x       =seqp[i]; 
                                                                        y       =seqp[j]; 
                                                                    //	xyflag  =1; 
									tmpscore=order.order[x][y];
                                                                } 
                                                                else 
                                                                { 
                                                                        x       =seqp[j]; 
                                                                        y       =seqp[i]; 
                                                                    //  xyflag  =-1; 
									tmpscore=0-order.order[x][y];
                                 				}
								

								if(info[i].level>info[j].level){	
									
								}
								else if(info[i].level<info[j].level){//	
									tmpscore=0-tmpscore;
								}	
								else
									tmpscore=0;
								if(tmpscore<0)	
									tmpscore=0;
							//	if(tmpscore<0)
							//		printf("i:%d,j:%d,tmpscore<0!%d\n",i,j,tmpscore);	
					      	//		printf("i=%d,j=%d,seqi=%d,seqj=%d,tmpscore:%d mac i:%lld,mac j:%lld\n",i,j,seqp[i],seqp[j],tmpscore,info[i].mac,info[j].mac);
                                                              
								score[count].score+=tmpscore;
							}
						}
					}
				}
	

//deal over*********************************************

				//printf("i=%d %s score %4.15f\n",i,p,score[i]);
				strncpy(score[count].name,p,strlen(p));
				if(score[count].score>max)
				{
					max=score[count].score;
					strncpy((result+9),p,(strlen(p)-8));
					//printf("max location:%s maxscore: %d\n",result+1,max);
				}
				if(count<n)
					count++;

				fclose(fp);
			}			
		}
	}
	free(seqp);
	closedir(dirp);
	return max;
}
int wiLoc_position_o(AP_info *ap,int apn,char *result){

	//apn means m AP >m lines in per *.idx,nidx means n location n *.idx 
	int mAP=sizeof(AP_info)*apn;
	int i=0;
	AP_info *info=malloc(mAP);
	memcpy(info,ap,mAP);
	//copy data complete.send signal to server.
	printf("order request start cal\n");

	int nidx=dirn_o("./ord",".ord");
	int nidxSize=sizeof(AP_SCORE)*nidx;
	AP_SCORE *score=malloc(nidxSize);
	memset(score,0,nidxSize);
/*	int i=0,no=0;
	for(i=0;i<nidx;i++)
		score[i].=0.0;
*//////////////////////////////////////

    	printf("%d candidates read\n", nidx);
	result[0]='1';
	double  mymax=score_cal_o(score,info,"./ord/",".ord",nidx,apn,result);
	printf("max:%s,%f\n",result+9,mymax);	
	for(i=0;i<nidx;i++)
	{

		printf("%s scores %f\n",score[i].name,score[i].score);
	}

	free(score);
	free(info);
}
