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

//��src������̬ѧ��������������̬�������Ѿ���ֵ����ͼ��
//operation=����ñ��CV_MOP_BLACKHAT//"��ñ"CV_MOP_TOPHAT
//achorCordnate,��̬���κ����ĵ����꣨�����º˱ߵľ��룩
IplImage *getMorphologyImg(IplImage* src,int operation,int thresholdBW, int isShowResultImg=1, int achorCordnate=1);


extern int whiteLineWeight;
#endif