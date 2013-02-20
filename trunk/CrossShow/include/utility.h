#ifndef _UTILITY_H
#define _UTILITY_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "global.h"
using namespace cv;


/*****************����ΪDEFINE********************************/
#define LLF_PRINTF  

extern FILE* g_logfile;
extern struct PenParam penParam;

#ifdef LLF_PRINTF  
#define llf_printf(format,...) fprintf(g_logfile,"Func:"__FUNCTION__", Line: %04d=> "format, __LINE__, ##__VA_ARGS__) ;\
			fflush(g_logfile);
#else  
#define llf_printf(format,...)  
#endif 

/*****************����Ϊ��������********************************/

extern int trace_level;
extern int line_color;
extern int color_line_tolerance;


/*****************����Ϊ�ṹ�嶨��********************************/

enum{
	WHITE = 255,
	BLACK = 0
};






/*****************����Ϊ��������********************************/

#define IMG_SHOW(name,src) \
	cvNamedWindow(#name);\
	cvShowImage(#name,src);

/*
*if runs on x86,convert 64 bits float  to int32. 
*else convert to int with stand C method
*val:int var to hold the result
*dval:doube var to be operatored,�����h���빦��
*/
inline void RoudToInt(int &val,double dval)
{
#if defined(__WIN32__) || defined(_WIN32)

	static double magic = 6755399441055744.0;
	dval += magic;
	val = *(int*)&dval;
#else
	val = (int)dval;
#endif
}

//���ٴ�ӡ
//������������
//level ==0:�رո���
//level ==1:ֻ��ӡϢ����������ӡʱ�䣬�ļ������кţ���������
//level ==2:��ӡ���� "ʱ�䣬������,�к�==>>��Ϣ����"
void trace(int level, const char *fmt, ...);

//��trace���ƣ�ֻ��û����ȼ�����
 void llf_error (const char *fmt, ...);


//color all lines within line_num+/-tolerance
void mklinecolor(Mat *m, const int mid_line_num,const int tolerance,const int color);


//�������ò����ļ�
int parse_configfile(const char* filename);

//����������ͼ��
void formatImg(IplImage* img,int pitch, int line_thick=1);

//��ָ��ͼ��src��rect�����п���ͼ��.������
//�����ڲ�����ͼ��ռ䣬����ʹ�����Ӧ����cvReleaseImge�ֶ��ͷŷ��ص�ͼ��ָ��
//rect:λ��Դͼ��src�еľ�������
IplImage * g_CopyRectFromImg( IplImage *src, CvRect rect);

//this func only used to debug
//rect: rect to be show on img
//img: the src imge(would not be affected) which is the source of the showed img
//��ʾ��ͼƬ�ڴ˺����˳�ǰ���Զ����٣��������ͷ�
void g_showImgRect(IplImage *showImgSrc, CvRect rect);

/**
*��ʼ���������ò������������ļ�����
*/
void initParms();

//��sample��ȡpresampleͼ
//picName:���ɵ�ͼƬ����
//filename:sample·��
void g_getPresample(const char* filename,int startline,int height, const char* result_picName);

//����ͼ��
IplImage* g_resizeImage(IplImage* src,float scale);
#endif