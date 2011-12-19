#include "wiLoc.h"
#include <stdio.h>
#include <stdlib.h>

int wiLoc_order(AP_info *ap,int apn,char *logname,char *result)
{
	char output_path[256];
	FILE *fp=NULL;
	int i=0;
	int j=0;
	int lastApn=0;	
	AP_order order;
	int tmp=0;
	int min=0;
	int n=0;
	int newn=0;

	char ch;
	int flagnew=0;

	int macSeq[MAX_AP_NUMBER];
	
	printf("\n ***order start****\n");
	for(i=0;i<MAX_AP_NUMBER;i++)
		macSeq[i]=-1;

//	for(i=0;i<apn;i++)
//		printf("mac:%lld  %d\n",ap[i].mac,ap[i].level);

	if(apn<=0)
		return;
	for(i=0;i<MAX_AP_NUMBER;i++)
	{
		order.mac[i]=0;
		for(j=0;j<MAX_AP_NUMBER;j++)
			order.order[i][j]=0;
	}
//read
	sprintf(output_path,"./ord/%s.ord",logname);
	
	fp=fopen(output_path,"r+");

//if file doesn't exist,create it and inital,then readdata
	if(fp==NULL)
	{
		//create it,doesn't inital,just keep order to 0;
		printf("open error ,create file\n");
		fp=fopen(output_path,"w+");
		flagnew=1;
	}
	else
	{
		//file exist,read data to order	
		//read data to AP_order
		printf("step 1.1,ord have n AP to order.mac apn:%d \n",apn);
		while(!feof(fp)&&(ch=fgetc(fp))!='\n')
                	if(ch==',')
                        	n++;
        	printf("read n:%d\n",n);
        	fseek(fp,0L,0);
                for(i=0;i<n;i++)
                {
                	fscanf(fp,"%lld ",&order.mac[i]);
                        for(j=0;j<n;j++)
                        	fscanf(fp,"%d,",&order.order[i][j]);
                }
		fseek(fp,0L,0);
	}
//write	
//consider new mac come,should add it
//order+ap=new all ap             newn
	
	i=0;j=0;
	newn=n;
	printf("step 1.2 order+ap=new order\n");
	for(i=0;i<apn;i++)
	{
		for(j=0;j<n;j++)
		{
			if(ap[i].mac==order.mac[j])
			{
				macSeq[j]=i;
				break;
			}
		}
		if(j>=n)//newn
		{
			order.mac[newn]=ap[i].mac;
			macSeq[newn]=i;
			printf("add mac:%lld\n",order.mac[newn]);			
			newn++;
		}
	}	


	printf("newn:%d\n",newn);
//debug info
/*	for(i=0;i<newn;i++)
	{
		printf("i:%d,mac:%lld\n",i,order.mac[i]);
		}
*/
//
//compare and write to AP_order

	printf("step 2,compare and order \n");
	for(i=0;i<newn;i++)
	{
		if(macSeq[i]>-1)
		{
			for(j=0;j<i;j++)
			{
				if(macSeq[j]>-1)
				{	
					if(ap[macSeq[i]].level>ap[macSeq[j]].level)
						order.order[i][j]++;
					else if(ap[macSeq[i]].level<ap[macSeq[j]].level)
						order.order[i][j]--;	
					else
					{
						//+0
					}
				}
			}
		}
	}
//write to file
	printf("step 3:write to file.newn:%d\n",newn);

	for(i=0;i<newn;i++)
	{
		for(j=0;j<newn;j++)
		{
			fprintf(fp,"%lld ",order.mac[i]);
			printf("%lld",order.mac[i]);
			for(j=0;j<newn;j++)
			{
				fprintf(fp," %d,",order.order[i][j]);
				printf(" %d,",order.order[i][j]);	
			}
			printf("\n");
			fprintf(fp,"\n");
		}
	}

	printf("***order end ****\n");
	fclose(fp);
}
