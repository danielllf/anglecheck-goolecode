#include <stdio.h>

#include "global.h"
#include "utility.h"

 int trace_level = 0;//用于trace函数等级控制
static int color_line_tolerance =1;
static int line_color = BLACK;



void PrintMat(CvMat* A)
{
	int i,j;
	//printf("\nMatrix = :");
	for(i=0;i<A->rows;i++)
	{
		printf("\n");

		switch( CV_MAT_DEPTH(A->type) )
		{
		case CV_32F:
		case CV_64F:
			for(j=0;j<A->cols;j++)
				printf("%9.3f ", (float) cvGetReal2D( A, i, j ));
			break;
		case CV_8U:
		case CV_16U:
			for(j=0;j<A->cols;j++)
				printf("%6d",(int)cvGetReal2D( A, i, j ));
			break;
		default:
			break;
		}
	}
	printf("\n");
}

//只用于cv_8u1单通道
void PrintMat2(Mat* m)
{
	assert(m->channels() == 1 );
	int width = m->size().width;
	int height = m->size().height;

	for(int i = 0;i<height;i++)
	{
		uchar *pdata = m->data + i*m->step;
		for (int j = 0;j<width;j++)
		{
			printf("%2d",(pdata[j]));
		}
		printf("=>%d\n",i);
	}
}

//
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
		else if (!strcmp(cmd, "JitterTolerance")) 
		{
			get_arg(arg, sizeof(arg), &p);
			val = atoi(arg);
			if (val<0) {
				llf_error("Invalid JitterTolerance: %s\n", arg);
				return -1;
			}
			jitter_tolerance = val;
		}
		else if(!strcmp(cmd,"SlideTolerance"))
		{
				get_arg(arg, sizeof(arg), &p);
				val = atoi(arg);
				if (val<0)
				{
					llf_error("Invalid SlideTolerance:%s\n",arg);
					return -1;
				}
				slide_tolerance = val;
		}
		else if(!strcmp(cmd,"StartLineType"))
		{
			get_arg(arg, sizeof(arg), &p);
			val = atoi(arg);
			if (val<0||val>1)
			{
				llf_error("Invalid StartLineType:%s\n",arg);
				return -1;
			}
			start_line_type = val;
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