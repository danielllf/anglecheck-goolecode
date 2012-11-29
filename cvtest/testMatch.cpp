
#include "global.h"
#if 0
int main()
{
	IplImage *back, *newsample;

	//get the src
	if ((back = cvLoadImage("../cvtest/3org.jpg",0))==NULL)printf("load src erro\n");
	if ((newsample = cvLoadImage("../cvtest/3.jpg",0))==NULL)printf("load src erro\n");
	SampleWithBackGroud avg(back,newsample,3);
	avg.backgroundDiffllf();
	CvRect  boundRect = avg.getBoundingRect();
	printf("rect(%d,%d,%d,%d)\n",boundRect.x,boundRect.y, boundRect.width, boundRect.height);
	avg.showImg();
	return 0;
}
#endif
#if 1
int main( int argc, char** argv ) {

	IplImage *src,*ftmp[6], *srcImg; //ftmp is what to display on
	IplImage *back, *newsample;

	//get the sample rect from the background diff
	if ((back = cvLoadImage("../cvtest/rotate degree1.00back.jpg",0))==NULL)printf("load src erro\n");
	if ((newsample = cvLoadImage("../cvtest/rotate degree1.00.jpg",0))==NULL)printf("load src erro\n");
	SampleWithBackGroud avg(back,newsample,3);
	avg.backgroundDiffllf();
	CvRect  boundRect = avg.calcBoundingRect();
	printf("forground rect(%d,%d,%d,%d)\n",boundRect.x,boundRect.y, boundRect.width, boundRect.height);

	//get  the template to be used for matching:
	//load the mather
	//if ((src = cvLoadImage("../cvtest/3mather.jpg",0))==NULL)printf("load src erro\n");
	if ((src = cvLoadImage("../cvtest/rotate degree0.00.jpg",0))==NULL)printf("load src erro\n");
	Mather *mather = new Mather(src);
	mather->setMaxAngle(5);
	int cycle_height = mather->findMinCycle(110,10);
	printf("cycle height:%d\n",cycle_height);
	//getchar();
#if 1
	//find sample hdr in mather
	 Sample sample(avg);
	//sample.shrinkSample(5,5);
	 sample.setPatchNum(5);
	 mather->setTmplateNum(5);
	CvRect hdr_rect = cvRect(boundRect.x, boundRect.y, boundRect.width/5, boundRect.height);
	
	int hdrPos = mather->findSampleHdrPos(newsample,hdr_rect);
	printf("hdrpos:%d\n",hdrPos);
	mather->mkLineColor(hdrPos,3);
	
	//sample.initTmplate(hdrPos,cvSize(boundRect.width/8,boundRect.height));
	//sample.initTmplate(200,cvSize(90,90));
	IplImage *samplePatchImg;
	IplImage *result = cvCloneImage(src);
	
	int i=0;
	while(sample.getPatchImg(&samplePatchImg)!=-1)
	{
		mather->fetchSamplePatchImg(samplePatchImg);
 
		CvPoint pt = mather->findMatchPoint(sample.m_rectPatch.x,1,CV_TM_CCOEFF);

		printf("find mather ¾ø¶Ô pt(%d,%d)\n",pt.x,pt.y);
		mather->releaseSamplePatchCopy(&samplePatchImg);

		cvSetImageROI(result, cvRect(pt.x,pt.y,
			sample.m_rectPatch.width, sample.m_rectPatch.height));
		if (i%2==0)
			cvAddS(result,cvScalar(0+50),result);
		else
			cvAddS(result,cvScalar(255-100),result);
	
		cvResetImageROI(result);
		//mark rect with color
		sample.markPatchRect();
		mather->markTemplateRect();

		sample.stepTmplate();
		++i;
	}
	printf("find angle:%.2f\n",mather->getFindAngle());

	avg.showImg();
	sample.showPic();
	mather->showPic();

	cvNamedWindow("findresult");
	cvShowImage("findresult",result);
	cvWaitKey();
#endif		
	
}
#endif