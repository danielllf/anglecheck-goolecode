#include "utility.h"
#include "AvgBackGround.h"

AvgBackGround::AvgBackGround(IplImage *avgback, IplImage *newsample, int thresHold)
{
	m_thresHold = thresHold;
	CvSize sz = cvGetSize(avgback);

	IavgBack = cvCreateImage(sz,IPL_DEPTH_8U,1);
	IhiF = cvCreateImage(sz,IPL_DEPTH_8U,1);
	IlowF = cvCreateImage(sz,IPL_DEPTH_8U,1);
	Imask = cvCreateImage( sz, IPL_DEPTH_8U, 1 );
	InewSample = cvCreateImage( sz, IPL_DEPTH_8U, 1 );
	IthresHold = cvCreateImage(sz,IPL_DEPTH_8U,1);

	cvZero(IhiF);
	cvZero(IlowF);
	cvZero(Imask);

	cvCopy(avgback,IavgBack);
	cvCopy(newsample,InewSample);
	cvSet(IthresHold,cvScalar(10));

}

AvgBackGround::~AvgBackGround()
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
void AvgBackGround::backgroundDiff()  //Mask should be grayscale
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
void AvgBackGround::showImg()
{
	IMG_SHOW("Imask",Imask);
	cvWaitKey();
}