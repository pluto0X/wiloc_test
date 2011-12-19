#include <stdio.h>
#include <stdlib.h>
#include "wiLoc.h"

int wiLoc_index(int argc,char *argv[],char *result){
    if(argc<2){
	printf("usage:%s pathtofile\n",argv[0]);
	exit(1);
    }
    char input_path[256];
    sprintf(input_path,"./log/%s",argv[1]);
    FILE *fp=fopen(input_path,"r");
    if(fp==NULL){
	printf("can't open file",input_path);
	exit(1);
    }
    long long int mac;
    int quality;
    int level;
    char output_path[256];
    //spectrum
    AP_spec spec[MAX_AP_NUMBER];
    int j,i;
    for(j=0;j<MAX_AP_NUMBER;j++){
	spec[j].mac=-1;
	for(i=0;i<POWER_NUMBER;i++)
		spec[j].power[i]=0;
    }
   i=0;j=0;


	

//read *.log to spec[i],m
	int apnum=0; 
    while(fscanf(fp,"%lld: %d %d", &mac, &quality,&level)!=EOF){//read one line in *.log
	i=0;
	while(mac!=spec[i].mac&&spec[i].mac!=-1&&i<MAX_AP_NUMBER){
	    i++;
	}
	if(spec[i].mac!=-1){				//old mac
	    	int index=-1*level/5;
		if(index<POWER_NUMBER)
	    		spec[i].power[index]+=1;
	}
	if(spec[i].mac==-1&&i<MAX_AP_NUMBER-1){		//new mac
	    	int index=-1*level/5;
		if(index<POWER_NUMBER)
	    		spec[i].power[index]+=1;
	    	spec[i].mac=mac;
		apnum++;
	}else{
	    //do nothing
	}
    }
    fclose(fp);
	i=0;
	j=0;

//idx
    	sprintf(output_path,"./idx/%s.idx",argv[1]); 

	printf("open:%s\n",output_path);
    	fp=fopen(output_path,"w");


//spec[i] write to *.idx
   	for(i=0;spec[i].mac!=-1;i++){
		fprintf(fp,"%lld", spec[i].mac);
//		printf("%lld", spec[i].mac);
		for(j=0;j<POWER_NUMBER;j++){
	   // 		printf(" %lld",spec[i].power[j]);
	    		fprintf(fp," %lld",spec[i].power[j]);
		}
	//	printf("\n");
		fprintf(fp,"\n");
    	}
	fclose(fp);


//debug info
/*	for(i=0;i<apnum;i++){
		printf("%15lld",spec[i].mac);
		for(j=0;j<POWER_NUMBER;j++)
			printf("%lld ",spec[i].power[j]);
		printf("\n");
	}
*/
//


//idx_1
//Normalized
	long long int total=0;
	printf("antique AP_NUM:%d\n",apnum);
	for(i=0;i<apnum;i++)
	{
		total=0;
		for(j=0;j<POWER_NUMBER;j++)
			total+=	spec[i].power[j];
		for(j=0;j<POWER_NUMBER;j++)
			spec[i].power[j]=spec[i].power[j]*MAX_NORMALIZED/total;
	}
//normalized over
 
    	sprintf(output_path,"./idx_1/%s.idx",argv[1]); 

	printf("open:%s\n",output_path);
    	fp=fopen(output_path,"w");


//spec[i] write to *.idx
   	for(i=0;spec[i].mac!=-1;i++){
		fprintf(fp,"%lld", spec[i].mac);
//		printf("%lld", spec[i].mac);
		for(j=0;j<POWER_NUMBER;j++){
//	    		printf(" %lld",spec[i].power[j]);
	    		fprintf(fp," %lld",spec[i].power[j]);
		}
//		printf("\n");
		fprintf(fp,"\n");
    	}
	fclose(fp);
	
	result[0]='0';
	result[1]='0';
	return 0;
}
