#ifndef _LINE_IMAGE_
#define _LINE_IMAGE_
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include <iostream>
#include<vector>
//�ڰ�ͼ�񣬺ڱ���+������



class LineImage{
public:
	LineImage(CvSize size);
	~LineImage();
	IplImage *getImage();
	int resetImageLine(CvPoint startPt,int lineLen);
public:
	IplImage *m_pImage;
	int m_linePitch;
};

//���м�������ڴˣ���ͼ���޹�

struct LINEIFO{
	int lineNum;
	int sum;
};
class CalcObj{

 
public:
	CalcObj();
	~CalcObj();
	void addone(int linenum,int sum);
	LINEIFO getTheMaxElement();
	void clearAll();
private:
	std::vector<LINEIFO> m_vect;
};
#endif