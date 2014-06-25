#ifndef __SOCKET_SERVER_H__
#define __SOCKET_SERVER_H__

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

typedef int socketfd;

typedef enum{
	ACCEPT_SUCCESS,
	ACCEPT_CONITINUE,
	ACCEPT_FAIL,
	ACCEPT_MAX
}ACCEPTSTATE;

int Init_Socket_Server(struct sockaddr_in *pServer_addr, const char *IP, int serverport,int Max_Num_Clients);
int TryAcceptClient(int Server_socket,struct sockaddr_in *pRemote_addr);



#endif//__SOCKET_SERVER_H__