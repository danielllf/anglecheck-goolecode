#include "../include/prepare.h"
#include "../include/llfutility.h"
IplImage* getSubImgDevidedByVerticalLine(IplImage *src)
{
	IplImage * tmp;

	return tmp;
}
IplImage *getMorphologyImg(IplImage* src,int operation,int thresholdBW, int isShowResultImg, int achorCordnate)
{

	IplImage * temp = cvCreateImage(cvGetSize(src), 8,1);
	IplImage * img=cvCreateImage(cvGetSize(src), 8, 1);
	cvCopyImage(src,temp);
	cvCopyImage(src, img);

	IplConvKernel *element = 0; //������̬ѧ�ṹָ��
	element = cvCreateStructuringElementEx(2*achorCordnate+1,2*achorCordnate+1, achorCordnate, achorCordnate, CV_SHAPE_RECT, 0);//3,5,7

	//����ñ��CV_MOP_BLACKHAT
	//"��ñ"CV_MOP_TOPHAT
	cvMorphologyEx(
		src,
		img,
		temp,
		element,// //default 3*3
		operation,//CV_MOP_BLACKHAT,
		1);

	IplImage* dst = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	cvThreshold( img, dst,thresholdBW, 255, CV_THRESH_BINARY ); //ȡ��ֵ��ͼ��תΪ��ֵͼ��
	if(isShowResultImg)
	{
		cvShowImage("MorphologyImgBinery", dst);
		cvWaitKey();
	}
	cvReleaseImage(&temp);
	cvReleaseImage(&img);

	return dst;
}