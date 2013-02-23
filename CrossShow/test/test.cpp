#include "cv.h"
#include "highgui.h"
#include "headers.h"
#include "utility.h"
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
typedef struct rrect{
	int x;
	int y;
	int width;
	int height;
}rrect;
void print_rect(void *pdata){
	printf("%d,",((int*)((rrect*)pdata)->x));
	printf("%d,",((int*)((rrect*)pdata)->y));

	printf("%d,",((int*)((rrect*)pdata)->width));

	printf("%d\n",((int*)((rrect*)pdata)->height));

}

int main()
{
	
	IplImage* img=g_CreateCrossImage(cvSize(640,480),3,20);
	LineImage lineimg(cvGetSize(img));
	lineimg.resetImageLine(cvPoint(0,50),90);
	/*cvSaveImage("manualed_cross_show.jpg",img);
	IMG_SHOW("IMG",img);*/
	IMG_SHOW("lineed",lineimg.m_pImage);
	cvWaitKey();

	IplImage* src;
	if( (src=cvLoadImage("manualed_cross_show.jpg",0))==NULL)
	{
		printf("load img erro\n");
		return -1;
	}
	 getLinePitch(*src);

	
	


getchar();
	return 0;
}