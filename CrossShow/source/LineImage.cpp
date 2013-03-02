#include "../include/global_def.h"
#include "../include/LineImage.h"
#include "../include/llfutility.h"
//Ϊ������ͼ�δ���ʱ�����ݾ���С����ֵӦ����С�����Ϊ1.ֻ�Ǵ�ʱ�۾����������ԲŴ���Щ
int WHITE_PART=100;//�ڰ�ͼƬ�а�ɫֵ����ΪС��255����������ͼ��Ӻ���С��255�������洦��

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
void LineImage::resetImageLine(CvPoint startPt,int lineLen)
{
		cvSet(m_pImage,cvScalar(0));
		cvLine(m_pImage,startPt,cvPoint(lineLen-1,startPt.y),cvScalar(WHITE_PART));
	
}
void LineImage::resetImageLineGroup(CvPoint startPt,int lineLen,int lineCntIngroup,int linepitch)
{

	cvSet(m_pImage,cvScalar(0));
	for (int i=0;i<lineCntIngroup;++i)
	{
		cvLine(m_pImage,cvPoint(startPt.x,startPt.y+i*linepitch),cvPoint(startPt.x+lineLen-1,startPt.y+i*linepitch),cvScalar(WHITE_PART));

	}

}

