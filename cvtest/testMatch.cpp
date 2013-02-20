
#include "global.h"
#include <windows.h> 
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
#if 0
int main( int argc, char** argv ) {

	IplImage *src,*ftmp[6], *srcImg; //ftmp is what to display on
	IplImage *back, *newsample;
	int methodCount = 2;//1,2,3
	//此实验中CV_TM_CCOEFF为最佳方法
	int priorityMethod = CV_TM_CCOEFF;//CV_TM_SQDIFF,CV_TM_CCORR,CV_TM_CCOEFF,
	bool useNormaled = true;
	int priorityMethod_getCycle=CV_TM_CCOEFF;
	int assumedMinCycle = 5;//针对小循环可设小些，如5，大循环应该设大些，如50
	int stripHeightTofindCycle = 100;//条越宽，其与周围像素的关联性越大，其唯一性增强。
	float imgeScale = 2.0;
	int widthDiv=2;
	int deltWidthShift=10;
	int startlineforMinCycle = 20;
	//g_getPresample("../cvtest/8newsample.jpg",60,100,"8back.jpg");return 0;
	
	//get the sample rect from the background diff
	if ((back = cvLoadImage("../cvtest/8back.jpg",0))==NULL)printf("load src erro\n");
	if ((newsample = cvLoadImage("../cvtest/8newsample.jpg",0))==NULL)printf("load src erro\n");
	SampleWithBackGroud avg(back,newsample,3);
	avg.backgroundDiffllf();
	CvRect  boundRect = avg.calcBoundingRect();
	printf("forground rect(%d,%d,%d,%d)\n",boundRect.x,boundRect.y, boundRect.width, boundRect.height);

	//load the mather
	if ((src = cvLoadImage("../cvtest/8mather.jpg",0))==NULL)printf("load src erro\n");

	Mather *mather = new Mather(g_resizeImage(src,imgeScale));
	cvReleaseImage(&src);


	mather->setMaxAngle(2);
	int cycle_height = mather->findMinCycle(stripHeightTofindCycle,startlineforMinCycle,assumedMinCycle,priorityMethod_getCycle,useNormaled);
	printf("cycle height:%d\n",cycle_height);
	//getchar();
#if 1
	//获取当前时间
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	printf( "mainStart...%4d/%02d/%02d %02d:%02d:%02d.%03d \n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 

	//find sample hdr in mather

	Sample sample(avg.InewSample, avg.getBoundingRect(),imgeScale);


	mather->catchSample(&sample);
	mather->getSampleShinked(0,0,0,0);
	 mather->setTmplateNum(5);

	int hdrPos = mather->findSampleHdrPos(widthDiv,deltWidthShift,methodCount,priorityMethod,useNormaled);
	printf("hdrpos:%d\n",hdrPos);


	GetLocalTime( &sys ); 
	printf( "mainStart...%4d/%02d/%02d %02d:%02d:%02d.%03d \n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 

	mather->mkLineColor(hdrPos,2,0);
	
	IplImage *samplePatchImg;
	IplImage *result = cvCloneImage(mather->getImage());
	
	int i=0;
	while(sample.getPatchImg(&samplePatchImg)!=-1)
	{
		mather->fetchSamplePatchImg(samplePatchImg);
 
		CvPoint pt = mather->MatchPatch(sample.m_rectPatch.x,methodCount,priorityMethod,useNormaled);

		printf("find mather 绝对 pt(%d,%d)\n",pt.x,pt.y);
		mather->releaseSamplePatchCopy(&samplePatchImg);

		mather->releaseSamplePatchCopy(&samplePatchImg);
		mather->releaseSamplePatchCopy(&samplePatchImg);
		cvSetImageROI(result, cvRect(pt.x,pt.y,
			sample.m_rectPatch.width, sample.m_rectPatch.height));
		if (i%2==0)
			cvAddS(result,cvScalar(ADD_VALUE),result);
		else
			cvAddS(result,cvScalar(-ADD_VALUE/2),result);
	
		cvResetImageROI(result);
		//mark rect with color
		sample.markPatchRect();
		mather->markTemplateRect();

		sample.stepTmplate();
		++i;
	}
	printf("find angle:%.2f\n",mather->getFindAngle());


	GetLocalTime( &sys ); 
	printf( "end...%4d/%02d/%02d %02d:%02d:%02d.%03d \n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 

	//avg.showImg();
	sample.showPic();	
	mather->showPic();
	cvNamedWindow("findresult");
	cvShowImage("findresult",result);

	sample.saveImageToFile("sample.jpg");
	mather->saveImageToFile("mather.jpg");
	cvSaveImage("matherafter.jpg",mather->m_cpsrc);
	cvSaveImage("result.jpg",result);


	cvWaitKey();
	//getchar();
#endif		
	
}
#endif