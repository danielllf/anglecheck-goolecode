#include <stdio.h>

#include "global.h"
#include "utility.h"

 int trace_level = 0;//用于trace函数等级控制
static int color_line_tolerance =1;
static int line_color = BLACK;
static FILE* g_logfile=NULL; 

void initParms()
{ 
	g_logfile = fopen("log.txt","w");
	parse_configfile("../cvtest/calangle.conf");

}

void mklinecolor(Mat *m, const int mid_line_num,const int tolerance,const int color)
{
	assert(m->channels()==1);
	int width = m->size().width;
	int height = m->size().height;
	int start_line, end_line;

	//以下防止加公差后超出范围
	int tmp_tolerance = tolerance;
	for(;;)
	{
		start_line = mid_line_num - tmp_tolerance;
		if (start_line>=0)break;
		else
			tmp_tolerance--;
	}
	tmp_tolerance = tolerance;
	while(1)
	{
		end_line = mid_line_num + tmp_tolerance;
		if (end_line<height)break;
		else
			tmp_tolerance--;
	}

  //black all lines within mid_line_num+/-tolerance
	for(int i =start_line;i<=end_line;i++)
	{
		uchar *pdata = m->data + i*m->step;
		for (int j = 0;j<width;j++)
		{
			pdata[j] = color;//color this line
		}
	}

}
//打印形如 "时间，函数名,行号==>>信息内容"
void llf_error(const char *fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);

	fprintf(stderr,"Time:"__DATE__","__TIME__",Func:"__FUNCTION__",Line:%05d==>> #erro#:", __LINE__);
	vfprintf(stderr, fmt, vl);

	va_end(vl);
}


void trace(int level, const char *fmt, ...)
{
	if (0 == level) return;

	va_list vl;
	va_start(vl, fmt);
	 if(2 == level)
		fprintf(stderr,"Time:"__DATE__","__TIME__",Func:"__FUNCTION__",Line:%05d==>> ", __LINE__);
	vfprintf(stderr, fmt, vl);

	va_end(vl);
}
static void get_arg(char *buf, int buf_size, const char **pp)
{
	const char *p;
	char *q;
	int quote;

	p = *pp;
	while (isspace(*p)) p++;
	q = buf;
	quote = 0;
	if (*p == '\"' || *p == '\'')
		quote = *p++;
	for(;;) {
		if (quote) {
			if (*p == quote)
				break;
		} else {
			if (isspace(*p))
				break;
		}
		if (*p == '\0')
			break;
		if ((q - buf) < buf_size - 1)
			*q++ = *p;
		p++;
	}
	*q = '\0';
	if (quote && *p == quote)
		p++;
	*pp = p;
}


int parse_configfile(const char* filename)
{
	FILE *f;
	char line[1024];
	char cmd[64];
	char arg[1024];
	const char *p;
	int val;

	f = fopen(filename, "r");
	if (!f) {
		perror(filename);
		return -1;
	}

	for(;;) {
		if (fgets(line, sizeof(line), f) == NULL)
			break;
		p = line;
		while (isspace(*p))
			p++;
		if (*p == '\0' || *p == '#')
			continue;

		get_arg(cmd, sizeof(cmd), &p);

		if (!strcmp(cmd, "TraceLevel")) {
			get_arg(arg, sizeof(arg), &p);
			val = atoi(arg);
			if (val < 0 || val > 2) {
				llf_error("Invalid TraceLevel: %s\n", arg);
				return -1;
			}
			trace_level = val;
		}
		else if(!strcmp(cmd,"ColorLineTol"))
		{
			get_arg(arg, sizeof(arg), &p);
			val = atoi(arg);
			if (val<0)
			{
				llf_error("Invalid ColorLineTol:%s\n",arg);
				return -1;
			}
			color_line_tolerance = val;
		}
		else if(!strcmp(cmd,"LineColor"))
		{
			get_arg(arg, sizeof(arg), &p);
			val = atoi(arg);
			if (val<0||val>255)
			{
				llf_error("Invalid LineColor:%s\n",arg);
				return -1;
			}
			line_color = val;
		}


	}

	fclose(f);
	return 0;
}

void formatImg(IplImage* img,int pitch, int line_thick)
{
	using namespace cv;
	CvScalar  black = cvScalar(0);
	CvScalar white = cvScalar(255);
	CvScalar cur_color = black;

	int color_span = 10*pitch;
	int shift_cnt = img->height/color_span;
	for (int i=0;i<img->height;i++)
	{
		if(i%pitch!=0)continue;
		if(i%color_span==0)
		{
			llf_error("i modcolor_span,i=%d\n",i);
			if(cur_color.val[0] == black.val[0])
			{cur_color = white;
			llf_error("cur_color==white\n");
			}
			else if(cur_color.val[0] == white.val[0])
			{cur_color = black;
			llf_error("cur_color==black\n");
			}
		}

		cvLine(img,cvPoint(0,i),cvPoint(img->width,i),cur_color,line_thick);

	}
}

IplImage * g_CopyRectFromImg( IplImage *src, CvRect rect)
{
	assert(rect.width<=src->width&&rect.height<=src->height);
	IplImage *subROI = cvCreateImageHeader(
		cvSize(rect.width,rect.height),src->depth,src->nChannels);
	
	(subROI)->origin = src->origin;
	(subROI)->widthStep = src->widthStep;
	subROI->imageData =  src->imageData + rect.y*src->widthStep +	rect.x;
 
	IplImage * subSrc = cvCreateImage(cvSize(subROI->width,subROI->height),subROI->depth,subROI->nChannels);
	cvCopy(subROI,subSrc);
	cvReleaseImageHeader(&subROI);
	return subSrc;
}

void g_showImgRect(IplImage *showImgSrc, CvRect rect)
{
	IplImage *imgToShow = cvCreateImage(cvGetSize(showImgSrc),showImgSrc->depth, showImgSrc->nChannels);
	cvCopy(showImgSrc, imgToShow);
	
	cvSetImageROI(imgToShow, rect);
	cvAddS(imgToShow,cvScalar(ADD_VALUE),imgToShow);
	cvResetImageROI(imgToShow);
	IMG_SHOW("临时调试图像",imgToShow);
	cvWaitKey();
	cvReleaseImage(&imgToShow);
}
void g_getPresample(const char* filename,int startline,int height, const char* result_picName)
{
	IplImage *presample;
	if ((presample = cvLoadImage(filename,0))==NULL)printf("load src erro\n");
	cvSetImageROI(presample,cvRect(0,startline,presample->width,height));
	cvSet(presample,cvScalar(0));
	cvResetImageROI(presample);
	cvSaveImage(result_picName,presample);
	 
}
//函数外需手动释放内存
IplImage* g_resizeImage(IplImage* src,float scale)
{

	IplImage *dst = 0;			//目标图像指针
	CvSize dst_cvsize;			//目标图像尺寸

	dst_cvsize.width = src->width * scale;		//目标图像的宽为源图象宽的scale倍
	dst_cvsize.height = src->height * scale;	//目标图像的高为源图象高的scale倍

	dst = cvCreateImage( dst_cvsize, src->depth, src->nChannels);	//构造目标图象
	cvResize(src, dst, CV_INTER_LINEAR);	//缩放源图像到目标图像
	return dst;
	//cvReleaseImage(&dst);
}