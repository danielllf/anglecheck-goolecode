
#ifndef _TEMPLATE_H
#define _TEMPLATE_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>

//匹配方法如下：
//CV_TM_SQDIFF        =0,平方差匹配
//CV_TM_SQDIFF_NORMED =1,归一化平方差匹配
//CV_TM_CCORR         =2,相关匹配
//CV_TM_CCORR_NORMED  =3,
//CV_TM_CCOEFF        =4,相关匹配二
//CV_TM_CCOEFF_NORMED =5
//将temp图块在srcImg中匹配，当找到匹配时，返回匹配点。否则返回（-1，-1）。
//平方差为优先匹配法。为了加快速度可将nMethods设为3,但误差可能会提高。
//使用计算归一化法（区别于cvNormalize,this is just for display）可提高匹配精度，但计算量更大
//priorityMethod=0,2,4
CvPoint findMatchPoint( IplImage* srcImg, IplImage *temp, int nMethods=3, int priorityMethod=CV_TM_SQDIFF, bool useNormed=true);


#endif
