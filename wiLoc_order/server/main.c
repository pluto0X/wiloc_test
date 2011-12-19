#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "../client/wifi_status.h"
#include "../client/report.h"
int main(){
    int LISTEN_BACKLOG=10;
    int sfd=socket(AF_INET,SOCK_STREAM,0);	
    struct sockaddr_in my_addr, their_addr;
    if(sfd==-1){
	fprintf(stderr,"socket error\n");
	exit(1);
    }
    //struct sockaddr addr;
    my_addr.sin_family=AF_INET;                
    my_addr.sin_port = htons(9100);            
    my_addr.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(my_addr.sin_zero), 8);               
    if(bind(sfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1){
	fprintf(stderr,"bind error\n");
	exit(1);
    }
    if(listen(sfd,LISTEN_BACKLOG)==-1){
	fprintf(stderr,"listen error\n");
	exit(1);
    }
    while(1){
	int newfd;
	socklen_t sin_size;
	int recv_size;
	unsigned char buf[1024];
	unsigned char temp_buf[1024];
	sin_size=sizeof(struct sockaddr);
	int total_size=0;
	newfd=accept(sfd,(void*)&their_addr,&sin_size);
	while((recv_size=recv(newfd,temp_buf,1024,0))>0)
	{
	    memcpy(buf+total_size,temp_buf,recv_size);
	    memset(temp_buf,0,256);
	    total_size+=recv_size;
	}while(recv_size>0);
	close(newfd);
	int i;
//	for(i=0;i<total_size;i++){
//	    printf("%x ",buf[i]); 
	//}
//	printf("\n");
	AP_head head;
	memcpy(&head,buf,sizeof(AP_head));
	printf("cmd:%d posID:%d AP_num:%d\n",head.cmd,head.posID,head.AP_num);
	AP_info *pinfo;
	pinfo=malloc(sizeof(AP_info)*head.AP_num);
	memcpy(pinfo,buf+sizeof(AP_head),sizeof(AP_info)*head.AP_num);
	int j;
	for(j=0;j<head.AP_num;j++){
	    printf("%lld: %d %d\n",pinfo[j].mac,pinfo[j].quality,pinfo[j].level);
	}
	if(head.cmd==CALIBERATION){
	    char log_path[256];
	    sprintf(log_path,"./log/%d.log",head.posID);
	    FILE *log=fopen(log_path,"a");
	    for(j=0;j<head.AP_num;j++)
		fprintf(log,"%lld: %d %d\n",pinfo[j].mac,pinfo[j].quality,pinfo[j].level);
	    fclose(log);
	
//should system index log
	}
    }
} 
