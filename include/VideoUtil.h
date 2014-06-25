#ifndef __VIDEO_UTIL_H__
#define __VIDEO_UTIL_H__

#include "GobalDefine.h"


void YUV422ToRGB24(unsigned char *yuv422,unsigned char *rgb24, int width, int height);
void YUV422ToRGB565(unsigned char *yuv422,unsigned char *rgb565, int width, int height);




#endif  //__VIDEO_UTIL_H__