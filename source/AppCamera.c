#include "AppCamera.h"
#include "VideoCapture.h"
#include "VideoUtil.h"
#include "BmpUtil.h"
#include "GrEngine.h"
#include "SysMessageQueue.h"

static void* OnMessage(void);

static ThreadApp CmApp;


static void* OnMessage(void)
{
	MessageID msgid = MSG_MAX;
	MessageParameter param;
	SysMessageQueue_Peek(CmApp.m_onwer,&msgid,&param);

	switch(msgid)
	{
		case MSG_KEY_PRESS: 	APP_DBG(APP_CAMERA_DEBUG,printf("MSG_KEY_PRESS is recviced %c\n",param.u8[0]))
								if(param.u8[0] =='E')
								{
									CLEAR(param);
									param.deactive.from = APP_CAMERA_CAPTURE;
									param.deactive.to = APP_CAMERA_CAPTURE;
									param.msg.to = APP_MANAGER;
									APP_DBG(APP_CAMERA_DEBUG,printf("param.deactive.to %d\n",param.deactive.to))
									SysMessageQueue_Send(MSG_APP_DEACTIVE,param);
								}
								break;
		
		case MSG_APP_DEACTIVE:	if(param.deactive.from == APP_MANAGER)
								{
									if(param.deactive.to == CmApp.m_onwer)
									{
									  CmApp.IsRunning = FALSE;
									  APP_DBG(APP_CAMERA_DEBUG,printf("MSG_APP_DEACTIVE is recviced\n"))
									}
								}
								
								break;

		default:break;
	}


	return ((void*)0);
	
}



void CameraThread_Initialize(void)
{
	CmApp.m_onwer = APP_CAMERA_CAPTURE;
	CmApp.AppStartEntry = CameraThread;
	CmApp.IsRunning = TRUE;
	CmApp.OnMessage = OnMessage;

	SysThread_SetActive(CmApp.m_onwer);
	Sys_CreateThreadApp(CmApp.m_onwer,NULL,CmApp.AppStartEntry,NULL);
}


static int VideoCapture(void)
{
	fd_set freads;
	
	FD_ZERO(&freads);
	FD_SET(GetVideoFd(),&freads);

	int r = select(GetVideoFd()+1,&freads,NULL,NULL,NULL);
	
	if(-1 == r){
		if(EINTR == errno)
			perror("select timeout:");
	}
	
	if(0 == r){
		fprintf(stderr,"seletct timeout \n");
		exit(1);
	}
	
	if(r>0)
	{
		if(SaveOneFrameByBmp())
		{
			MessageParameter param;

			CLEAR(param);
			param.msg.from = APP_CAMERA_CAPTURE;
			param.msg.to   = APP_SOCKET_CLIENT;
			param.command  = CAPTURE_IS_OK;
			
			SysMessageQueue_Send(MSG_APP_COMMAND,param);
			return r;
		}
	}

	return r;
	
}


void*CameraThread(void *arg)
{
	APP_DBG(APP_CAMERA_DEBUG,printf("CameraThread start\n"))
	InitVideoCapture();
	StartCapture();
	
	while(CmApp.IsRunning)
	{
		CmApp.OnMessage();
		VideoCapture();
		sleep_ms(10);
	}
	EndCapture();
	Sys_ExitThreadApp(CmApp.m_onwer);

	
   return ((void*)0);
	
}








































