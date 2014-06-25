#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>


#define SERVPORT 1500
#define BACKLOG  5

#define ERROR_OK 0

int Init_Socket_Client(struct sockaddr_in *pServer_addr, const char *IP, int serverport);
int Get_SockClient(int client_num);
int Get_MaxClientFd(void);
int Register_SockClient(int sockclient);










#endif //__SOCKET_CLIENT_H_

