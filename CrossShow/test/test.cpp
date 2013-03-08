#include "cv.h"
#include "highgui.h"
#include "../include/global_def.h"
#include "headers.h"
#include "llfutility.h"
#include <list>

#if 0
int main(int argc, char ** argv)
{
	int thresholdBW = 30;
	cvNamedWindow("image");
	IplImage * src = cvLoadImage("../test/shizix.jpg", 0);
	if(src==NULL){printf("load file erro\n");return -1;}
	IplImage * temp = cvCreateImage(cvGetSize(src), 8,1);
	IplImage * img=cvCreateImage(cvGetSize(src), 8, 1);
	cvCopyImage(src,temp);
	cvCopyImage(src, img);

	 IplConvKernel *element = 0; //定义形态学结构指针
    element = cvCreateStructuringElementEx(3,3, 1, 1, CV_SHAPE_ELLIPSE, 0);//3,5,7
	
	
	//“黑帽”
	cvMorphologyEx(
		src,
		img,
		temp,
		NULL,//element,// //default 3*3
		CV_MOP_BLACKHAT,
		1);


	cvShowImage("image", img);
	
	IplImage* dst = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	cvThreshold( img, dst,thresholdBW, 255, CV_THRESH_BINARY ); //取阈值把图像转为二值图像

	IplImage *scaledImg = g_resizeImage(dst,1.005);
	cvErode( scaledImg, scaledImg,element);//腐蚀
	 cvThreshold( scaledImg, scaledImg,thresholdBW, 255, CV_THRESH_BINARY );
	cvSaveImage("dstnosmooElipseshizix2.jpg",scaledImg);
	cvShowImage("scaledImg", scaledImg);
	cvWaitKey();
	 
	cvReleaseImage(&temp);
	cvReleaseImage(&src);
	cvReleaseImage(&img);
	cvReleaseImage(&scaledImg);
	cvDestroyAllWindows();
	return 0;
}
#endif
#if 1
int main()
{
	
	//IplImage* img=g_CreateCrossImage(cvSize(640,480),3,20);
	//LineImage lineimgObj(cvGetSize(img));
	//cvSaveImage("manualed_cross_show6.bmp",img);
	//IMG_SHOW("IMG",img);

	//cvWaitKey();
	
	IplImage* src;
	if( (src=cvLoadImage("dstnosmooElipseshizix.jpg",0))==NULL)//如使用压缩图片，如jpg，会造成图像数据损失。
	{
		printf("load img erro\n");
		return -1;
	}
	//src = g_CopyRectFromImg(src,cvRect(300,300,300,300));
	cvThreshold( src,src,50, 100, CV_THRESH_BINARY ); //取阈值把图像转为二值图像
	//获取当前时间

	GetLocalTime( &sys ); 
	printf( "mainStart...%4d/%02d/%02d %02d:%02d:%02d.%03d \n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 
	int pitch= getLinePitchProcess(*src);
	printf("pitch:%d\n",pitch);
	GetLocalTime(&sys);
	printf( "mainEnd..%4d/%02d/%02d %02d:%02d:%02d.%03d \n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 

   vectorPoint vec;
   int startline = getShiftPos(src,pitch, allowedPercentTOLwhenShifting,continuesLinecount,15,vec);
   printf("start line:%d\n",startline);
	printVecPoint(vec);
   GetLocalTime(&sys);
   printf( "after getShiftPos..%4d/%02d/%02d %02d:%02d:%02d.%03d \n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 

   	IMG_SHOW("src",src);
	cvSaveImage("aftersrc.jpg",src);
 cvWaitKey();
	cvReleaseImage(&src);
	return 0;
}
#endif
//int main()
//{
//	CalcObjList cal;
//	cal.addone(1,23);
//cal.addone(2,24);
//cal.addone(3,23);
//cal.addone(4,24);
//cal.addone(5,23);
//std::list<LINEIFO>::iterator it = cal.m_list.begin();
//it = cal.m_list.erase(it);
//cal.removeWhereValueis(23);
//	return 0;
//}