#ifndef _DO_PROCESS_
#define _DO_PROCESS_
#include<cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include <list>
#include <vector>
#include "global_def.h"
#include "LineImage.h"
#include "prepare.h"
typedef std::vector<CORDINATE_PAIR> vecCordinate;

int getLinePitchProcess(IplImage* src);


//在查找偏移之前，先要找到作为标准的那一行。
int getTheStarLine(LineImage &lineimgObj,IplImage *src,int lineCntInGroup,int lineLen);

//返回在一组linemask下求和最大时的linenum
int getMaxLineGroupSumLineWithinTol(IplImage *src,LineImage &lineimgObj,int linePitch,int lineCntInGroup,int lineLen,CvPoint startPT,int  pitchTolInt);

//获取每一行像素的上下偏移数据。
//返回 int 型为始的作为参考的线行号
//vect 返回为一个vect，每个成员为一个线段相对于第一个线段的偏移像素个数
 int getShiftPos(IplImage *src,int linePitch,double pitchTol,int lineCntInGroup,int &vectElementCount,vectorPoint &vect);
//set line white from ref the line num in linelist
void setImgLineGroup(IplImage* src,std::list<int> linelist);

int getShiftPosProcess(IplImage* src,vectorPoint &rltvec ,int secCnt,bool isAdaptiveThres=true,int thresholdBW=10, int achorCordnate=1);

/************变量声明**************************/
extern unsigned char cmpLineLen;
extern float Purfactor ;
extern int continuesLinecount;//间距均匀的连续线的条数
extern int safeVoidpitchCycleCntIngetStartLine ;//在计算cmp starline时，去除图片最后的几个cycle,防止循环溢出。因为pitch是一个统计均值，不一定准确。
//此值如果未找到，则会从0.05开始，以台阶0.05向上递增，直到找到为止
extern float continuesTol;//（设置时<0.05）连续几条线被视为是连续且均匀时的sdv/mean,越小越好
extern int lineThickness;
extern double allowedPercentTOLwhenShifting;

#endif