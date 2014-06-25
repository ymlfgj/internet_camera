#include "AppSocketClient.h"
#include "VideoCapture.h"
#include "VideoUtil.h"
#include "BmpUtil.h"
#include "SysMessageQueue.h"

static void* OnMessage(void);
unsigned char BmpCapIsOK = FALSE;


static ThreadApp ClApp;
static int SockClient = -1;


void SocketClient_Initialize(void)
{
	ClApp.m_onwer = APP_SOCKET_CLIENT;
	ClApp.AppStartEntry = SocketCilentThread;
	ClApp.IsRunning = TRUE;
	ClApp.OnMessage = OnMessage;
	
	SysThread_SetActive(ClApp.m_onwer);
	Sys_CreateThreadApp(ClApp.m_onwer,NULL,ClApp.AppStartEntry,NULL);
}

static void SocketClient_Finalization(void)
{
	close(SockClient);
	Sys_ExitThreadApp(ClApp.m_onwer);
}

static void OnCommand(COMMAND command_id)
{
	switch(command_id)
	{
		case CAPTURE_IS_OK:
		{
			APP_DBG(APP_CAMERA_DEBUG,printf("Receive CAPTURE_IS_OK\n"))
			BmpCapIsOK = TRUE;
			break;
		}

		default: break;
		
	}
}


static void* OnMessage(void)
{
	MessageID msgid = MSG_MAX;
	MessageParameter param;

	SysMessageQueue_Peek(ClApp.m_onwer,&msgid,&param);

	switch(msgid)
	{
		case MSG_KEY_PRESS:
		{
			APP_DBG(APP_SOCKET_CLIENT_DEBUG,printf("MSG_KEY_PRESS is recviced %c\n",param.u8[0]))
			break;
		}
		
		case MSG_APP_DEACTIVE:
		{
			if(param.deactive.from == APP_MANAGER)
			{
				if(param.deactive.to == ClApp.m_onwer)
				{
				  ClApp.IsRunning = FALSE;
				  APP_DBG(APP_CAMERA_DEBUG,printf("MSG_APP_DEACTIVE is recviced\n"))
				}
			}
			break;
		}

		case MSG_APP_COMMAND:
		{
			if(param.command!=COMMAND_MAX)
			{
			  OnCommand(param.command);
			}
			break;
		}

		default:break;
	}

	return ((void*)0);
	
}



void*SocketCilentThread(void *arg)
{
	APP_DBG(APP_SOCKET_CLIENT_DEBUG,printf("SocketCilentThread start\n"))

    struct sockaddr_in remote_addr;

	SockClient = Init_Socket_Client(&remote_addr,SERVER_IP,SERVPORT);

	if(SockClient < 0)
	{
		ClApp.IsRunning = FALSE;
	}

	if(connect(SockClient ,(struct sockaddr*)&remote_addr,sizeof(remote_addr)) < 0)
	{
		fd_set fwriteds;
		FD_ZERO(&fwriteds);
		FD_SET(SockClient,&fwriteds);

		struct timeval tv;
		tv.tv_sec = 5;
	    tv.tv_usec = 0;

		int r = select(SockClient+1,NULL,&fwriteds,NULL,&tv);

		if( r <= 0 )
		{
			perror("Connect failed or time out !\n");
			ClApp.IsRunning = FALSE;
		}
		else
		{
			APP_DBG(APP_SOCKET_CLIENT_DEBUG,printf("Connect server IP success !\n"));
		}
		
	}

 	while(ClApp.IsRunning)
 	{
 		ClApp.OnMessage();
		sleep_ms(10);
 	}
	
	SocketClient_Finalization();
	return ((void*)0);
	
}

