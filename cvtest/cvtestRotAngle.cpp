// cvTest1.cpp : 定义控制台应用程序的入口点。
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define  DST_NUM 10
#define MAX_WIN_NAME_LEN 50

using namespace cv;
using namespace std;

void PrintMat(CvMat* A);
/** @function main */
int main( int argc, char** argv )
{
	char* source_window = "Source image";
	char rotate_window[DST_NUM][MAX_WIN_NAME_LEN];

	Mat rot_mat( 2, 3, CV_32FC1 );
	Mat src, rotate_dst[DST_NUM];

	/// 加载源图像
	src = imread( "../cvtest/miorg.jpg", 0 );

	//设置RIO
	//cvSetImageROI(&IplImage(src),cvRect(0,10,IplImage(src).width,20));
	//cvAddS(&IplImage(src),cvScalar(50),&IplImage(src));
	//cvSet2D(&IplImage(src),);
	//cvSetZero(&IplImage(src));
#if 1
	cvFlip(&IplImage(src));//转换坐标系为bottom left,用来计算图像顺时针角度偏移
	
	
	Point center = Point( 0, 0 );
	double angle = 0.0;//顺时针为负数
	double scale = 1.0;
	double  delta = 1.0;

	/// 通过上面的旋转细节信息求得旋转矩阵
	//rot_mat formuler:tranfform metrix 2*3
	//			Rz	==	[cosa -sina x-x*cosa+y*sina]
	//				==	[sina  cosa y-x*sina-y*cosa]
	//when rot with orig(0.0) TOP-LEFT point:
	//          Rz  == [cosa -sina 0]
	//				== [sina  cosa 0]
	
	//PrintMat(&CvMat(rot_mat));//打印显示矩阵rot_mat

	int i=0;
	while(i<DST_NUM)
	{
		memset(rotate_window[i],0,MAX_WIN_NAME_LEN);
		sprintf(rotate_window[i],"imrotate degree%-3.2f.jpg",angle);
		
		rot_mat = getRotationMatrix2D( center, -angle, scale );
		warpAffine( src, rotate_dst[i], rot_mat, src.size());///旋转src的图像
		//cvGetQuadrangleSubPix(&CvMat(src),&CvMat(rotate_dst[i]),&CvMat(rot_mat));
		cvFlip(&IplImage(rotate_dst[i]));// org point ==>top left befor show it 
		namedWindow( rotate_window[i], CV_WINDOW_AUTOSIZE );
		imshow( rotate_window[i], rotate_dst[i]);
		imwrite(rotate_window[i],rotate_dst[i]);
		angle += delta;
		i++;
	}

	cvFlip(&IplImage(src));// org point==>top left befor show it 
#endif
	//cvResetImageROI(&IplImage(src));
	namedWindow( source_window, CV_WINDOW_AUTOSIZE );
	imshow( source_window, src);
	/// 等待用户按任意按键退出程序
	waitKey(0);

	return 0;
}


// 显示矩阵
#if 0
void PrintMat(CvMat* A)
{
	int i,j;
	//printf("\nMatrix = :");
	for(i=0;i<A->rows;i++)
	{
		printf("\n");

		switch( CV_MAT_DEPTH(A->type) )
		{
		case CV_32F:
		case CV_64F:
			for(j=0;j<A->cols;j++)
				printf("%9.3f ", (float) cvGetReal2D( A, i, j ));
			break;
		case CV_8U:
		case CV_16U:
			for(j=0;j<A->cols;j++)
				printf("%6d",(int)cvGetReal2D( A, i, j ));
			break;
		default:
			break;
		}
	}
	printf("\n");
}
#endif