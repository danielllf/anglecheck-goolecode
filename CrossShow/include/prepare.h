#ifndef PREPARE_H_
#define PREPARE_H_
#include<cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "global_def.h"
#define TARGETTYPE_LEFT 0
#define TARGETTYPE_MIDDLE 1
#define TARGETTYPE_RIGHT 2
struct CORDINATE_PAIR 
{
	int left_cordinate;
	int right_cordinate;
};
CORDINATE_PAIR getX_cordinateofVerticalBar(IplImage *bineryImg,int imgPitch,float verticalbarTimesOfpictch,int subImageType, int y_cordinate=0,int x_cordStart=0,bool showResultMark=false);

//��src������̬ѧ��������������̬�������Ѿ���ֵ����ͼ��
//operation=����ñ��CV_MOP_BLACKHAT//"��ñ"CV_MOP_TOPHAT
//achorCordnate,��̬���κ����ĵ����꣨�����º˱ߵľ��룩
IplImage *getMorphologyImg(IplImage* src,int operation,bool isAdaptiveThres=true,int thresholdBW=10, int achorCordnate=1);


extern int whiteLineWeight;
#endif