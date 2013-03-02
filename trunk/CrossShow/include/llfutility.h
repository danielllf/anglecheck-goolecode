#ifndef LLF_UTILITY_H
#define LLF_UTILITY_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "global_def.h"

using namespace cv;



/*****************以下为变量声明********************************/

extern int trace_level;
extern int line_color;
extern int color_line_tolerance;
extern SYSTEMTIME sys; 

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


//color all lines within line_num+/-tolerance
void mklinecolor(Mat *m, const int mid_line_num,const int tolerance,const int color);


//解析配置参数文件
int parse_configfile(const char* filename);

//用线来划分图像
void formatImg(IplImage* img,int pitch, int line_thick=1);

//从指定图像src的rect区域中拷贝图像.并返回
//因在内部分配图像空间，所以使用完后应该用cvReleaseImge手动释放返回的图像指针
//rect:位于源图像src中的矩形区域
IplImage * g_CopyRectFromImg( IplImage *src, CvRect rect);

//this func only used to debug
//rect: rect to be show on img
//img: the src imge(would not be affected) which is the source of the showed img
//显示的图片在此函数退出前会自动销毁，无需再释放
void g_showImgRect(IplImage *showImgSrc, CvRect rect);

/**
*初始化所有配置参数，由配置文件读入
*/
void initParms();

//从sample获取presample图
//picName:生成的图片名字
//filename:sample路径
void g_getPresample(const char* filename,int startline,int height, const char* result_picName);

//缩放图像
IplImage* g_resizeImage(IplImage* src,float scale);

//需手动释放内存
IplImage * g_CreateCrossImage(CvSize size,int spotSideLen/*正方形边长*/,int pitch);

//独立的函数打印二维数组到指定文件
void PrintMat(CvMat* A);
void printVecPoint(vectorPoint &vec);
#endif