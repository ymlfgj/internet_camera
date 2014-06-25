#include "SysTimer.h"


static UINT32 SysTick;
static UINT32 TimerID_Flag;
static UINT32 SysTimer[MAX_TIMER_COUNT];
static MUTEXID timer_mutex;

#define  TimerToTick(ms)		(ms/20);
#define  Timer_Out(id)			(SysTimer[id] == TIMER_ZERO)



void SysTimer_Initialize(void)
{
	SysTick = TIMER_ZERO;
	TimerID_Flag = TIMER_ZERO;
	struct itimerval _in_timer,out_timer;

	CLEAR(SysTimer);

	timer_mutex = SysMutex_Register();

	signal(SIGALRM,OnTimer);

	_in_timer.it_value.tv_sec = 0;
	_in_timer.it_value.tv_usec = TIMER_PERIOD;
	_in_timer.it_interval.tv_sec = 0;
	_in_timer.it_interval.tv_usec = TIMER_PERIOD;

	setitimer(ITIMER_REAL,&_in_timer,&out_timer);
	
}


int SysTimer_GetTick(void)
{
	return SysTick;
}



TIMERID SysTimer_RegisterID(void)
{
	int i=0;
	TIMERID id = TIMERFULL;

	for(i=0;i<MAX_TIMER_COUNT;i++)
	{
		if(GET_BIT(TimerID_Flag,i) == UNUSED)
		{
			  id = i;
			  SET_BIT(TimerID_Flag,i);
			  SYS_DBG(SYS_TIMER_DEBUG, printf("TimerID_Flag =%x  id =%d\n",TimerID_Flag,id))
			  break;
		}
	}

	if(id == TIMERFULL)
		return TIMERFULL;
	else
		return id;
	
}


void SysTimer_Start(TIMERID id,UINT32 ms)
{
	int tick = TimerToTick(ms);

	if(GET_BIT(TimerID_Flag,id) == USED)
	{
		SysTimer[id] = tick ;
	}
	else
	{
		SYS_DBG(SYS_TIMER_DEBUG,printf("Timer %d is not register\n",id))
	}
}


void SysTimer_UpDate(void)
{
	int i;
	for(i=0;i<MAX_TIMER_COUNT;i++)
	{
	   if(GET_BIT(TimerID_Flag,i) == USED)
	   {
		 if(!Timer_Out(i))
			SysTimer[i]--;
	   }
	}
}

int SysTimer_CheckTimeOut(TIMERID id)
{
	Mutex_LockBlock(timer_mutex);
	
	BOOL IsOut = FALSE;
	if(GET_BIT(TimerID_Flag,id) == USED)
	{
		if(SysTimer[id] == TIMER_ZERO)
		{
			CLEAR_BIT(TimerID_Flag,id);
			IsOut = TRUE;
			return IsOut;
		}
		
	}
	else
	{
		SYS_DBG(SYS_TIMER_DEBUG,printf("The TimerID %d is not register\n",id))
	}

	return IsOut;

	Mutex_UnLock(timer_mutex);
}



void OnTimer(int signo)
{
	switch(signo)
	{
		case SIGALRM:
						
					    SysTick++;
						SysTimer_UpDate();
						signal(SIGALRM,OnTimer);
						break;
		default:
						break;
	}
}





























