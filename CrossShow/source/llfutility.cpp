#include <stdio.h>

#include "../include/global_def.h"
#include "../include/llfutility.h"


 int trace_level = 2;//用于等级控制
 int log2file = 1;//是否打印到文件(1)还是屏幕(0)
static int color_line_tolerance =1;
static int line_color = BLACK;
static FILE* g_logfile=NULL; 
	SYSTEMTIME sys; 

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

////打印形如 "时间，函数名,行号==>>信息内容"
 void llf_error (int curtrace_level,char*file,int line,const char *fmt, ...)
{
	char* filename = strrchr(file, '\\') + 1;
	if (curtrace_level>trace_level)return;
	FILE* pFile=NULL;

	if (log2file)
	{
		pFile=g_logfile;
		pFile = fopen("log.txt","a");
	}
	else
	{
		pFile = stderr;
	}
	va_list vl;
	va_start(vl, fmt);
	if(curtrace_level==1)fprintf(pFile,"%s,line:%d ",filename,line);
	if (curtrace_level==2)fprintf(pFile,__TIME__"=>");
    
	vfprintf(pFile, fmt, vl);
	va_end(vl);
	fflush(pFile);
	if (log2file)
	{
		fclose(pFile);
	}
	
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
			log_process("i modcolor_span,i=%d\n",i);
			if(cur_color.val[0] == black.val[0])
			{cur_color = white;
			log_process("cur_color==white\n");
			}
			else if(cur_color.val[0] == white.val[0])
			{cur_color = black;
			log_process("cur_color==black\n");
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
	cvAddS(imgToShow,cvScalar(60),imgToShow);
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

	dst_cvsize.width = (int)src->width * scale;		//目标图像的宽为源图象宽的scale倍
	dst_cvsize.height = (int)src->height * scale;	//目标图像的高为源图象高的scale倍

	dst = cvCreateImage( dst_cvsize, src->depth, src->nChannels);	//构造目标图象
	cvResize(src, dst, CV_INTER_LINEAR);	//缩放源图像到目标图像
	return dst;
	//cvReleaseImage(&dst);
}
//需手释
IplImage * g_CreateCrossImage(CvSize size,int spotSideLen/*正方形边长*/,int pitch)
{
	IplImage *blackimg = cvCreateImage(size,8,1);
	cvSet(blackimg,cvScalar(0));
	for (int i=0;i<blackimg->height;++i)
	{
		char* pdata = blackimg->imageData+i*blackimg->widthStep;
		for(int j=0;j<blackimg->width;++j)
		{
			if(j%pitch==0&&i%pitch==0)
			{
				cvSetImageROI(blackimg,cvRect(j,i,spotSideLen,spotSideLen));
				cvAddS(blackimg,cvScalar(255),blackimg);
				cvResetImageROI(blackimg);
			}
		}
	}
	return blackimg;

}


// 显示矩阵
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
				printf("%19.13f ", (float) cvGetReal2D( A, i, j ));
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
void printVecPoint(vectorPoint &vec)
{
	int len = vec.size();
	for (int i=0;i<len;i++)
	{
		log_process("(%d,%d)\n",vec[i].x,vec[i].y);
	}
	
}
//返回以lineimg中line位置覆盖下的两图像之和,以line为mask
int getSumOfLineMask(IplImage* src,IplImage* lineImg)
{
	IplImage *rltimg = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
	cvSet(rltimg,cvScalar(0));
	cvAdd(src,lineImg,rltimg,lineImg);
	int sum = (int)cvSum(rltimg).val[0];
	cvReleaseImage(&rltimg);
	return  sum;
}
void PrintTime(char* info)
{
	GetLocalTime( &sys ); 
	printf("%s...",info);
	printf( " %02d:%02d:%02d.%03d \n",sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 

}
