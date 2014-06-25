#include "SysKey.h"

static ThreadApp SysKey;

ThreadApp* Get_pSysKey(void)
{
	return &SysKey;
}


void SysKey_Initialize(void)
{
	SysKey.m_onwer = SYS_KEY;
	SysKey.AppStartEntry = SysCheckKey;
	SysKey.IsRunning = TRUE;

	Sys_CreateThreadApp(SysKey.m_onwer,NULL,SysKey.AppStartEntry,NULL);
}


void Sys_GetKeyValue(KEY_VALUE *pValue)
{
	char ch;
	
	CLEAR(ch);
	ch = getchar();
	if(ch!=0)
	{
		*pValue = ch;
	}
	CLEAR(ch);
	
	
}



void*SysCheckKey(void *arg)
{
	KEY_VALUE key_val = 0;
	MessageParameter param;
	param.u8[0] = 0;
	
	while(SysKey.IsRunning)
	{
		Sys_GetKeyValue(&key_val);
		if(key_val!=0&&key_val!='\n')
	    {
		  param.u8[0] = key_val;
		  SysMessageQueue_Send(MSG_KEY_PRESS,param);
		  sleep_ms(100);
		}
	}

	return ((void*)0);
}




















