
#include "global.h"

int main( int argc, char** argv ) {

	IplImage *src, *templ,*ftmp[6], *srcImg; //ftmp is what to display on

	//get  the template to be used for matching:
	if ((templ = cvLoadImage("../cvtest/rotate degree5.00.jpg",0))==NULL)printf("load sample erro\n");

	//get the src
	if ((src = cvLoadImage("../cvtest/rotate degree0.00.jpg",0))==NULL)printf("load src erro\n");


	Mather *mather = new Mather(src);
	mather->setMaxAngle(8);
	Sample sample(src,templ);
	sample.initTmplate(200,cvSize(50,50));
	IplImage *tmp;
	IplImage *result = cvCloneImage(src);
	int i=0;
	while(sample.getTmplate(&tmp)!=-1)
	{
		mather->createTmplate(tmp);
 
		CvPoint pt = mather->findMatchPoint(sample.m_rectTmplate,1);
		printf("find mather ¾ø¶Ô pt(%d,%d)\n",pt.x,pt.y);
		mather->releaseTmplate(tmp);

		cvSetImageROI(result, cvRect(pt.x,pt.y,
			sample.m_rectTmplate.width, sample.m_rectTmplate.height));
		if (i%2==0)
			cvSet(result,cvScalar(0));
		else
			cvSet(result,cvScalar(255));
	
		cvResetImageROI(result);
		//mark rect with color
		sample.markTmpRect();
		mather->markTmpRect();

		sample.stepTmplate();
		++i;
	}
	printf("find angle:%.2f\n",mather->getFindAngle());
	sample.showPic();
	mather->showPic();
	cvNamedWindow("findresult");
	cvShowImage("findresult",result);
	cvWaitKey();
	//getchar();
}
