
#include "AppManager.h"
#include "SysMessageQueue.h"
#include "SysKey.h"
#include "SysTimer.h"
#include "SysMain.h"
#include "GrEngine.h"


void SystemInitialize(void)
{
	SYS_DBG(SYS_MAIN_DEBUG,printf("SystemInitialize start\n"))
	
	SysThreadBase_Initialize();
	SysTimer_Initialize();
	SysMessageQueue_Initialize();
	SysKey_Initialize();
	GrEngine_Initialize();
	ThreadApps_Initialize();
	
}



void SystemFinalize(void)
{
	SYS_DBG(SYS_MAIN_DEBUG,printf("SystemFinalize start\n"))
	
	Sys_WaitThreadsFinalize();
}


int SystemMainLoop(void)
{
	while(1)
	{
	  sleep_second(10);
	}

	return TRUE;
}






















