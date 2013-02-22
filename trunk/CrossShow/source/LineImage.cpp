#include "../include/LineImage.h"

LineImage::LineImage(CvSize size)
{
	m_pImage = cvCreateImage(size,8,1);
}
LineImage::~LineImage()
{
	cvReleaseImage(&m_pImage);
}
int LineImage::resetImagePitch(int linePitch)
{
	for (int i=0;i<m_pImage->height;++i)
	{
		if(i%linePitch==0)
		cvLine(m_pImage,cvPoint(0,i),cvPoint(m_pImage->width,i),cvScalar(255));
	}
	return 0;
}