#ifndef PREPARE_H_
#define PREPARE_H_
#include<cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "global_def.h"
IplImage* getSubImgDevidedByVerticalLine(IplImage *src);

//��src������̬ѧ��������������̬�������Ѿ���ֵ����ͼ��
//operation=����ñ��CV_MOP_BLACKHAT//"��ñ"CV_MOP_TOPHAT
//achorCordnate,��̬���κ����ĵ����꣨�����º˱ߵľ��룩
IplImage *getMorphologyImg(IplImage* src,int operation,int thresholdBW, int isShowResultImg=1, int achorCordnate=1);

#endif