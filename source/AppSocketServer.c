#include "AppSocketServer.h"
#include "SysMessageQueue.h"
#include <SDL.h>

static void* OnMessage(void);
static int WatchRecvProcess(void *arg);

static ThreadApp SrApp;
static SDL_Thread* rec_thread;


void SocketServer_Initialize(void)
{
	SrApp.m_onwer = APP_SOCKET_SERVER;
	SrApp.AppStartEntry = SocketServerThread;
	SrApp.IsRunning = TRUE;
	SrApp.OnMessage = OnMessage;

	rec_thread = SDL_CreateThread(WatchRecvProcess, NULL);

	SysThread_SetActive(SrApp.m_onwer);
	Sys_CreateThreadApp(SrApp.m_onwer,NULL,SrApp.AppStartEntry,NULL);
}


static void OnCmmand(MessageParameter *pParam )
{
	int cmd = pParam->command;
	
	switch(cmd)
	{
		case NOTIFY_CONNECT:
		{
			APP_DBG(APP_SOCKET_SERVER_DEBUG,printf("Srv OnCmmand NOTIFY_CONNECT recevied !\n"));
		}
		break;

		default:break;
	}
}



static void* OnMessage(void)
{
	MessageID msgid = MSG_MAX;
	MessageParameter param;

	SysMessageQueue_Peek(SrApp.m_onwer,&msgid,&param);

	switch(msgid)
	{
		case MSG_KEY_PRESS: 
		{
			APP_DBG(APP_SOCKET_SERVER_DEBUG,printf("MSG_KEY_PRESS is recviced %c\n",param.u8[0]))
		}
		break;
		
		case MSG_APP_DEACTIVE:break;

		case MSG_APP_COMMAND:
		{
			OnCmmand(&param);
		}
		break;

		default:break;
	}

	return ((void*)0);
	
}





void DeliverSocketProcess(int ClientNum,struct sockaddr_in Remote_addr)
{
	int SockClient = -1;

	SockClient = Get_SockClient(ClientNum);
	
	switch(ClientNum)
	{
		case 0:break;
		
		case 1:break;
		
		case 2:break;
		
		case 3:break;
		
		case 4:break;

		default: break;
	}
}


int WatchRecvProcess(void *arg)
{
	for(;;)
	{
		int i = 0;

		int max_fd = Get_MaxClientFd();

		for(i = 0; i < BACKLOG; i++)
		{
			int sockfd = Get_SockClient(i);

			if(sockfd != BACKLOG)
			{
				
			}
		}

		
	}

	
	return 0;
}


void*SocketServerThread(void *arg)
{
	APP_DBG(APP_SOCKET_SERVER_DEBUG,printf("SocketServerThread start\n"))

	int SockServer;
	int SockClient;
	int ClientNumber = 0;
	struct sockaddr_in Server_addr;
	struct sockaddr_in Remote_addr;

	SockServer = Init_Socket_Server(&Server_addr,SERVER_IP,SERVPORT,BACKLOG);

	while(SrApp.IsRunning)
	{
		SrApp.OnMessage();
		sleep_ms(20);

		fd_set freads;
		FD_ZERO(&freads);
		FD_SET(SockServer,&freads);

		struct timeval tv;
		tv.tv_sec = 0;
	    tv.tv_usec = 1000; //1ms

		int r = select(SockServer+1, &freads, NULL, NULL, &tv);

		if(r <= 0)
		{
			continue;
		}
		else
		{
			if( (SockClient = TryAcceptClient(SockServer,&Remote_addr)) > 0 )
			{
				ClientNumber = Register_SockClient(SockClient);

				MessageParameter param;

				CLEAR(param);
				param.msg.from = APP_SOCKET_SERVER;
				param.msg.to   = APP_SOCKET_SERVER;
				param.command  = NOTIFY_CONNECT;
				param.u8[0]    = ClientNumber;

				SysMessageQueue_Send(MSG_APP_COMMAND,param);
			}
		}


	}

	Sys_ExitThreadApp(SrApp.m_onwer);

	return ((void*)0);
	
}
























