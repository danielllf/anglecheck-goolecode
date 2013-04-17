#include "../include/prepare.h"
#include "../include/llfutility.h"
#include "LineImage.h"
int whiteLineWeight = 50;
//imgPitch:要提前知道图像的pitch
//verticalbarTimesOfpictch:适竖杠的的长度，要至少要大于imgPitch
//1个vertical短线,sum不变化时，认为找到了vertical line
//返回x-cordiante of the found bar
//subImageType:最左端（图中会有1个分割线），中端(图中会有2个分割线)
//已经无用，不推荐用
CORDINATE_PAIR getX_cordinateofVerticalBar(IplImage *bineryImg,int imgPitch,float verticalbarTimesOfpictch,int subImageType, int y_cordinate,int x_cordStart,bool showResultMark)
{
	CORDINATE_PAIR cordpair;
	cordpair.left_cordinate=-1;
	cordpair.right_cordinate=-1;

	//1.cal the vertical bar length and y_cordinate
	int verticabarLen = verticalbarTimesOfpictch*imgPitch;
	if (verticabarLen+y_cordinate>=bineryImg->height)
	{
		verticabarLen = bineryImg->height-y_cordinate;
	}
	int maxlienCnt = bineryImg->height/verticabarLen;

	//2.
	LineImage lineimgObj(cvGetSize(bineryImg),1,whiteLineWeight,LINE_VERTICAL);

	lineimgObj.resetImageLine(cvPoint(0,10),verticabarLen);//just for getting sumLineimg,10可为任意数字
	int sumLineimg = (int)(cvSum(lineimgObj.getImage()).val[0]);
	int sumAtlinePos=0;
#define LINEPOS_ONLINE 0
#define LINEPOS_OFFLINE 1
	  int linepos = LINEPOS_OFFLINE;
	  int findcout = 0;
		for (int i=x_cordStart;i<bineryImg->width;++i)
		{
			lineimgObj.resetImageLine(cvPoint(i,y_cordinate),verticabarLen);
			//此处假设竖直线的左右倾斜公差为+/-10*imgPitch
			CvSize size = cvGetSize(bineryImg);
			CvRect Roirect = cvRect(0,0,size.width,size.height);
			sumAtlinePos=getSumOfLineMask(bineryImg,lineimgObj.getImage(),Roirect);	

			if (sumAtlinePos==sumLineimg)
			{ 
					linepos = LINEPOS_ONLINE;
			}
			else
			{//offline now
				//online--> offline说明找到一条vertical bar。从line的右边缘结束
				if (linepos==LINEPOS_ONLINE)
				{
					findcout++;
					if(subImageType==TARGETTYPE_MIDDLE)
					{
						if (findcout==1)
						{
							cordpair.left_cordinate=i;
							linepos = LINEPOS_OFFLINE;
						    continue;
						}
						if (findcout==2)
						{
							cordpair.right_cordinate=i;
						}
					}
					if (subImageType==TARGETTYPE_LEFT)
					{
						cordpair.right_cordinate=i;
						cordpair.left_cordinate = 0;
					}
					if (subImageType==TARGETTYPE_RIGHT)
					{
						cordpair.left_cordinate=i;
						cordpair.right_cordinate = bineryImg->width;
					}
					if(showResultMark)
					{
						IplImage *rlt = cvCreateImage(cvGetSize(bineryImg), 8,1);
						cvCopyImage(bineryImg,rlt);
						cvLine(rlt,cvPoint(i-5,y_cordinate),cvPoint(i+5,y_cordinate),cvScalar(125),3);
						IMG_SHOW("rlt",rlt);
						cvWaitKey();
					}
					return cordpair;
				}
				linepos = LINEPOS_OFFLINE;
			}
		}

	return cordpair;//not found
}
IplImage *getMorphologyImg(IplImage* src,int operation,bool isAdaptiveThres,int thresholdBW, int achorCordnate)
{

	IplImage * temp = cvCreateImage(cvGetSize(src), 8,1);
	IplImage * img=cvCreateImage(cvGetSize(src), 8, 1);
	cvCopyImage(src,temp);
	cvCopyImage(src, img);

	IplConvKernel *element = 0; //定义形态学结构指针
	element = cvCreateStructuringElementEx(2*achorCordnate+1,2*achorCordnate+1, achorCordnate, achorCordnate, CV_SHAPE_RECT, 0);//3,5,7

	//“黑帽”CV_MOP_BLACKHAT
	//"白帽"CV_MOP_TOPHAT
	cvMorphologyEx(
		src,
		img,
		temp,
		element,// //default 3*3
		operation,//CV_MOP_BLACKHAT,
		1);

	IplImage* dst = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	if (isAdaptiveThres)
	{
		cvAdaptiveThreshold(img,dst,255.0,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,3,5);
	}
	else
	{
		cvThreshold( img, dst,thresholdBW, 255.0, CV_THRESH_BINARY ); //取阈值把图像转为二值图像
	}

		//cvShowImage("MorphologyImgBinery", dst);
		//cvWaitKey();

	cvReleaseImage(&temp);
	cvReleaseImage(&img);
	

	return dst;
}