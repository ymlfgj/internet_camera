#include "SysMessageQueue.h"

SysMessageQueue  SysMsgQue;
static UINT8 current_DeQueueMessageID;
#define  SetGurDeQueueMessageID(msg)		(current_DeQueueMessageID = msg)  //current dequeue message
#define  GetCurDeQueueMessageID()			(current_DeQueueMessageID)  //current deque message


#define SysMessageQueue_isFull()			(SysMsgQue.m_current_size == MAX_MESSAGEQUEUE_SIZE)
#define SysMessageQueue_isEmpty()       	(SysMsgQue.m_current_size == 0)

#define CurrentDeQueueCount(id)  			(SysMsgQue.m_position.m_dequeueCount[id])
#define CurrentDeQueueStatus(id) 			(SysMsgQue.m_position.m_dequeueStatus[id])
 
void DeQueue_Initialize(UINT8 MAX_QUEUE_SIZE)
{
	UINT8 i;
	for(i=0;i<MAX_QUEUE_SIZE;i++)
	{
		SysMsgQue.m_position.m_dequeueStatus[i] = DEQUEUE_FINISH; 
	}
	SysMsgQue.m_position.m_dequeuePosition = 0;
	SetGurDeQueueMessageID(SysMsgQue.m_position.m_dequeuePosition);
	
}

void SysMessageQueue_Initialize(void)
{
	
	CLEAR(SysMsgQue);
	DeQueue_Initialize(MAX_MESSAGEQUEUE_SIZE);
	SysMsgQue.m_timer = SysTimer_RegisterID();
	SysMsgQue.m_mutex = SysMutex_Register();
	SYS_DBG(SYS_MESSAGE_DEBUG,printf("SysMsgQue.m_timer =%d SysMsgQue.m_mutex = %d\n",SysMsgQue.m_timer,SysMsgQue.m_mutex))
}



int SysMessageQueue_Send(MessageID MsgID,MessageParameter param)
{
	UINT8  curPosition =  0;
	
	Mutex_LockBlock(SysMsgQue.m_mutex);
	if(SysMessageQueue_isFull())
	{
		SYS_DBG(SYS_MESSAGE_DEBUG,printf("MessageQueue is Full\n"))
		Mutex_UnLock(SysMsgQue.m_mutex);
		return -1;
	}

	SysMsgQue.m_current_size++;
	SYS_DBG(SYS_MESSAGE_DEBUG,printf("Current MessageQueue Size =%d\n",SysMsgQue.m_current_size))

	curPosition = SysMsgQue.m_position.m_enqueuePosition;
	
	SysMsgQue.m_ptrMessage[curPosition].msgID = MsgID;
	SysMsgQue.m_ptrMessage[curPosition].param = param;

	CLEAR(SysMsgQue.m_position.m_dequeueStatus[curPosition]);
	CLEAR(SysMsgQue.m_position.m_dequeueCount[curPosition]);

	SET_BIT(SysMsgQue.m_flag,curPosition);
	INCREASE(SysMsgQue.m_position.m_enqueuePosition,MAX_MESSAGEQUEUE_SIZE);
	SYS_DBG(SYS_MESSAGE_DEBUG,printf("Current enqueuePosition =%d\n",SysMsgQue.m_position.m_enqueuePosition))
    Mutex_UnLock(SysMsgQue.m_mutex);
	
	return 0;
}





int SysMessageQueue_Peek(THREADAPP receiver,MessageID *msgID,MessageParameter *param)
{
	UINT8 curdemsgid;
	
	MessageParameter Invalidparam;
	CLEAR(Invalidparam);
	
	if(Mutex_LockNonBlock(SysMsgQue.m_mutex)!=0)
	{
		SYS_DBG(SYS_MESSAGE_DEBUG,printf("The Mutex is used by other now!\n"))
		return -1;
	}
	else
	{
	  
	 curdemsgid = GetCurDeQueueMessageID();
	  
	 if(SysMsgQue.m_ptrMessage[curdemsgid].msgID == MSG_KEY_PRESS) //BroadCast method
	 {
	      if(CurrentDeQueueStatus(curdemsgid) !=DEQUEUE_FINISH)
		  {
		  	if(GET_BIT(SysMsgQue.m_position.m_dequeueCount[curdemsgid],receiver)==0)
			{
			   *msgID = SysMsgQue.m_ptrMessage[curdemsgid].msgID;
		       *param = SysMsgQue.m_ptrMessage[curdemsgid].param;
			   
			   SET_BIT(SysMsgQue.m_position.m_dequeueCount[curdemsgid],receiver);
			   
			   SYS_DBG(SYS_MESSAGE_DEBUG,printf("m_dequeueCount[%d] = %x\n",curdemsgid,SysMsgQue.m_position.m_dequeueCount[curdemsgid]))
			   SYS_DBG(SYS_MESSAGE_DEBUG,printf("CurActiveApps = 0x%x\n",Get_CurActiveApps()))
			   if((CurrentDeQueueCount(curdemsgid)==Get_CurActiveApps())&&(CurrentDeQueueStatus(curdemsgid)!=DEQUEUE_FINISH))
				{
					SysMsgQue.m_position.m_dequeueStatus[curdemsgid] = DEQUEUE_FINISH;
					CLEAR(SysMsgQue.m_position.m_dequeueCount[curdemsgid]);

					CLEAR_BIT(SysMsgQue.m_flag,curdemsgid);
					INCREASE(SysMsgQue.m_position.m_dequeuePosition,MAX_MESSAGEQUEUE_SIZE);
					SetGurDeQueueMessageID(SysMsgQue.m_position.m_dequeuePosition);
					SysMsgQue.m_current_size--;
					SYS_DBG(SYS_MESSAGE_DEBUG,printf("Dequeue KeyPress is successful\n"))
				}
			}
			else
			{
				*msgID = MSG_MAX;

				*param = Invalidparam;
			}
		  }
	 }//BroadCast method
	 else
	 {
	 	 if(CurrentDeQueueStatus(curdemsgid) !=DEQUEUE_FINISH)
	 	 {
		   	if(SysMsgQue.m_ptrMessage[curdemsgid].param.msg.to == receiver)
		   	{
		   		SYS_DBG(SYS_MESSAGE_DEBUG,printf("receiver is %d\n",receiver))
					
		   		*msgID = SysMsgQue.m_ptrMessage[curdemsgid].msgID;
			    *param = SysMsgQue.m_ptrMessage[curdemsgid].param;

				SysMsgQue.m_position.m_dequeueStatus[curdemsgid] = DEQUEUE_FINISH;
				CLEAR(SysMsgQue.m_position.m_dequeueCount[curdemsgid]);

				CLEAR_BIT(SysMsgQue.m_flag,curdemsgid);
				INCREASE(SysMsgQue.m_position.m_dequeuePosition,MAX_MESSAGEQUEUE_SIZE);
				SetGurDeQueueMessageID(SysMsgQue.m_position.m_dequeuePosition);
				SysMsgQue.m_current_size--;
				SYS_DBG(SYS_MESSAGE_DEBUG,printf("Dequeue is successful\n"))
		   		
		   	}
	 	 }
	 }

	  Mutex_UnLock(SysMsgQue.m_mutex);
	  
	}

	return 0;
	
}
























































