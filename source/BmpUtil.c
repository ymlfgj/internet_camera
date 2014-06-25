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

#include "BmpUtil.h"

static FILE *stfile_bmp;


void InitBmpHeader(tagBMPHEADER* pHeader)
{
	pHeader->header.bfType = 0x4D42;
	pHeader->header.bfSize = BMP_SIZE;
	pHeader->header.bfReserved1 = 0;
	pHeader->header.bfReserved2 = 0;
	pHeader->header.bfOffBits = 54; //40+14;
	
	pHeader->info.biSize = 40;
	pHeader->info.biWidth = CAMERA_WIDTH;
	pHeader->info.biHeight = CAMERA_HEIGHT;
	pHeader->info.biPlanes = 1;
	pHeader->info.biBitCount = RGB24COUNT; //RGB24
	pHeader->info.biCompression = 0;
	pHeader->info.biSizeImage = (BMP_SIZE+54);
	pHeader->info.biXPelsPerMeter = 3780;
	pHeader->info.biYPelsPerMeter = 3780;
	pHeader->info.biClrUsed = 0;
	pHeader->info.biClrImportant = 0;
}




void SaveBmpFile(unsigned char *pbuffer,unsigned int buffer_len,tagBMPHEADER header, const char* PathName)
{
	int len = 0;
	
	stfile_bmp = fopen(PathName,"w+");
	
	len = sizeof(header);
	fwrite(&header,len,1,stfile_bmp);
	fwrite(pbuffer,buffer_len,1,stfile_bmp);
	fclose(stfile_bmp);
}

void OpenBmpFile(const char* PathName)
{
	if((stfile_bmp=fopen(PathName,"r"))==NULL)
	{
	   perror("Open File error");
	}
	
}

void CloseBmpFile(const char* PathName)
{
	PathName = PathName;
	
	if(stfile_bmp!= NULL)
		fclose(stfile_bmp);
		
}

int ReadBmpFile(const char* PathName,unsigned char *pbuffer)
{
	int nRead = 0;
	int len = 0;
	if(pbuffer!=NULL)
	{
	  len = sizeof(pbuffer);
	  memset(pbuffer,0,len);
	}

	if(stfile_bmp!=NULL)
	{
		nRead = fread(pbuffer,1,len,stfile_bmp);
	}
	

	return nRead;
	
}












