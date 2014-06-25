#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <VideoUtil.h>

static unsigned short Convert_RGB24ToRGB565(int R, int G ,int B)
{
	unsigned short RGB565 = 0;

	RGB565 = RGB565|((R&0xF8)<<8);  //R
	
	RGB565 = RGB565|((G&0xE0)<<3);  //G
	RGB565 = RGB565|((G&0x1C)<<3);  //G

	RGB565 = RGB565|((B&0x1F)<<0);  //B

	return RGB565;
}



void YUV422ToRGB24(unsigned char *yuv422,unsigned char *rgb24, int width, int height)
{
	int R,G,B;
	int Y,U,V;
	int x,y;
	int nwidth = width<<1;
	
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			if(x%2==0)
			{
				Y = yuv422[y*nwidth+2*x];
				U = yuv422[y*nwidth+2*x+1];
				V = yuv422[y*nwidth+2*x+3];
			}
			else
			{
				Y = yuv422[y*nwidth+2*x];
				U = yuv422[y*nwidth+2*x-1];
				V = yuv422[y*nwidth+2*x+1];
			}
			
			R = Y + 1.402*(V-128);
			G = Y - 0.34414*(U-128) - 0.71414*(V-128);
			B = Y + 1.772*(U-128);
			
			if(R>255) R = 255;
			if(R<0)	R = 0;
			if(G>255) G = 255;
			if(G<0)	G = 0;
			if(B>255) B = 255;
			if(B<0)	B = 0;
			
			*(rgb24+((height-1-y)*width+x)*3) = B;
			*(rgb24+((height-1-y)*width+x)*3+1) = G;
			*(rgb24+((height-1-y)*width+x)*3+2) = R;
		}
	}
}

void YUV422ToRGB565(unsigned char *yuv422,unsigned char *rgb565, int width, int height)
{
	int R,G,B;
	int Y,U,V;
	int x,y;
	int nwidth = width<<1;
	unsigned short rgb_565;
	
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			if(x%2==0)
			{
				Y = yuv422[y*nwidth+2*x];
				U = yuv422[y*nwidth+2*x+1];
				V = yuv422[y*nwidth+2*x+3];
			}
			else
			{
				Y = yuv422[y*nwidth+2*x];
				U = yuv422[y*nwidth+2*x-1];
				V = yuv422[y*nwidth+2*x+1];
			}
			
			R = Y + 1.402*(V-128);
			G = Y - 0.34414*(U-128) - 0.71414*(V-128);
			B = Y + 1.772*(U-128);
			
			if(R>255) R = 255;
			if(R<0)	R = 0;
			if(G>255) G = 255;
			if(G<0)	G = 0;
			if(B>255) B = 255;
			if(B<0)	B = 0;

			rgb_565 = Convert_RGB24ToRGB565(R,G,B);

			*(rgb565+((y)*width+x)*2) = UINT16_LOW_BYTE(rgb_565);
			*(rgb565+((y)*width+x)*2+1) = UINT16_HIGH_BYTE(rgb_565);

		}
	}
	
}













