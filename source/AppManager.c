#include "SysMessageQueue.h"
#include "SysKey.h"
#include "AppManager.h"
#include "AppCamera.h"
#include "AppSocketClient.h"
#include "AppSocketServer.h"



static void* OnMessage(void);


static ThreadApp AppManager;


void Application_Active(MessageParameter param)
{
	THREADAPP ACTIVEAPP = param.active.to;
	switch(ACTIVEAPP)
	{
		case APP_CAMERA_CAPTURE:		CameraThread_Initialize();break;
		case APP_SOCKET_CLIENT:			SocketClient_Initialize();break;
		case APP_SOCKET_SERVER:			SocketServer_Initialize();break;
		default:						break;
	}
	
}
void Application_Deactive(MessageParameter param)
{
    if((param.deactive.to!=AppManager.m_onwer)&&(param.deactive.from != AppManager.m_onwer))
	{
	  
	  param.deactive.from = AppManager.m_onwer;
	  param.msg.to = param.deactive.to;
	  APP_DBG(APP_MANAGER_DEBUG,printf("param.msg.to =%d\n",param.msg.to))
	  SysMessageQueue_Send(MSG_APP_DEACTIVE,param);
	}

	
}

static void OnKey(MessageParameter param)
{
	KEY_VALUE KEY_ID = param.u8[0];
	APP_DBG(APP_MANAGER_DEBUG,printf("MSG_KEY_PRESS is recviced %c\n",param.u8[0]))
	switch(KEY_ID)
	{
		case KEY_C:	
		{
		 	param.active.to = APP_CAMERA_CAPTURE;
			SysMessageQueue_Send(MSG_APP_ACTIVE,param);
			break;
		}
		default:break;
		
					
	}
}


static void* OnMessage(void)
{
	MessageID msgid = MSG_MAX;
	MessageParameter param;

	SysMessageQueue_Peek(AppManager.m_onwer,&msgid,&param);

	switch(msgid)
	{
		case MSG_KEY_PRESS: OnKey(param);
							break;
		
		case MSG_APP_ACTIVE:
						    Application_Active(param);
							break;
		
		case MSG_APP_DEACTIVE:
			               	Application_Deactive(param);
							break;

		default:break;
	}

	return ((void*)0);
	
}




void ThreadApps_Initialize(void)
{
	AppManager.m_onwer = APP_MANAGER;
	AppManager.IsRunning = TRUE;
	AppManager.AppStartEntry = ThreadAppManager;
	AppManager.OnMessage = OnMessage;
	SysThread_SetActive(AppManager.m_onwer);

	Sys_CreateThreadApp(AppManager.m_onwer,NULL,AppManager.AppStartEntry,NULL);
}




void* ThreadAppManager(void*arg)
{
#ifdef ARM_BOARD
	CameraThread_Initialize();
	SocketClient_Initialize();
#else
	SocketServer_Initialize();
#endif

	while(AppManager.IsRunning)
	{
		sleep_ms(20);
		AppManager.OnMessage();
	}
	Sys_ExitThreadApp(AppManager.m_onwer);
	return ((void*)0);
	
}









