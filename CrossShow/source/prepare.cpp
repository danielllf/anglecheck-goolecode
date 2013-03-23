#include "../include/prepare.h"
#include "../include/llfutility.h"
#include "LineImage.h"
int whiteLineWeight = 50;
//imgPitch:Ҫ��ǰ֪��ͼ���pitch
//1��vertical����,sum���仯ʱ����Ϊ�ҵ���vertical line
//����x-cordiante of the found bar
//targetPos:Ŀ��ͼ��������ˣ��жˣ��Ҷ�
int getX_cordinateofVerticalBar(IplImage *bineryImg,int imgPitch,float verticalbarTimesOfpictch,int targetPos, int y_cordinate,int x_cordStart,bool showResultMark)
{
	//1.cal the vertical bar length an y_cordinate
	int verticabarLen = verticalbarTimesOfpictch*imgPitch;
	if (verticabarLen+y_cordinate>=bineryImg->height)
	{
		verticabarLen = bineryImg->height-y_cordinate;
	}
	int maxlienCnt = bineryImg->height/verticabarLen;

	//2.
	LineImage lineimgObj(cvGetSize(bineryImg),1,whiteLineWeight,LINE_VERTICAL);

	lineimgObj.resetImageLine(cvPoint(0,10),verticabarLen);//just for getting sumLineimg,10��Ϊ��������
	int sumLineimg = (int)(cvSum(lineimgObj.getImage()).val[0]);
	int sumAtlinePos=0;
#define LINEPOS_ONLINE 0
#define LINEPOS_OFFLINE 1
	  int linepos = LINEPOS_OFFLINE;
	  int findcout = 0;
		for (int i=x_cordStart;i<bineryImg->width;++i)
		{
			lineimgObj.resetImageLine(cvPoint(i,y_cordinate),verticabarLen);
			sumAtlinePos=getSumOfLineMask(bineryImg,lineimgObj.getImage());	

			if (sumAtlinePos==sumLineimg)
			{ 
					linepos = LINEPOS_ONLINE;
			}
			else
			{
				if (linepos==LINEPOS_ONLINE)
				{//online--> offlineʱ����line���ұ�Ե����
					findcout++;
					if(targetPos==TARGETPOS_MIDDLE)
					{
						if (findcout==1)
						{
							linepos = LINEPOS_OFFLINE;
						    continue;
						}
					}
					if(showResultMark)
					{
						IplImage *rlt = cvCreateImage(cvGetSize(bineryImg), 8,1);
						cvCopyImage(bineryImg,rlt);
						cvLine(rlt,cvPoint(i-5,y_cordinate),cvPoint(i+5,y_cordinate),cvScalar(125),3);
						IMG_SHOW("rlt",rlt);
						cvWaitKey();
					}
					return i;
				}
				linepos = LINEPOS_OFFLINE;
			}
		}

	return -1;//not found
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
	cvThreshold( img, dst,thresholdBW, 255.0, CV_THRESH_BINARY ); //ȡ��ֵ��ͼ��תΪ��ֵͼ��
	if(isShowResultImg)
	{
		cvShowImage("MorphologyImgBinery", dst);
		cvWaitKey();
	}
	cvReleaseImage(&temp);
	cvReleaseImage(&img);

	return dst;
}