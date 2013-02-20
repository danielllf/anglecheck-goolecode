#include "cv.h"
#include "highgui.h"
#include "utility.h"
#if 1
int main(int argc, char ** argv)
{
	cvNamedWindow("image");
	IplImage * src = cvLoadImage("../cvtest/shizix.jpg", 0);

	IplImage * temp = cvCreateImage(cvGetSize(src), 8,1);
	IplImage * img=cvCreateImage(cvGetSize(src), 8, 1);
	cvCopyImage(src,temp);
	cvCopyImage(src, img);
	//cvSmooth(img,img);
 IplConvKernel *element = 0; //������̬ѧ�ṹָ��
    element = cvCreateStructuringElementEx(3,3, 1, 1, CV_SHAPE_ELLIPSE, 0);//3,5,7
	
	//��̬�ݶ�
	cvMorphologyEx(
		src,
		img,
		temp,
		NULL,//element,// //default 3*3
		CV_MOP_GRADIENT,
		1);

 
	
	cvShowImage("image", img);
	cvWaitKey(0);

	//"��ñ"
	cvMorphologyEx(
		src,
		img,
		temp,
		NULL,// element,////default 3*3
		CV_MOP_TOPHAT,
		1);
	cvShowImage("image", img);
	cvWaitKey(0);
	//����ñ��
	cvMorphologyEx(
		src,
		img,
		temp,
		NULL,//element,// //default 3*3
		CV_MOP_BLACKHAT,
		1);


	cvShowImage("image", img);
	cvSaveImage("../saved2.jpg",img);
	cvWaitKey(0);
	
	IplImage* dst = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	cvThreshold( img, dst,30, 255, CV_THRESH_BINARY ); //ȡ��ֵ��ͼ��תΪ��ֵͼ��
	//cvAdaptiveThreshold( img, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 5 );
	/*cvEqualizeHist(img,equalizedimg);
	cvSaveImage("../savedEqulized.jpg",equalizedimg);*/
	//�øýṹ��Դͼ�������ѧ��̬ѧ�Ŀ������󣬹��Ʊ�������
	IplImage *scaledImg = g_resizeImage(dst,1.005);
	cvErode( scaledImg, scaledImg,element);//��ʴ
	 cvThreshold( scaledImg, scaledImg,30, 255, CV_THRESH_BINARY );
	cvSaveImage("../dstnosmooElipseshizix2.jpg",scaledImg);
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