#include "utility.h"
#include "SampleWithBackGroud.h"

SampleWithBackGroud::SampleWithBackGroud(IplImage *avgback, IplImage *newsample, int thresHold)
{
	//avgback应该由计算多帧后得来，这里直接使用单背景代替，后续应该加入多帧累积计算

	m_boundingRect = cvRect(0,0,0,0);//为rect分配空间
	m_thresHold = thresHold;
	CvSize sz = cvGetSize(avgback);

	IavgBack = cvCreateImage(sz,IPL_DEPTH_8U,1);
	IhiF = cvCreateImage(sz,IPL_DEPTH_8U,1);
	IlowF = cvCreateImage(sz,IPL_DEPTH_8U,1);
	Imask = cvCreateImage( sz, IPL_DEPTH_8U, 1 );
	InewSample = cvCreateImage( sz, IPL_DEPTH_8U, 1 );
	IthresHold = cvCreateImage(sz,IPL_DEPTH_8U,1);
	ImaskAVGCC = cvCreateImage(sz,IPL_DEPTH_8U,1);

	cvZero(IhiF);
	cvZero(IlowF);
	cvZero(Imask);

	cvCopy(avgback,IavgBack);
	cvCopy(newsample,InewSample);
	cvSet(IthresHold,cvScalar(10));

}

SampleWithBackGroud::~SampleWithBackGroud()
{
	cvReleaseImage(&IavgBack);
	cvReleaseImage(&IhiF);
	cvReleaseImage(&IlowF);	
	cvReleaseImage(&InewSample);
	cvReleaseImage(&IthresHold);
	cvReleaseImage(&Imask);
}

// Create a binary: 0,255 mask where 255 means forground pixel
// I		Input image, 3 channel, 8u
// Imask	mask image to be created, 1 channel 8u
// num		camera number.
//
void SampleWithBackGroud::backgroundDiffllf()  //Mask should be grayscale
{
	//set hight threshold
	cvAdd(IthresHold,IavgBack,IhiF);
	//set low threshold
	cvSub(IavgBack,IthresHold,IlowF);
	//Channel 1
	cvInRange(InewSample,IlowF,IhiF,Imask);

	//Finally, invert the results
	cvSubRS( Imask, cvScalar(255), Imask);
}

CvRect SampleWithBackGroud::calcBoundingRect()
{
	assert(Imask!=NULL);
	cvCopy(Imask,ImaskAVGCC);

	int num=1;//只输出一个rect
	cvconnectedComponents(ImaskAVGCC,1,4.0,&num,&m_boundingRect);
	return m_boundingRect;
}
CvRect SampleWithBackGroud::getBoundingRect()
{	
	return m_boundingRect;
}
void SampleWithBackGroud::showImg()
{ 
	cvSetImageROI(InewSample,m_boundingRect);
	cvAddS(InewSample,cvScalar(60),InewSample);
	cvResetImageROI(InewSample);
	IMG_SHOW(IavgBack,IavgBack);
	IMG_SHOW(InewSample,InewSample);
	//IMG_SHOW(Imask,Imask);

}