#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "wifi_status.h"

int wifi_status(AP_info* pinfo){
    FILE *fp=popen("/sbin/iwlist wlan0 scanning","r");
    char buffer[1024];
    int index=-1;
    while(fscanf(fp,"%s",buffer)!=EOF){
	//printf("%s\n",buffer);
	if(!strcmp(buffer,"Address:")){//if "Address" is detected, then next line will be the mac address
	    index++;
	    fscanf(fp,"%s",buffer);
	    //memcpy(&pinfo[index].macstr,buffer,17);
	    //printf("mac address of %d item %s\n",index,pinfo[index].macstr);
	    int j,k;
	    char temp_buf[100];
	    j=0;
	    k=0;
	    while(buffer[j]!='\0'){
		if(buffer[j]!=':')
		    temp_buf[k++]=buffer[j];
		j++;	    
	    }
	    //printf("pure mac is %s in hex\n",temp_buf);
	    pinfo[index].mac=strtoll(temp_buf,NULL,16);
	    //printf("pure mac is %lld\n",pinfo[index].mac);
	}
	if(!strncmp(buffer,"Quality=",8)){
	    char temp_buf[256];
	    memset(temp_buf,0,256);
	    int j,k;
	    j=8;k=0;
	    while(buffer[j]>='0'&&buffer[j]<='9'){
		temp_buf[k++]=buffer[j];
		j++;
	    }
	    pinfo[index].quality=atoi(temp_buf);
	    //printf("%s\n",buffer);
	    //printf("quality is %d\n",pinfo[index].quality);
	}
	if(!strncmp(buffer,"level=",6)){
	    pinfo[index].level=atoi(buffer+6);
	    //printf("level is %d\n",pinfo[index].level);
	    //printf("%s\n",buffer);
	}
	memset(buffer,0,1024);
    }
    pclose(fp);
    return index+1;
}
