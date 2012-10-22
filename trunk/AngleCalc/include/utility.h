#ifndef _UTILITY_H
#define _UTILITY_H
#include <cv.h>
#include "global.h"
using namespace cv;


/*****************以下为DEFINE********************************/
#define LLF_PRINTF  

extern FILE* g_logfile;
extern struct PenParam penParam;

#ifdef LLF_PRINTF  
#define llf_printf(format,...) fprintf(g_logfile,"Func:"__FUNCTION__", Line: %04d=> "format, __LINE__, ##__VA_ARGS__) ;\
			fflush(g_logfile);
#else  
#define llf_printf(format,...)  
#endif 

/*****************以下为变量声明********************************/

extern int trace_level;
extern int line_color;
extern int color_line_tolerance;


/*****************以下为结构体定义********************************/

enum{
	WHITE = 255,
	BLACK = 0
};






/*****************以下为函数声明********************************/

#define IMG_SHOW(name,src) \
	cvNamedWindow(#name);\
	cvShowImage(#name,src);

/*
*if runs on x86,convert 64 bits float  to int32. 
*else convert to int with stand C method
*val:int var to hold the result
*dval:doube var to be operatored,带四舎五入功能
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

//跟踪打印
//设置三级跟踪
//level ==0:关闭跟踪
//level ==1:只打印息本身，而不打印时间，文件名和行号，函数名等
//level ==2:打印形如 "时间，函数名,行号==>>信息内容"
void trace(int level, const char *fmt, ...);

//与trace类似，只是没加入等级控制
 void llf_error (const char *fmt, ...);

//用来打印一个矩阵
void PrintMat(CvMat* A);

//用来打印一个矩阵,参数为Mat，只打单通道
void PrintMat2(Mat* m);

//color all lines within line_num+/-tolerance
void mklinecolor(Mat *m, const int mid_line_num,const int tolerance,const int color);


//解析配置参数文件
int parse_configfile(const char* filename);

//用线来划分图像
void formatImg(IplImage* img,int pitch, int line_thick=1);

#endif