#include "cv.h"
#include "highgui.h"
#include "utility.h"
#if 1
int main(int argc, char ** argv)
{
	int thresholdBW = 30;
	cvNamedWindow("image");
	IplImage * src = cvLoadImage("../cvtest/shizix.jpg", 0);

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