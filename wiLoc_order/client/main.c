#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wifi_status.h"
#include "report.h"
int main(int argc,char* argv[]){
    if(argc<3){
	printf("usage %s command posID\n",argv[0]);
	exit(1);
    }
    int command, posID;
    int AP_num,i;
    AP_info info[100];
    AP_num=wifi_status(info);
//    fprintf(stderr,"number of AP is %d\n",AP_num);
    int delta[20];
    memset(delta,0,20);
 /*   for(i=0;i<AP_num;i++){
	printf("%lld: %d %d\n",info[i].mac,info[i].quality,info[i].level);
    }
  */
    command=atoi(argv[1]);
    posID=atoi(argv[2]);
    report(info,AP_num,command,posID);
}
