
#include "global.h"

int main( int argc, char** argv ) {

	IplImage *src, *templ,*ftmp[6], *srcImg; //ftmp is what to display on

	//get  the template to be used for matching:
	if ((templ = cvLoadImage("../cvtest/rotate degree3.00.jpg",0))==NULL)printf("load sample erro\n");

	//get the src
	if ((src = cvLoadImage("../cvtest/rotate degree0.00.jpg",0))==NULL)printf("load src erro\n");

	Mather *mather = new Mather(src);
	Sample sample(src,templ);
	sample.initTmplate(400,cvSize(50,50));
	IplImage *tmp;
	IplImage *result = cvCloneImage(src);
	int i=0;
	while(sample.getTmplate(&tmp)!=-1)
	{
		mather->createTmplate(tmp);
		CvPoint pt = mather->findMatchPoint(tmp,1);
		printf("find pt:(%d,%d)\n",pt.x,pt.y);
		mather->releaseTmplate(tmp);

		cvSetImageROI(result, cvRect(pt.x,pt.y,sample.m_rectTmplate.width, sample.m_rectTmplate.height));
		if (i%2==0)
			cvSet(result,cvScalar(255));
		else
			cvSet(result,cvScalar(0));
	
		cvResetImageROI(result);
		sample.stepTmplate();
		++i;
	}
	cvNamedWindow("sample");
	cvShowImage("sample",templ);
	cvNamedWindow("findresult");
	cvShowImage("findresult",result);
	cvWaitKey();
	//getchar();
}
