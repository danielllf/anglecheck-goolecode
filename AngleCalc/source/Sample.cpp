#include "Sample.h"

Sample::Sample(IplImage *mather, IplImage *sample)
{

	m_templateImg=NULL;
	getPrmFromMather(mather);
	loadSamplePic(sample);

}
Sample::~Sample()
{
	cvReleaseImageHeader(&m_templateImg);
	cvReleaseImage(&m_sample);
}

//prepair for the ROI param
void Sample::getPrmFromMather(IplImage *mather)
{
	//sholud be same as the sample pic
	m_nChannels = mather->nChannels;
	m_depth = mather->depth;
	m_widthStep = mather->widthStep;
}


//may load dynamicly on realtime running
void Sample::loadSamplePic(IplImage *sample)
{
	m_sample = cvCreateImage(cvSize(sample->width,sample->height),sample->depth, sample->nChannels);
	cvCopy(sample,m_sample);
}

//对一个图所有子template操作完后，调用resetTmplate以为下一幅sample图做准备
//初始化这一组template 的starline 和 cvsize，与具体图像数据有关
void Sample::initTmplate(int starLine, CvSize templateSize)
{
	m_tmplateSize = templateSize;
	m_cmpStartLine = starLine;
	m_iterTemplate = 0;
	m_rectTmplate = cvRect(0,starLine, templateSize.width,templateSize.height);
	m_totTmplateNum = m_sample->width/templateSize.width;
	if(m_templateImg==NULL)
	{
		//init header
		m_templateImg = cvCreateImageHeader(
			m_tmplateSize,
			m_depth,
			m_nChannels);	
		m_templateImg->origin = m_sample->origin;
		m_templateImg->widthStep = m_sample->widthStep;
	}


}

//创建ROI，并赋于数据
int Sample::getTmplate(IplImage ** temp)
{
	if (m_iterTemplate<m_totTmplateNum)
	{
		m_templateImg->imageData = m_sample->imageData + m_rectTmplate.y*m_sample->widthStep +	m_rectTmplate.x;		
		++m_iterTemplate;
		printf("template:(%d,%d)\t",m_rectTmplate.x,m_rectTmplate.y);

		*temp=m_templateImg;

		return 0;	
	}
	else
	{
		return -1;
	}
}

void Sample::stepTmplate()
{
	//right move rect 
	m_rectTmplate.x += m_tmplateSize.width;
}