#ifndef LLF_UTILITY_H
#define LLF_UTILITY_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "global_def.h"

using namespace cv;



/*****************����Ϊ��������********************************/

extern int trace_level;
extern int line_color;
extern int color_line_tolerance;
extern SYSTEMTIME sys; 

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

//���ֶ��ͷ��ڴ�
IplImage * g_CreateCrossImage(CvSize size,int spotSideLen/*�����α߳�*/,int pitch);

//�����ĺ�����ӡ��ά���鵽ָ���ļ�
void PrintMat(CvMat* A);
void printVecPoint(vectorPoint &vec);
#endif