#ifndef PREPARE_H_
#define PREPARE_H_
#include<cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "global_def.h"
#define TARGETPOS_LEFT 0
#define TARGETPOS_MIDDLE 1
int getX_cordinateofVerticalBar(IplImage *bineryImg,int imgPitch,float verticalbarTimesOfpictch,int targetPos, int y_cordinate=0,int x_cordStart=0,bool showResultMark=false);

//对src进行形态学操作，并返回形态操作后并已经二值化的图像
//operation=“黑帽”CV_MOP_BLACKHAT//"白帽"CV_MOP_TOPHAT
//achorCordnate,形态方形核中心的坐标（距上下核边的距离）
IplImage *getMorphologyImg(IplImage* src,int operation,int thresholdBW, int isShowResultImg=1, int achorCordnate=1);


extern int whiteLineWeight;
#endif