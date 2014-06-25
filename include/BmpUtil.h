#ifndef __BMP_Util_H__
#define __BMP_Util_H__

#include "GobalDefine.h"

#define RGB24COUNT		24


#pragma pack(1)
typedef struct tagBITMAPFILEHEADER {  
	UINT  bfType;  
	DWORD bfSize; 
	UINT  bfReserved1; 
	UINT  bfReserved2; 
	DWORD bfOffBits;
} BITMAPFILEHEADER; 


typedef struct tagBITMAPINFOHEADER { 
	DWORD biSize; 
	LONG  biWidth; 
	LONG  biHeight; 
	WORD  biPlanes; 
	WORD  biBitCount; 
	DWORD biCompression; 
	DWORD biSizeImage; 
	LONG  biXPelsPerMeter; 
	LONG  biYPelsPerMeter; 
	DWORD biClrUsed; 
	DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagBMPHEADER{
	BITMAPFILEHEADER  header;
	BITMAPINFOHEADER  info;
}tagBMPHEADER ;
#pragma pack()

void InitBmpHeader(tagBMPHEADER* pHeader);
void SaveBmpFile(unsigned char *pbuffer,unsigned int buffer_len,tagBMPHEADER header, const char* PathName);
void OpenBmpFile(const char* PathName);
void CloseBmpFile(const char* PathName);
int ReadBmpFile(const char* PathName,unsigned char *pbuffer);




#endif // __BMP_HEADER_H__