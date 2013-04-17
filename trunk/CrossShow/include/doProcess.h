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


//�ڲ���ƫ��֮ǰ����Ҫ�ҵ���Ϊ��׼����һ�С�
int getTheStarLine(LineImage &lineimgObj,IplImage *src,int lineCntInGroup,int lineLen);

//������һ��linemask��������ʱ��linenum
int getMaxLineGroupSumLineWithinTol(IplImage *src,LineImage &lineimgObj,int linePitch,int lineCntInGroup,int lineLen,CvPoint startPT,int  pitchTolInt);

//��ȡÿһ�����ص�����ƫ�����ݡ�
//���� int ��Ϊʼ����Ϊ�ο������к�
//vect ����Ϊһ��vect��ÿ����ԱΪһ���߶�����ڵ�һ���߶ε�ƫ�����ظ���
 int getShiftPos(IplImage *src,int linePitch,double pitchTol,int lineCntInGroup,int &vectElementCount,vectorPoint &vect);
//set line white from ref the line num in linelist
void setImgLineGroup(IplImage* src,std::list<int> linelist);

int getShiftPosProcess(IplImage* src,vectorPoint &rltvec ,int secCnt,bool isAdaptiveThres=true,int thresholdBW=10, int achorCordnate=1);

/************��������**************************/
extern unsigned char cmpLineLen;
extern float Purfactor ;
extern int continuesLinecount;//�����ȵ������ߵ�����
extern int safeVoidpitchCycleCntIngetStartLine ;//�ڼ���cmp starlineʱ��ȥ��ͼƬ���ļ���cycle,��ֹѭ���������Ϊpitch��һ��ͳ�ƾ�ֵ����һ��׼ȷ��
//��ֵ���δ�ҵ�������0.05��ʼ����̨��0.05���ϵ�����ֱ���ҵ�Ϊֹ
extern float continuesTol;//������ʱ<0.05�����������߱���Ϊ�������Ҿ���ʱ��sdv/mean,ԽСԽ��
extern int lineThickness;
extern double allowedPercentTOLwhenShifting;

#endif