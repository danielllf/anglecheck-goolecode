#include "cv.h"
#include "highgui.h"
#include "../include/global_def.h"
#include "headers.h"
#include "llfutility.h"
#include <list>
#include "prepare.h"
/*
1>获取linepitch（此时的图中linelen长度内不能有垂直分界线）
2>求出每个子图的垂直分界线
3>计算每个子图的偏移相对坐标
4>将每个子图的相对坐标合并成一副图的整体坐标

*/
#if 0
int main(int argc, char ** argv)
{
	int thresholdBW = 10;
	//cvNamedWindow("image");
	IplImage * src = cvLoadImage("IMG_6995_1.bmp", 0);
	if(src==NULL){printf("load file erro\n");return -1;}
	cvNot(src,src);
IplImage *subImg =g_CopyRectFromImg(src,cvRect(0,0,600,400));
	IplImage *scaledImg = g_resizeImage(subImg,1.00);

	IplImage * temp = cvCreateImage(cvGetSize(scaledImg), 8,1);
	IplImage * img=cvCreateImage(cvGetSize(scaledImg), 8, 1);
	cvCopyImage(scaledImg,temp);
	cvCopyImage(scaledImg, img);

	 IplConvKernel *element = 0; //定义形态学结构指针
    element = cvCreateStructuringElementEx(3,3, 1, 1, CV_SHAPE_RECT, 0);//3,5,7
	
	
	//“黑帽”
	cvMorphologyEx(
		scaledImg,
		img,
		temp,
		NULL,//element,// //default 3*3
		CV_MOP_BLACKHAT,//CV_MOP_BLACKHAT,
		1);
	//cvShowImage("src", src);
	cvShowImage("imagecaled", scaledImg);
	cvShowImage("image", img);
	cvWaitKey();
	
	IplImage* dst = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	cvThreshold( img, dst,thresholdBW, 255, CV_THRESH_BINARY ); //取阈值把图像转为二值图像
  

	//cvErode( scaledImg, scaledImg,element);//腐蚀
	//cvThreshold( dst, dst,thresholdBW, 255, CV_THRESH_BINARY );
	 //cvAdaptiveThreshold(img,dst,255.0,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,3,6);
	cvSaveImage("dstnosmooElipseshizix2.bmp",dst);
	cvShowImage("scaledImg", dst);
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

	int thresholdBW = 10;
	IplImage * src1 = cvLoadImage("6955_600_400_1.bmp", 0);
	if(src1==NULL){printf("load file erro\n");return -1;}
	//IplImage *subImg =g_CopyRectFromImg(src1,cvRect(0,0,600,400));
	cvThreshold(src1,src1,5,255.0,CV_THRESH_BINARY);

	int x = getX_cordinateofVerticalBar(src1,14,1.5,TARGETPOS_LEFT,10,100,true);
	//获取当前时间
	getchar();return 0;
	//cvSaveImage("6955_600_400.bmp",subImg);
	IplImage *scaledImg =src1; //g_resizeImage(subImg,1.00);
	IplImage* src=getMorphologyImg(scaledImg,CV_MOP_TOPHAT,thresholdBW);

	cvThreshold( src,src,50, 100, CV_THRESH_BINARY ); //取阈值把图像转为二值图像

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