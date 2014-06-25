#include "Debug.h"


const char *pDebugSys[5]={
		"SYS_MESSAGE_DEBUG:",
		"SYS_KEY_DEBUG    :",
		"SYS_TIMER_DEBUG  :",
		"SYS_THREAD_DEBUG :",
		"SYS_MAIN_DEBUG   :",
};

const char *pDebugApp[4]={
	   "APP_CAMERA_DEBUG:",
	   "APP_MANAGER_DEBUG:",
	   "APP_SOCKET_CLIENT_DEBUG:",
	   "APP_SOCKET_SERVER_DEBUG:",
};

void PrintLevel(DEBUG_SORT sort,unsigned int level)
{
	 
	UINT8 i;
	for (i=0;i<32;i++)
	{
	 if(GET_BIT(level,i)!=0)
	 {
	 	switch(sort)
	 	{
	 		case SYS_DEBUG: printf(pDebugSys[i]);break;
			case APP_DEBUG: printf(pDebugApp[i]);break;
			default:break;
	 	}
		break;
	 }
	 		
	}
  
	
}

























