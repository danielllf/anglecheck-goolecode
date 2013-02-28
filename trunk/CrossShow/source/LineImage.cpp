#include "../include/global_def.h"
#include "../include/LineImage.h"
//Ϊ������ͼ�δ���ʱ�����ݾ���С����ֵӦ����С�����Ϊ1.ֻ�Ǵ�ʱ�۾����������ԲŴ���Щ
int WHITE_PART=50;//�ڰ�ͼƬ�а�ɫֵ����ΪС��255����������ͼ��Ӻ���С��255�������洦��

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

