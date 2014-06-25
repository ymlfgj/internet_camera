#include "GobalDefine.h"
#include "SocketClient.h"

static int SockClients[BACKLOG] = { 0, };
static unsigned char ClientFlag = 0;


int Register_SockClient(int sockclient)
{
	int i;
	int bvalid = 0;
	for(i = 0;i<BACKLOG;i++)
	{
	 	if(GET_BIT(ClientFlag, i)!=0)
	 	{
			if(SockClients[i] ==sockclient)
			return i;
	 	}
	}
		
	for(i =0 ;i<BACKLOG;i++)
	{
		if(GET_BIT(ClientFlag,i)==0)
		{
			SET_BIT(ClientFlag,i);
			bvalid = 1;
			break;
		}
	}

	printf("ClientFlag =%x Current used bit =%d\n",ClientFlag,i);

	if(bvalid)
	{
		SockClients[i] = sockclient;
		 return i;
	}
	else
	{
		 perror("Over Max Clients");
	}
	

	return BACKLOG;
	
	
}

int Get_SockClient(int client_num)
{
	if(GET_BIT(ClientFlag,client_num)!=0)
		return SockClients[client_num];


	return BACKLOG;
}

int Get_MaxClientFd(void)
{
	int i;
	int max_fd = 0;

	for(i = 0; i < BACKLOG; i++)
	{
		int fd = Get_SockClient(i);
		
		if(fd > max_fd)
		{
			max_fd = fd;
		}
	}

	return max_fd;
}


int Init_Socket_Client(struct sockaddr_in *pServer_addr, const char *IP, int serverport)
{
	int sockfd;
	
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket create failed!");
		return -1;
	}
	
	pServer_addr->sin_family=AF_INET;
	pServer_addr->sin_port=htons(serverport);
    pServer_addr->sin_addr.s_addr = inet_addr(IP); 
	bzero(&(pServer_addr->sin_zero),8);

	int option = 1;

	if(ioctl(sockfd,FIONBIO,&option) < 0)
	{
		perror("socket set non block failed!");
		return -1;
	}
	
	return sockfd;
}











































