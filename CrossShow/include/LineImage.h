#ifndef _LINE_IMAGE_
#define _LINE_IMAGE_
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>

//黑白图像，黑背景+白线条

#define LINE_VERTICAL 1  
#define LINE_HORIZON 0

class LineImage{
public:
	LineImage(CvSize size,int lineThick,int whiteLineWeight,int lineDirect=LINE_HORIZON);
	~LineImage();
	IplImage *getImage();
	void resetImageLine(CvPoint startPt,int lineLen);
	void resetImageLineGroup(CvPoint startPt,int lineLen,int lineCntIngroup,int linepitch);
public:
	IplImage *m_pImage;
	int m_lineThickness;
	int m_whiteLineweight;
	int m_lineDirect;
};

//所有计算过程在此，与图像无关



#endif