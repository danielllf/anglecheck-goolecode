#ifndef _DO_PROCESS_
#define _DO_PROCESS_
#include<cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include <list>
int getLinePitchProcess(IplImage &src);


int getSumOfLineMask(IplImage* src,IplImage* lineImg);

//set line white from ref the line num in linelist
void setImgLine(IplImage* src,std::list<int> linelist);
#endif