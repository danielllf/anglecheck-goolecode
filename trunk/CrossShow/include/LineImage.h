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
	int resetImagePitch(int linePitch);
public:
	IplImage *m_pImage;
	int m_linePitch;
};
#endif