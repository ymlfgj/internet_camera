#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#include "GobalDefine.h"
#include "SocketServer.h"



int Init_Socket_Server(struct sockaddr_in *pServer_addr, const char *IP, int serverport,int Max_Num_Clients)
{
	int sockfd;
	
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket create failed!");
		exit(1);
	}
	
	pServer_addr->sin_family=AF_INET;
	pServer_addr->sin_port=htons(serverport);
    pServer_addr->sin_addr.s_addr = inet_addr(IP); 
	bzero(&(pServer_addr->sin_zero),8);
	
	if(bind(sockfd,(struct sockaddr*)pServer_addr,sizeof(struct sockaddr))==-1)
	{
		perror("bind port failed!");
		exit(1);
	}
	
	if(listen(sockfd,Max_Num_Clients)==-1)
	{
		perror("listen port failed!");
		exit(1);
	}

	int option = 1;

	if(ioctl(sockfd,FIONBIO,&option) < 0)
	{
		perror("socket set non block failed!");
		return -1;
	}
	
	return sockfd;
}


int TryAcceptClient(int Server_socket,struct sockaddr_in *pRemote_addr)
{
	
	socklen_t size_len = 0;
	int client_fd = 0;
	
	size_len = sizeof(struct sockaddr_in);

	if((client_fd = accept(Server_socket,(struct sockaddr*)pRemote_addr,&size_len))==-1)
	{
		printf("Accept Continue!\n");
		return ACCEPT_CONITINUE;
			
	}
	else
	{
		return client_fd;
	}
}








































