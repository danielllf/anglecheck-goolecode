//有关统计算法的相关函数
#include<cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <vector>

#ifndef _GLOBAL_H
#define _GLOBAL_H
/*****************以下为结构体定义********************************/

enum{
	WHITE = 255,
	BLACK = 0
};

#define debug_ShowTime 
//#define showLineGroup 
#define  ShowLineSect 
typedef std::vector<CvPoint> vectorPoint;
typedef std::vector<int> vectorInt;
/*****************以下为变量声明********************************/


extern int WHITE_PART;

#endif