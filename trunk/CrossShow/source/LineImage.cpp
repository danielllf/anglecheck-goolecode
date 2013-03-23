#include "../include/global_def.h"
#include "../include/LineImage.h"
#include "../include/llfutility.h"

LineImage::LineImage(CvSize size,int lineThick,int whiteLineWeight,int lineDirect)
{
	m_pImage = cvCreateImage(size,IPL_DEPTH_8U,1);
	cvSet(m_pImage,cvScalar(0));
	m_lineThickness = lineThick;
	m_whiteLineweight = whiteLineWeight;
	m_lineDirect =lineDirect;
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
		if(m_lineDirect==LINE_HORIZON)
		{
			cvLine(m_pImage,startPt,cvPoint(startPt.x+lineLen-1,startPt.y),cvScalar(m_whiteLineweight),m_lineThickness);
		}
		else
		{
			cvLine(m_pImage,startPt,cvPoint(startPt.x,startPt.y+lineLen-1),cvScalar(m_whiteLineweight),m_lineThickness);
		}

	
}
void LineImage::resetImageLineGroup(CvPoint startPt,int lineLen,int lineCntIngroup,int linepitch)
{

	cvSet(m_pImage,cvScalar(0));
	for (int i=0;i<lineCntIngroup;++i)
	{
		cvLine(m_pImage,cvPoint(startPt.x,startPt.y+i*linepitch),cvPoint(startPt.x+lineLen-1,startPt.y+i*linepitch),cvScalar(m_whiteLineweight), m_lineThickness);

	}

}

