#ifndef __VIDEO_CAPTURE_H__
#define __VIDEO_CAPTURE_H__

#include "GobalDefine.h"








struct buffer{
	void* start;
	size_t length;
};
int GetVideoFd(void);
int InitVideoCapture(void);
void StartCapture(void);
void EndCapture(void);
int SaveOneFrameByBmp(void);

#endif   //__VIDEO_CAPTURE_H__




















































