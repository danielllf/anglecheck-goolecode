#ifndef _LINE_IMAGE_
#define _LINE_IMAGE_
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>

//�ڰ�ͼ�񣬺ڱ���+������



class LineImage{
public:
	LineImage(CvSize size,int lineThick);
	~LineImage();
	IplImage *getImage();
	void resetImageLine(CvPoint startPt,int lineLen);
	void resetImageLineGroup(CvPoint startPt,int lineLen,int lineCntIngroup,int linepitch);
public:
	IplImage *m_pImage;
	int m_lineThickness;
};

//���м�������ڴˣ���ͼ���޹�



#endif