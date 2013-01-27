
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
#if 1
int main( int argc, char** argv ) {

	IplImage *src,*ftmp[6], *srcImg; //ftmp is what to display on
	IplImage *back, *newsample;
	int methodCount = 2;//1,2,3
	//��ʵ����CV_TM_CCOEFFΪ��ѷ���
	int priorityMethod = CV_TM_CCOEFF;//CV_TM_SQDIFF,CV_TM_CCORR,CV_TM_CCOEFF,
	bool useNormaled = true;
	int priorityMethod_getCycle=CV_TM_CCOEFF;
	int assumedMinCycle = 50;//���Сѭ������СЩ����5����ѭ��Ӧ�����Щ����50
	int stripHeightTofindCycle = 60;//��Խ��������Χ���صĹ�����Խ����Ψһ����ǿ��
	float imgeScale = 0.5;
	//g_getPresample("../cvtest/4newsample5.jpg",60,100,"4back5.jpg");return 0;
	
	//get the sample rect from the background diff
	if ((back = cvLoadImage("../cvtest/3back.jpg",0))==NULL)printf("load src erro\n");
	if ((newsample = cvLoadImage("../cvtest/3newsample.jpg",0))==NULL)printf("load src erro\n");
	SampleWithBackGroud avg(back,newsample,3);
	avg.backgroundDiffllf();
	CvRect  boundRect = avg.calcBoundingRect();
	printf("forground rect(%d,%d,%d,%d)\n",boundRect.x,boundRect.y, boundRect.width, boundRect.height);

	//load the mather
	if ((src = cvLoadImage("../cvtest/3mather.jpg",0))==NULL)printf("load src erro\n");

	Mather *mather = new Mather(g_resizeImage(src,imgeScale));
	cvReleaseImage(&src);


	mather->setMaxAngle(5);
	int cycle_height = mather->findMinCycle(stripHeightTofindCycle,0,assumedMinCycle,priorityMethod_getCycle,useNormaled);
	printf("cycle height:%d\n",cycle_height);
	//getchar();
#if 1
	//��ȡ��ǰʱ��
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	printf( "mainStart...%4d/%02d/%02d %02d:%02d:%02d.%03d \n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 

	//find sample hdr in mather

	Sample sample(avg.InewSample, avg.getBoundingRect(),imgeScale);


	mather->catchSample(&sample);
	mather->getSampleShinked(5,5,5,50);
	 mather->setTmplateNum(10);

	int hdrPos = mather->findSampleHdrPos(3,25,methodCount,priorityMethod,useNormaled);
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

		printf("find mather ���� pt(%d,%d)\n",pt.x,pt.y);
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