#ifndef __APP_SOCKET_SERVER_H__
#define __APP_SOCKET_SERVER_H__

#include "ThreadBase.h"
#include "SocketClient.h"
#include "SocketServer.h"








ThreadApp* Get_pSrApp(void);
void SocketServer_Initialize(void);
void*SocketServerThread(void *arg);


















#endif //__APP_SOCKET_SERVER_H__





