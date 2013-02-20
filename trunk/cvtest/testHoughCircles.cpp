#include <cv.h>
#include <highgui.h>
#include <math.h>
#if 0
int main(int argc, char** argv) {
	cvNamedWindow("image");
	IplImage * src = cvLoadImage("../cvtest/7newsample.jpg", 0);

	IplImage * temp = cvCreateImage(cvGetSize(src), 8,1);
	IplImage * img=cvCreateImage(cvGetSize(src), 8, 1);
	cvCopyImage(src,temp);
	cvCopyImage(src, img);
	cvSmooth(img,img);
	IplConvKernel *element = 0; //定义形态学结构指针
	element = cvCreateStructuringElementEx(3,3, 1, 1, CV_SHAPE_ELLIPSE, 0);//3,5,7
	cvErode( src, src, element);
	//形态梯度
	cvMorphologyEx(
		src,
		img,
		img,
		element,//NULL, //default 3*3
		CV_MOP_GRADIENT,
		1);

	cvShowImage("image", img);
	cvWaitKey(0);
	IplImage* image=img; //= cvLoadImage( 		"../cvtest/7newsample.jpg",		CV_LOAD_IMAGE_GRAYSCALE		);
	IplImage* src2 =img;//= cvLoadImage( "../cvtest/7newsample.jpg"); //Changed for prettier show in color
	CvMemStorage* storage = cvCreateMemStorage(0);
	cvSmooth(image, image, CV_GAUSSIAN, 5, 5 );
	CvSeq* results = cvHoughCircles( 
		image, 
		storage, 
		CV_HOUGH_GRADIENT, 
		4, 
		image->width/10 
		); 
	for( int i = 0; i < results->total; i++ ) {
		float* p = (float*) cvGetSeqElem( results, i );
		CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
		cvCircle( 
			src2,
			pt, 
			cvRound( p[2] ),
			CV_RGB(0xff,0,0) 
			);
	}
	cvNamedWindow( "cvHoughCircles", 1 );
	cvShowImage( "cvHoughCircles", src2);
	cvWaitKey(0);
}

#endif