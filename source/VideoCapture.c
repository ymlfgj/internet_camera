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
#include <linux/videodev2.h>

#include "BmpUtil.h"
#include "VideoUtil.h"
#include "VideoCapture.h"
#include "GrEngine.h"




static const char* dev_name = "/dev/video2";
static int video_fd = -1;


static struct buffer* buffers=NULL;
static unsigned int n_buffers = 0;

unsigned char bmp_buffer[BMP_SIZE];
unsigned int  file_length;


static tagBMPHEADER BmpHeader;


int GetVideoFd(void)
{
	if(video_fd!=(-1))
		return video_fd;

	return (-1);
}

static void SetVideoFd(int fd)
{
	video_fd = fd;
}


int InitVideoCapture(void)
{
	int Video_fd;
	struct v4l2_capability cap;
	unsigned int i;
//	enum v4l2_buf_type type;

	Video_fd = open(dev_name,O_RDWR|O_NONBLOCK,0);

	if(ioctl(Video_fd,VIDIOC_QUERYCAP,&cap)==-1){
		perror("failure VIDIOC_QUERYCAP:");
	}
	else
	{
		printf("driver=%s\n",cap.driver);
		printf("card=%s\n",cap.card);
		printf("bus_info=%s\n",cap.bus_info);
		printf("capabilities=%x\n",cap.capabilities);
	}

	struct v4l2_fmtdesc fmtdesc;
	CLEAR(fmtdesc);
	
	fmtdesc.index = 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	while(ioctl(Video_fd,VIDIOC_ENUM_FMT,&fmtdesc)==0)
	{
		fmtdesc.index++;
		printf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",\
															 fmtdesc.pixelformat & 0xFF, (fmtdesc.pixelformat >> 8) & 0xFF,\
															(fmtdesc.pixelformat >> 16) & 0xFF, (fmtdesc.pixelformat >> 24) & 0xFF,\
															fmtdesc.description);
	}

	struct v4l2_format fmt;
	CLEAR(fmt);

	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(ioctl(Video_fd,VIDIOC_G_FMT,&fmt) == -1){
		perror("failure VIDIOC_G_FMT:");
	}
	printf("Camera support: width = %d  height = %d \n",fmt.fmt.pix.width,fmt.fmt.pix.height);
	
	fmt.fmt.pix.width =  CAMERA_WIDTH;
	fmt.fmt.pix.height = CAMERA_HEIGHT;
//	fmt.fmt.pix.pixelformat  = V4L2_PIX_FMT_MJPEG;
	fmt.fmt.pix.pixelformat  = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	if(ioctl(Video_fd,VIDIOC_S_FMT,&fmt) == -1){
		perror("failure VIDIOC_S_FMT:");
	}

	file_length = fmt.fmt.pix.width * fmt.fmt.pix.height;

	struct v4l2_control control;
	CLEAR(control);
	
	control.id = V4L2_CID_BRIGHTNESS;
	if(ioctl(Video_fd,VIDIOC_G_CTRL,&control) == -1)
	{
		perror("Failure VIDIOC_G_CTRL!");
	}
	printf("V4L2_CID_BRIGHTNESS=%d\n",control.value);
	
	control.value = 0;
	if(ioctl(Video_fd,VIDIOC_S_CTRL,&control) == -1)
	{
		perror("Failure VIDIOC_S_CTRL!");
	}
	

	struct v4l2_requestbuffers req;
	CLEAR(req);
	
	req.count  =  1;
	req.type   =  V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory =  V4L2_MEMORY_MMAP;
	if(ioctl(Video_fd,VIDIOC_REQBUFS, &req) == -1){
		perror("failure VIDIOC_REQBUFS:");
	}


	buffers = calloc(req.count,sizeof(*buffers));
	
	for(n_buffers = 0; n_buffers < req.count; n_buffers++)
	{
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type =  V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		
		if(ioctl(Video_fd,VIDIOC_QUERYBUF,&buf) == -1){
			perror("failure VIDIOC_QUERYBUF:");
		}
	
		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start = mmap(NULL,buf.length,PROT_READ | PROT_WRITE ,MAP_SHARED ,Video_fd, buf.m.offset);

		if(MAP_FAILED == buffers[n_buffers].start){
			perror("failure mmap:");
		}
	}


	for(i = 0;i < n_buffers; i++)
	{
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type =  V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		
		if(ioctl(Video_fd,VIDIOC_QBUF,&buf) == -1){
			perror("failure VIDIOC_QBUF:");
		}
		
	}

	SetVideoFd(Video_fd);

	return Video_fd;

	
}

void StartCapture(void)
{
	int fd = GetVideoFd();
	
	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	InitBmpHeader(&BmpHeader);
		
	if(ioctl(fd, VIDIOC_STREAMON, &type) == -1){
	   perror("failure VIDIOC_STREAMON:");
	}
	
}


int SaveOneFrameByBmp(void)
{
	int fd = GetVideoFd();
	
	struct v4l2_buffer buf;
	CLEAR(buf);
	
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory =  V4L2_MEMORY_MMAP;
	
	if(ioctl(fd,VIDIOC_DQBUF,&buf) == -1){
		perror("Failure!");
	}

//	YUV422ToRGB24(buffers[buf.index].start,bmp_buffer,CAMERA_WIDTH,CAMERA_HEIGHT);
//	SaveBmpFile(bmp_buffer,BMP_SIZE,BmpHeader,TEMP_BMP_FILE);
//	YUV422ToRGB565(buffers[buf.index].start, bmp_buffer,CAMERA_WIDTH,CAMERA_HEIGHT);
//	GrEngine_DrawBMP(bmp_buffer,CAMERA_WIDTH,CAMERA_HEIGHT);

	GrEngine_SDL_DrawBMP(TEMP_BMP_FILE, buffers[buf.index].start );
	
	
	CLEAR(bmp_buffer);
	ioctl(fd,VIDIOC_QBUF,&buf);
	
	return 1;
}

void EndCapture(void)
{
	int fd = GetVideoFd();
	
	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(ioctl(fd, VIDIOC_STREAMOFF, &type) == -1){
	   perror("failure VIDIOC_STREAMOFF:");
	}
	if(fd != -1)
	{
		close(fd);
	}
	
}


































