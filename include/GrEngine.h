#ifndef __GR_ENGINE_H__
#define __GR_ENGINE_H__

#define MODE_MAX        3
typedef enum{
	RGB888,
	RGB565,
	RGB32,
	RGB8,
	RGBMAX,
}COLOR_MODE;

typedef struct POSTION{
	int x;
	int y;
}POSTION;

typedef struct RECT{
	int left;
	int right;
	int top;
	int bottom;
}RECT;


typedef struct RESOLUTION{
	int resx;
	int resy;
    unsigned char bits_per_pixel;
	unsigned char color_width;
}RESOLUTION;


typedef struct LCD_PARAM{
	RESOLUTION res;
	int screen_size;
	int line_size;
	COLOR_MODE color_mode;
	char *fbp;
}LCD_PARAM;


void GrEngine_Initialize(void);
void GrEngine_DrawPoint(POSTION pos,int color);
void GrEngine_DrawLine(POSTION start, POSTION end,int color);
void GrEngine_FillRect(RECT rect,int color);
void GrEngine_DrawBMP(unsigned char * pBuffer,unsigned int width ,unsigned int height);
void GrEngine_Finalize(void);
void GrEngine_SDL_DrawBMP(char* filename,unsigned char*buffer);






































#endif //__GR_ENGINE_H__






