#include "../include/global_def.h"
#include "../include/LineImage.h"
//为了让在图形处理时，数据尽量小，此值应尽量小。最好为1.只是此时眼睛不见，所以才大了些
int WHITE_PART=50;//黑白图片中白色值设置为小于255，所以在两图相加后，仍小于255，不会益处。

LineImage::LineImage(CvSize size)
{
	m_pImage = cvCreateImage(size,IPL_DEPTH_8U,1);
	cvSet(m_pImage,cvScalar(0));
}
LineImage::~LineImage()
{
	cvReleaseImage(&m_pImage);
}
IplImage* LineImage::getImage()
{
	return	m_pImage; 
}
int LineImage::resetImageLine(CvPoint startPt,int lineLen)
{
		cvSet(m_pImage,cvScalar(0));
		cvLine(m_pImage,startPt,cvPoint(lineLen-1,startPt.y),cvScalar(WHITE_PART));
	
	return 0;
}

