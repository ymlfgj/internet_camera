#ifndef __SYS_TIMER_H__
#define __SYS_TIMER_H__


#include "GobalDefine.h"
#include "ThreadBase.h"




#define   MAX_TIMER_COUNT 32
#define   UNUSED		  0
#define	  USED			  1
#define   TIMERFULL		 0xFF

#define	  TIMER_PERIOD	  20000	  //20ms
#define   TIMER_ZERO	  0


void SysTimer_Initialize(void);
void SysTimer_Start(TIMERID id,UINT32 ms);
int SysTimer_GetTick(void);
TIMERID SysTimer_RegisterID(void);
int SysTimer_CheckTimeOut(TIMERID id);
void OnTimer(int signo);











#endif //__SYS_TIMER_H__


