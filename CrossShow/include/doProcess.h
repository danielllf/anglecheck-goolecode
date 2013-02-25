#ifndef _DO_PROCESS_
#define _DO_PROCESS_
#include<cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
int getLinePitch(IplImage &src);


int getSumOfLineMask(IplImage* src,IplImage* lineImg);

#endif