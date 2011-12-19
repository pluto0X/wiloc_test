#include <stdio.h>
#include <stdlib.h>
#define MAX_AP_NUMBER 100
#define POWER_NUMBER 20//from 0 dbm to 100 dbm every 5dbm
typedef struct AP_spec_t{
    long long int mac;
    int power[POWER_NUMBER];
} AP_spec;
int main(int argc,char *argv[]){
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
    //spectrum
    AP_spec spec[MAX_AP_NUMBER];
    int j;
    for(j=0;j<MAX_AP_NUMBER;j++){
	spec[j].mac=-1;
    }
    while(fscanf(fp,"%lld: %d %d", &mac, &quality,&level)!=EOF){
	int i;
	i=0;
	while(mac!=spec[i].mac&&spec[i].mac!=-1){
	    i++;
	}
	if(spec[i].mac!=-1){
	    int index=-1*level/5;
	    spec[i].power[index]+=1;
	}
	if(spec[i].mac==-1&&i<MAX_AP_NUMBER-1){
	    int index=-1*level/5;
	    spec[i].power[index]+=1;
	    spec[i].mac=mac;
	}else{
	    //do nothing
	}
    }
    close(fp);
    int i=0;
    char output_path[256];
    sprintf(output_path,"./idx/%s.idx",argv[1]); 
    fp=fopen(output_path,"w");
    while(spec[i].mac!=-1){
	fprintf(fp,"%lld", spec[i].mac);
	printf("%lld", spec[i].mac);
	for(j=0;j<POWER_NUMBER;j++){
	    printf(" %d",spec[i].power[j]);
	    fprintf(fp," %d",spec[i].power[j]);
	}
	printf("\n");
	fprintf(fp,"\n");
	i++;
    }
}
