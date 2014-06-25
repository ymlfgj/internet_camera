#include "ThreadBase.h"

static ThreadHandler thread[MAX_THREAD];
static short thread_flag = 0;

static pthread_mutex_t mutex[MAX_MUTEX_COUNT];
static UINT32 mutex_flag = 0;


static UINT32 curActiveApps = 0;
static MUTEXID mutexthread;

UINT32 Get_CurActiveApps(void)
{
	return curActiveApps;
}


static pthread_t FindThreadHandlerByApp(THREADAPP app, UINT8 *oNum)
{
	int i=0;
	pthread_t handler;
	for(i =0;i<MAX_THREAD;i++)
	{
	  if(GET_BIT(thread_flag,i)!=0)
	  {
		if(thread[i].owner == app)
		{
			*oNum = i;
			handler = thread[i].handler;
			break;
		}
	  }
	}

	return handler;
}



void SysThread_SetActive(THREADAPP app)
{
	UINT8 num = 0;

	FindThreadHandlerByApp(app,&num);
	thread[num].IsActive = TRUE;

	SET_BIT(curActiveApps,app);
	
	
}


void SysThread_SetDeAcitve(THREADAPP app)
{

	UINT8 num = 0;

	FindThreadHandlerByApp(app,&num);
	thread[num].IsActive = FALSE;

	CLEAR_BIT(curActiveApps,app);
	
}


void SysThreadBase_Initialize(void)
{
	CLEAR(thread);
	CLEAR(thread_flag);
	CLEAR(mutex);
	CLEAR(mutex_flag);
	mutexthread = SysMutex_Register();
}



/**************************************************************************************
Thread MODULE
**************************************************************************************/



static pthread_t *Register_Thread(THREADAPP app)
{
	unsigned char i;
	unsigned char valid = 0;
	for(i=0; i<MAX_THREAD;i++)
	{
		if(GET_BIT(thread_flag,i)==0)
		{
			SET_BIT(thread_flag,i);
			thread[i].owner = app;
			valid = 1;
			break ;
		}
	}

	SYS_DBG(SYS_THREAD_DEBUG,printf("thread_flag =%x Current used bit =%d\n",thread_flag,i))

	if(valid)
	{
		return &thread[i].handler;
		
	}
	else
	{
		SYS_DBG(SYS_THREAD_DEBUG,printf("Cannot Register any thread\n"))
		return NULL;
	}

	
    return NULL;

}




int Sys_CreateThreadApp(THREADAPP app,const pthread_attr_t *restrict_attr,void* (*start_rtn)(void*),void* arg)
{
	Mutex_LockBlock(mutexthread);
	
	int ret = 0;
	pthread_t *threadhandler;

	threadhandler = Register_Thread(app);
	
	ret = pthread_create(threadhandler,restrict_attr,start_rtn,arg);

	if(ret!=0)
		SYS_DBG(SYS_THREAD_DEBUG,printf("Create thread failure\n"))
	
	else
		SYS_DBG(SYS_THREAD_DEBUG,printf("Create thread success\n"))

	Mutex_UnLock(mutexthread);


	return ret;
}


void Sys_ExitThreadApp(THREADAPP app)
{
	Mutex_LockBlock(mutexthread);
	pthread_t handler;
	UINT8 Num =0;

	handler = FindThreadHandlerByApp(app,&Num);

	CLEAR_BIT(thread_flag,Num);
	SysThread_SetDeAcitve(app);
	Mutex_UnLock(mutexthread);

	pthread_exit(NULL);
}


static int Wait_ThreadEnd(pthread_t threadhandler)
{

	int ret = 0;

	ret = pthread_join(threadhandler,NULL);

	if(ret == 0)
		SYS_DBG(SYS_THREAD_DEBUG,printf("exit success \n"))
	else
		SYS_DBG(SYS_THREAD_DEBUG,printf("error ID =%d \n",ret))


	return ret;
	
}


int Sys_WaitThreadsFinalize(void)
{
	unsigned char i;
	for(i=0;i<MAX_THREAD;i++)
	{
	  if(GET_BIT(thread_flag,i)!=0)
	  {
	  	Wait_ThreadEnd(thread[i].handler);
		CLEAR_BIT(thread_flag,i);
	  }
	}

	thread_flag = 0;

	return TRUE;
	
}
/**************************************************************************************
Thread MODULE
**************************************************************************************/




/**************************************************************************************
MUTEX MODULE
**************************************************************************************/
MUTEXID SysMutex_Register(void)
{
	int i;
	MUTEXID id = MUTEX_INVALID;
	SYS_DBG(SYS_THREAD_DEBUG,printf("mutex_flag = 0x%x\n",mutex_flag))
	for(i=0;i<MAX_MUTEX_COUNT;i++)
	{
		if(GET_BIT(mutex_flag,i)==UNUSED)
		{
			id = i;
			SET_BIT(mutex_flag,id);
			pthread_mutex_unlock(&mutex[id]);
			break;
		}
	}

	if(id == MUTEX_INVALID)
		SYS_DBG(SYS_THREAD_DEBUG,printf("Mutex Queue is Full\n"))
	
	return id;
}

void SysMutex_Clear(MUTEXID id)
{
	if(GET_BIT(mutex_flag,id) == USED)
	{
		CLEAR_BIT(mutex_flag,id);
		pthread_mutex_unlock(&mutex[id]);
		
	}
}


int Mutex_LockNonBlock(MUTEXID id)
{
	int ret = 0;
	if(GET_BIT(mutex_flag,id) == USED)
		ret = pthread_mutex_trylock(&mutex[id]); 

	return ret;
	
}

int Mutex_LockBlock(MUTEXID id)
{
	int ret =0;
    if(GET_BIT(mutex_flag,id) == USED)
		ret = pthread_mutex_lock(&mutex[id]);

	return ret;
	
}


int Mutex_UnLock(MUTEXID id)
{
	int ret = 0;
	if(GET_BIT(mutex_flag,id)== USED)
	ret = pthread_mutex_unlock(&mutex[id]);

	return ret;
	
}
/**************************************************************************************
MUTEX MODULE
**************************************************************************************/
UINT8 Sys_Print_NumbersUsedMutex(void)
{
	UINT8 count = 0;
	UINT8 MutexCount =0;

	for(count=0;count<MAX_MUTEX_COUNT;count++)
	{
		if(GET_BIT(mutex_flag,count)==USED)
		 {
		 	MutexCount++;
		 }
	}

	return MutexCount;
}






















