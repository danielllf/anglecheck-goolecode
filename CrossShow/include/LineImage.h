#ifndef _LINE_IMAGE_
#define _LINE_IMAGE_
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>

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



#endif