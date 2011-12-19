#include "wifi_status.h"
#include "report.h"
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
int report(AP_info* pinfo, int AP_num,int command,int posID){
    struct sockaddr_in serv_addr;
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
	fprintf(stderr,"socket error");
	exit(1);
    }
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERVERPORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(serv_addr.sin_zero),8);
    if(connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
	fprintf(stderr,"connect error\n");
	exit(1);
    }
    AP_head head;
    head.AP_num=AP_num;
    head.cmd=command;
    switch(command){
	case CALIBERATION:
	    head.posID=posID;
	    break;
	case QUERY:
//		printf("client location request\n");
	    break;
	default:
	    break;
    }
    unsigned char send_buf[1024];
    unsigned char recv_buf[256];
    memcpy(send_buf,&head,sizeof(AP_head));
    memcpy(send_buf+sizeof(AP_head),pinfo,sizeof(AP_info)*AP_num);
    int i;
    send(sfd,send_buf,sizeof(AP_info)*AP_num+sizeof(AP_head),0);
//   printf("wait server calculate location\n"); 
	int nNetTimeOut=5000;
	setsockopt(sfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeOut,sizeof(int));
    int nrecv=recv(sfd,recv_buf,256,0);
    
    if(nrecv>1)
    {

	if(recv_buf[0]=='0')
		;//printf("calibration over\n");
	else if(recv_buf[0]=='1')
		printf("%s,%s,%s\n",&recv_buf[1],&recv_buf[4+1],&recv_buf[4*2+1]);
	else
		printf("some error occured\n");
	}
    close(sfd);
}
