#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "GobalDefine.h"


typedef enum{
	SYS_DEBUG,
	APP_DEBUG,
}DEBUG_SORT;



#define SYS_MESSAGE_DEBUG					(UINT32)(1<<0)
#define SYS_KEY_DEBUG						(UINT32)(1<<1)											
#define SYS_TIMER_DEBUG						(UINT32)(1<<2)
#define SYS_THREAD_DEBUG					(UINT32)(1<<3)
#define SYS_MAIN_DEBUG						(UINT32)(1<<4)

#define SYS_DEBUG_LEVEL						(SYS_MESSAGE_DEBUG)



#define APP_CAMERA_DEBUG					(UINT32)(1<<0)
#define APP_MANAGER_DEBUG					(UINT32)(1<<1)
#define APP_SOCKET_CLIENT_DEBUG				(UINT32)(1<<2)
#define APP_SOCKET_SERVER_DEBUG             (UINT32)(1<<3)


#define APP_DEBUG_LEVEL						(APP_CAMERA_DEBUG|APP_SOCKET_CLIENT_DEBUG|APP_SOCKET_SERVER_DEBUG|APP_MANAGER_DEBUG)



#define SYS_DBG(flag,s)						{if(flag&SYS_DEBUG_LEVEL){PrintLevel(SYS_DEBUG,flag);s;}}
#define APP_DBG(flag,s)						{if(flag&APP_DEBUG_LEVEL){PrintLevel(APP_DEBUG,flag);s;}}


void PrintLevel(DEBUG_SORT sort,unsigned int level);




















#endif//__DEBUG_H__





