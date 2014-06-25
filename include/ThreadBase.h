#ifndef __THREAD_BASE_H__
#define __THREAD_BASE_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "GobalDefine.h"


#define MAX_THREAD   16
#define MAX_MUTEX_COUNT 32
#define MUTEX_INVALID	0xFF



typedef struct ThreadHandler{
	pthread_t handler;
	THREADAPP owner;
	BOOL      IsActive;
}ThreadHandler;

typedef struct ThreadApp{
	THREADAPP m_onwer;
	MUTEXID   m_mutex;
	BOOL  IsRunning;
	void* (*AppStartEntry)(void*);
	void* (*OnMessage)(void);
}ThreadApp;


void SysThreadBase_Initialize(void);
int  Sys_CreateThreadApp(THREADAPP app,const pthread_attr_t *restrict_attr,void* (*start_rtn)(void*),void* arg);
void Sys_ExitThreadApp(THREADAPP app);
void SysThread_SetActive(THREADAPP app);
void SysThread_SetDeAcitve(THREADAPP app);
UINT8 Sys_Print_NumbersUsedMutex(void);
UINT32 Get_CurActiveApps(void);
int Sys_WaitThreadsFinalize(void);
MUTEXID SysMutex_Register(void);
void SysMutex_Clear(MUTEXID id);
int Mutex_LockNonBlock(MUTEXID id);
int Mutex_LockBlock(MUTEXID id);
int Mutex_UnLock(MUTEXID id);












#endif //__THREAD_BASE_H__


