#ifndef __GOBAL_DEFINE_H__
#define __GOBAL_DEFINE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <malloc.h>
#include <asm/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include "Debug.h"

//WIN32 Defetion
typedef unsigned short   UINT;
typedef unsigned short	 WORD;
typedef unsigned int	 DWORD;
typedef unsigned int     LONG;
typedef unsigned char    BOOL;
//
typedef unsigned char    UINT8;
typedef unsigned short   UINT16;
typedef unsigned int	 UINT32;

typedef  char    		 INT8;
typedef  short   		 INT16;
typedef  int	 		 INT32;
typedef  unsigned char   TIMERID;
typedef  unsigned char   MUTEXID;


typedef enum{
	APP_CAMERA_CAPTURE = 0,
	APP_SOCKET_CLIENT  = 1,
	APP_SOCKET_SERVER  = 2,
	APP_MANAGER		   = 3,
	APP_MAX			   = APP_MANAGER+1,	
	
}THREADAPP;

typedef enum{
	SYS_KEY           =APP_MAX+1,
	SYS_TIMER,
	SYS_MAX		      =SYS_TIMER+1,
}SYSAPP;


typedef enum{
	MSG_APP_ACTIVE,
	MSG_APP_DEACTIVE,
	MSG_KEY_PRESS,
	MSG_APP_COMMAND,
	MSG_MAX,
}MessageID;


typedef enum{
	CAPTURE_IS_OK = 1,
	SEND_IS_OK,
	RECE_IS_OK,
	NOTIFY_CONNECT,
	RECEIVE_DATA,
	COMMAND_MAX,
}COMMAND;


typedef struct MessageParameter{

	UINT8  u8[16];
	UINT16 u16[8];
	UINT32 u32[4];
	
	struct{
		THREADAPP from;
		THREADAPP to;
	}active;
	
	struct{
		THREADAPP from;
		THREADAPP to;
	}deactive;

	COMMAND command;

	struct{
		THREADAPP from;
		THREADAPP to;
	}msg;
}MessageParameter;




#define TRUE				1
#define FALSE				0
#define UNUSED		        0
#define	USED			    1

#define CAMERA_WIDTH   		320
#define CAMERA_HEIGHT  		240

#define BMP_SIZE			(CAMERA_WIDTH*CAMERA_HEIGHT*3)  //640*480*3

#define TEMP_BMP_FILE		"temp_file.bmp"

#define UINT16_HIGH_BYTE(x)     (unsigned char)(x&0x00FF)
#define UINT16_LOW_BYTE(x)		(unsigned char)((x&0xFF00)>>8)

#define CLEAR(X) memset(&(X),0,sizeof(X))

#define sleep_second(x)	        sleep(x)
#define sleep_ms(x)				usleep(x * 1000)
#define sleep_us(x)				usleep(x)

#define SET_BIT(n,count)			n =((1<<count)|(n))
#define CLEAR_BIT(n,count)          n =(~(1<<count))&(n)
#define GET_BIT(n,count)			((1<<count)&(n))
#define Msg(str)				fprintf(stderr,"File:%s Line:%d Function:%s\n%s\n",\
								__FILE__,__FILE__,__func__,str)

#define SERVER_IP				"192.168.1.106"
#define CLIENT_IP






#endif //__GOBAL_DEFINE_H__




































