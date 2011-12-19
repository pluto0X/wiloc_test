#include "../client/wifi_status.h"
#define MAX_AP_NUMBER 100
typedef struct AP_spect_t{
    long long int mac;
    int power[20];
}AP_spec;
int main(int argc,char *argv[]){
    if(argc<3){
	printf("usage:%s ap_info pos1.idx pos2.idx\n",argv[0]);
	exit(1);
    }
    FILE *fp=fopen(argv[1],"r");
 	printf("%s is loaded as pos\n",argv[1]);
    if(fp==NULL){
	printf("can't open file %s\n",argv[1]);
	exit(1);
    }
    AP_info info[MAX_AP_NUMBER];
    int AP_num=0;
    long long int mac;
    int quality;
    int level;
    while(fscanf(fp,"%lld: %d %d",&mac,&quality,&level)!=EOF){
	info[AP_num].mac=mac;
	info[AP_num].quality=quality;
	info[AP_num].level=level;
	AP_num++;
    }
    printf("%d ap_info read\n",AP_num);
    fclose(fp);
    int i;

	printf("argc is %d\n",argc);
    int *score=malloc(sizeof(int)*(argc-2));
    memset(score,0,sizeof(int)*(argc-2));
    printf("%d candidates read\n", argc-2);
    for(i=2;i<argc;i++){
	//read every idx
	fp=fopen(argv[i],"r");
	if(fp==NULL){
	    printf("read %s fail\n",argv[i]);
	    continue;
	}
	long long int mac;
	int power[20];
	while(fscanf(fp,"%lld",&mac)!=EOF){
	    int j;
	    for(j=0;j<20;j++)
		fscanf(fp,"%d",&(power[j]));
	    fscanf(fp,"\n");
	    for(j=0;j<AP_num;j++){
		if(mac==info[j].mac){
		    int index=-1*info[j].level/5;
		    score[i-2]=power[index]+score[i-2];
		}
	    }
	}
	close(fp);
    }
    char max_idx[256];
    int max_score=0;
    for(i=2;i<argc;i++){
	printf("%s scores %d\n",argv[i],score[i-2]);
	if(score[i-2]>max_score){
		strcpy(max_idx,argv[i]);
		max_score=score[i-2];
	}
    }
    printf("**%s is the resutl\n",max_idx);
    printf("\n");
}
