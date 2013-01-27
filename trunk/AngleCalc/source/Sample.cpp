#include "Sample.h"
#include "utility.h"

//此类操作对象不应包含背景信息
//sample中，采样图中的多个子矩形区域中的一个叫做：patch
//sample中应该为从背景中取出的布匹（前景图）

Sample::Sample(IplImage *SampleSrc, CvRect rectInSrc)
{

	m_patchImg=NULL;
	m_iterTemplate = 0;
	m_totPatchNum = 0;
	m_sample = m_sampleBeforeShrink = g_CopyRectFromImg(SampleSrc, rectInSrc);

}
Sample::~Sample()
{
	cvReleaseImageHeader(&m_patchImg);

	cvReleaseImage(&m_sample);
	cvReleaseImage(&m_sampleBeforeShrink);
}

IplImage *Sample::getImage()
{
	return m_sample;
}

void Sample::shrinkSample(int left, int right, int top, int bottom)
{
   //update sample rect
	CvRect shinkedRect = cvRect(left, top,
										m_sampleBeforeShrink->width-left-right, m_sampleBeforeShrink->height-bottom-top);
	//update other related parms
	m_sample = g_CopyRectFromImg(m_sampleBeforeShrink, shinkedRect);

}
void Sample::setPatchNum(int num)
{
	m_totPatchNum = num;
	int patchWidth = m_sample->width/m_totPatchNum;
	int patchHeight = m_sample->height;
	m_patchSize = cvSize(patchWidth, patchHeight);
	//init the 1st patch
	m_rectPatch = cvRect(0,0,m_patchSize.width,m_patchSize.height);
}


int Sample::getPatchImg(IplImage ** temp)
{
	assert(m_totPatchNum!=0);
	if (m_iterTemplate<m_totPatchNum)
	{
		*temp = g_CopyRectFromImg(m_sample,m_rectPatch);
		printf("sample patch rect:(%d,%d,%d,%d)\t",m_rectPatch.x,m_rectPatch.y,m_rectPatch.width, m_rectPatch.height);

		//++m_iterTemplate;
		
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
	m_rectPatch.x += m_patchSize.width;
	++m_iterTemplate;
}

//color the rect
void Sample::markPatchRect()
{
	cvSetImageROI(m_sample,m_rectPatch);
	if(m_iterTemplate%2==0)
	{
		cvAddS(m_sample,cvScalar(ADD_VALUE),m_sample);
	}
	else
	{
		cvAddS(m_sample,cvScalar(-ADD_VALUE),m_sample);
	}
	cvResetImageROI(m_sample);
}

void Sample::showPic()
{
	cvNamedWindow("sample");
	cvShowImage("sample",m_sample);
}