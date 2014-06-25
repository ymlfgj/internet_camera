#ifndef __SYS_MESSAGE_QUEUE_H__
#define __SYS_MESSAGE_QUEUE_H__
#include "GobalDefine.h"
#include "ThreadBase.h"
#include "SysTimer.h"




#define MAX_MESSAGEQUEUE_SIZE      32
#define DEQUEUE_FINISH				1




typedef struct SysMessage{
	THREADAPP reciver;
	MessageID  msgID;
	MessageParameter param;
}SysMessage;


typedef struct QueuePosition{
	UINT8   m_enqueuePosition;
	UINT8   m_dequeuePosition;
	UINT8	m_dequeueStatus[MAX_MESSAGEQUEUE_SIZE];
	UINT16  m_dequeueCount[MAX_MESSAGEQUEUE_SIZE];
}QueuePosition;



typedef struct SysMessageQueue{
	SysMessage m_ptrMessage[MAX_MESSAGEQUEUE_SIZE];
	QueuePosition  m_position;
	UINT8   m_current_size;
	UINT8   m_enqueuePosition;
	UINT32  m_flag;
	TIMERID m_timer;
	MUTEXID m_mutex;
}SysMessageQueue;



#define INCREASE(position,limit)        ((position == (limit-1))?(position =0):(position++))
#define DECREASE(position,limit)		((position == 0)?(position =(limit -1):(position--))







void SysMessageQueue_Initialize(void);
int SysMessageQueue_Send(MessageID MsgID,MessageParameter param);
int SysMessageQueue_Peek(THREADAPP receiver,MessageID *msgID,MessageParameter *param);


















#endif // __SYS_MESSAGE_QUEUE_H__


