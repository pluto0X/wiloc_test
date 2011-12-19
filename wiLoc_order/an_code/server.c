#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/epoll.h>
#include "wiLoc.h"
#include "../client/report.h"
#define QUEUELEN 1024
#define THREAD_NUM 3
pthread_mutex_t mlock=		PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t m_condlock=	PTHREAD_COND_INITIALIZER;
int sockArray[QUEUELEN]={0};
int sockFront=0;
int sockRear=0;
int m_epollfd;

//
void *thread_main();
void thread_make(int i)
{
	pthread_t ntid;
	int j=0;
	for(;j<i;j++)
		pthread_create(&ntid,NULL,&thread_main,NULL);
}
void *thread_main()
{
	//wait sock,read and deal
	int sock;
	while(1)
	{
	//lock
	pthread_t tid=pthread_self();
	printf("%u wait \n",(unsigned int)tid);	
	pthread_mutex_lock(&mlock);
	while(sockFront==sockRear)
		pthread_cond_wait(&m_condlock,&mlock);	
	if(sockFront!=sockRear)
	{	
		sockFront=(sockFront+1)%QUEUELEN;
		sock=sockArray[sockFront];
		printf("%u get data %d \n",(unsigned int)tid,sock);	
	}
	//unlock
	pthread_mutex_unlock(&mlock);
	//deal data:recv data;
	int recv_size;
	unsigned char buf[1024];
	int total_size=0;

	recv_size=recv(sock,buf,1024,0);
	if(recv_size>0)
	{
		//head
		printf("total size:%d\n",recv_size);
		AP_head head;
		memcpy(&head,buf,sizeof(AP_head));
		printf("cmd:%d posID:%d AP_num:%d\n",head.cmd,head.posID,head.AP_num);
		AP_info *pinfo;
		//copy data to pinfo
		pinfo=malloc(sizeof(AP_info)*head.AP_num);
		memcpy(pinfo,buf+sizeof(AP_head),sizeof(AP_info)*head.AP_num);
		int j;
		for(j=0;j<head.AP_num;j++)
	    		printf("%lld: %d %d\n",pinfo[j].mac,pinfo[j].quality,pinfo[j].level);
	
		char result[256]={0};
	    	char log_path[256];
	    	char log_name[256];
	    	char *argc[2]={"index",log_name};
		//deal data	
		if(head.cmd==CALIBERATION){			//flag=0,caliberate,write to /log/NO.log
	    		printf("caliberation!\n");
	    		sprintf(log_path,"./log/%d.log",head.posID);
	    		FILE *log=fopen(log_path,"a");
	    		for(j=0;j<head.AP_num;j++)
				fprintf(log,"%lld: %d %d\n",pinfo[j].mac,pinfo[j].quality,pinfo[j].level);
	    		fclose(log);
	    		sprintf(log_name,"%d.log",head.posID);
			printf("log ok\n");			//exec index log
			if(wiLoc_index(2,argc,result))
			{
				printf("index error\n");
			}
			wiLoc_order(pinfo,head.AP_num,log_name,result);
			printf("index ok,calibration over!\n");
		}
		else if(head.cmd==QUERY)			//location request
		{
			printf("location request\n");
			wiLoc_position(pinfo,head.AP_num,result);
			wiLoc_position_1(pinfo,head.AP_num,result);
			wiLoc_position_o(pinfo,head.AP_num,result);
		}
		else
			printf("unknown command request %d\n",head.cmd);
		//deal over
		free(pinfo);

		//send sock something
		int nsend=send(sock,result,256,0);
		printf("deal over");
		struct epoll_event ev;
		epoll_ctl(m_epollfd,EPOLL_CTL_DEL,sock,&ev);
		close(sock);
			

	}
	}//while over
}

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
	int nfds;
	m_epollfd=epoll_create(QUEUELEN);
	struct epoll_event m_epollev[QUEUELEN];
	struct epoll_event ev;
	ev.data.fd=sfd;
	ev.events=EPOLLIN|EPOLLET;
	epoll_ctl(m_epollfd,EPOLL_CTL_ADD,sfd,&ev);

	int sockfd=0;int i=0;
	int sock;
	socklen_t sin_size;
	   
	thread_make(THREAD_NUM);

	while(1){
//	epoll
	//if recv,lock sockArray[sockRear++]=sock; unlock;
	//read rear=(rear+1)%QUEUELEN	sockArray[rear]=sock;
	nfds=epoll_wait(m_epollfd,m_epollev,QUEUELEN,-1);
	for(i=0;i<nfds;i++)
	{
		sockfd=m_epollev[i].data.fd;
		struct epoll_event _event=m_epollev[i];
		if(sfd==sockfd)
		{
			//accept
			sin_size=sizeof(struct sockaddr);
			sock=accept(sfd,(void*)&their_addr,&sin_size);
			ev.data.fd=sock;
			ev.events=EPOLLIN|EPOLLET;
			epoll_ctl(m_epollfd,EPOLL_CTL_ADD,sock,&ev);
		}
		else if(_event.events&EPOLLIN)
		{
		//lock
			pthread_mutex_lock(&mlock);
			if(sockFront==((sockRear+1)%QUEUELEN))
			{
				printf("GET TO THE MAX LIMIT %d\n",QUEUELEN);
				pthread_mutex_unlock(&mlock);
				continue;
			}
			sockRear=(sockRear+1)%QUEUELEN;
			sockArray[sockRear]=sockfd;
			printf("here we are %d\n",sockfd);
			pthread_mutex_unlock(&mlock);
			pthread_cond_signal(&m_condlock);	
		//unlock
		}
		else
			printf("epoll:else\n");
	}
    }
} 
