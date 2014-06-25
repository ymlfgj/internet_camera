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
#include <linux/fb.h>

#include "GobalDefine.h"
#include "VideoCapture.h"
#include "GrEngine.h"
#include <SDL.h>


static const char* FrameBufferDev = "/dev/fb0";
static int lcd_fd = -1;
static LCD_PARAM lcd_param;
static SDL_Surface* pScreen;
static SDL_Overlay* pBmp;

void GrEngine_Initialize(void)
{
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;

	CLEAR(vinfo);
	CLEAR(finfo);
	
	lcd_fd = open(FrameBufferDev,O_RDWR);
	if(lcd_fd == -1)
		perror("lcd_fd Failure");

	if(ioctl(lcd_fd,FBIOGET_VSCREENINFO,&vinfo) == -1)
		perror("Failure FBIOGET_VSCREENINFO");

	if(ioctl(lcd_fd,FBIOGET_FSCREENINFO,&finfo) == -1)
		perror("Failure FBIOGET_FSCREENINFO");

	lcd_param.res.resx = vinfo.xres;
	lcd_param.res.resy = vinfo.yres;
	lcd_param.res.bits_per_pixel = vinfo.bits_per_pixel;
	lcd_param.res.color_width = lcd_param.res.bits_per_pixel/8;

	if(lcd_param.res.bits_per_pixel == 16)
	{
		lcd_param.color_mode = RGB565;
	}
	else if(lcd_param.res.bits_per_pixel == 24)
	{
		lcd_param.color_mode = RGB888;
	}
	else
	{
		//
	}
	
	lcd_param.line_size = finfo.line_length;
	lcd_param.screen_size = (lcd_param.res.resx * lcd_param.res.resy) * (lcd_param.res.color_width);

	lcd_param.fbp = mmap(NULL,lcd_param.screen_size,PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0);

	memset(lcd_param.fbp,0,lcd_param.screen_size);

	GrEngine_Finalize();

//	if( SDL_Init(SDL_INIT_EVERYTHING) < 0)
//	{
//		printf("SDL_Init failed !\n");
//	}
/*
	pScreen = SDL_SetVideoMode(lcd_param.res.resx, lcd_param.res.resy , 0 , SDL_SWSURFACE);

	pBmp = SDL_CreateYUVOverlay(CAMERA_WIDTH, CAMERA_HEIGHT , SDL_YUY2_OVERLAY , pScreen);

	printf("screen width[%d] height[%d] bits_per_pixel[%d]\n",lcd_param.res.resx,lcd_param.res.resy,lcd_param.res.bits_per_pixel);
*/	
}




void GrEngine_DrawPoint(POSTION pos,int color)
{

/*
	int location = 0;
	short color_width = 0;
	int resx,resy,line_info;
	short ActualColor;

	color_width = lcd_param.res.color_width;
	resx = lcd_param.res.resx;
	resy = lcd_param.res.resy;
	line_info = lcd_param.line_size;

	if(lcd_param.color_mode == RGB565)
		 ActualColor = (short)(color&0x0000ffff);

	location = pos.x*color_width + pos.y*line_info;

	if(lcd_param.color_mode == RGB565)
	{
		*(lcd_param.fbp+location) = UINT16_LOW_BYTE(ActualColor);
		*(lcd_param.fbp+location+1) = UINT16_HIGH_BYTE(ActualColor);
	}
*/	
}

void GrEngine_FillRect(RECT rect,int color)
{
	int i,j;
	POSTION pos;

	for(i = rect.left;i<rect.right;i++)
		for(j = rect.top;j<rect.bottom;j++)
		{
			pos.x = i;
			pos.y = j;
			GrEngine_DrawPoint(pos,color);
		}
	
}

void GrEngine_DrawLine(POSTION start, POSTION end,int color)
{

}

void GrEngine_DrawBMP(unsigned char* pBuffer,unsigned int width ,unsigned int height)
{
	int location = 0;
	int screen_size = 0;

	screen_size = lcd_param.screen_size;

	int x,y;

	for(y = 0; y < height; y++)
		for(x =0; x < width; x++)
		{
			location = (y * height + x)*2;

			*(lcd_param.fbp+location ) = pBuffer[location];
			*(lcd_param.fbp+location+1 )= pBuffer[location+1];
			
		}
		
}


void GrEngine_Finalize(void)
{
	munmap(lcd_param.fbp,lcd_param.screen_size);
	close(lcd_fd);
}



void GrEngine_SDL_DrawBMP(char* filename ,unsigned char*buffer)
{
/*
	SDL_Surface* pShownBMP = 0;
	
	pShownBMP = SDL_LoadBMP(filename);
	
	SDL_Rect* pSrcRect = 0;
	SDL_Rect* pDstRect = 0;
	
	SDL_BlitSurface(pShownBMP, pSrcRect, pScreen, pDstRect);
	
	SDL_Flip(pScreen);
*/
/*
	pBmp->pixels[0] = (Uint8*)buffer;
	
	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = CAMERA_WIDTH;
	rect.h = CAMERA_HEIGHT;

	SDL_DisplayYUVOverlay(pBmp,&rect);
*/
}









