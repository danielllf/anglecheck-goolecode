//�й�ͳ���㷨����غ���
#include<cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <vector>

#ifndef _GLOBAL_H
#define _GLOBAL_H
/*****************����Ϊ�ṹ�嶨��********************************/

enum{
	WHITE = 255,
	BLACK = 0
};

#define debug_ShowTime 
//#define showLineGroup 
#define  ShowLineSect 
typedef std::vector<CvPoint> vectorPoint;
typedef std::vector<int> vectorInt;
/*****************����Ϊ��������********************************/


extern int WHITE_PART;

#endif