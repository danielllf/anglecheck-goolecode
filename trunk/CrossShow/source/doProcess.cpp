#include "../include/doProcess.h"
#include "../include/LineImage.h"
#include "../include/CalcObj.h"
unsigned char cmpLineLen=3;
float Purfactor = 0.9;
int continuesLinecount = 4;//间距均匀的连续线的条数
enum Pos_STATUS{
	 OnValley=0,
	 OnMoutain
};

//返回以lineimg中line位置覆盖下的两图像之和,以line为mask
int getSumOfLineMask(IplImage* src,IplImage* lineImg)
{
	IplImage *rltimg = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
	cvSet(rltimg,cvScalar(0));
	 cvAdd(src,lineImg,rltimg,lineImg);
	 int sum = (int)cvSum(rltimg).val[0];
	 cvReleaseImage(&rltimg);
	return  sum;
}
int getLinePitchProcess(IplImage &src)
{
	LineImage lineimgObj(cvGetSize(&src) );

	int sumAtlinePos;
	CvScalar sumSrc = cvSum(&src);
	Pos_STATUS current_linePos,pre_linePos;
	CalcObjVector elem_calc;
	CalcObjList final_calc;
    LINEIFO elementLineInfo;//很小区域内算出的SUM最大的一条线,是终极计算的element 
	LINEIFO finalLineInfo;
	int src_height = cvGetSize(&src).height;

	lineimgObj.resetImageLine(cvPoint(0,10),cmpLineLen);//just for getting sumLineimg,10可为任意数字
	int sumLineimg = (int)(cvSum(lineimgObj.getImage()).val[0]);

	for (int i=0;i<src_height;++i)
	{

		lineimgObj.resetImageLine(cvPoint(0,i),cmpLineLen);
		
		sumAtlinePos=getSumOfLineMask(&src,lineimgObj.getImage());		

		if(i==0)//init status at first
		{
			if(sumAtlinePos==sumLineimg)
				pre_linePos=current_linePos = OnValley;
			else
				pre_linePos=current_linePos = OnMoutain;
		}

		if(sumAtlinePos==sumLineimg)
		{
			if (pre_linePos==OnMoutain)//OnMoutain-->>OnValley
			{
				//end the calc for this small area
				elementLineInfo=elem_calc.getTheMaxElement();
				final_calc.addone(elementLineInfo.lineNum,elementLineInfo.value);

			}
			current_linePos = OnValley;
			pre_linePos = current_linePos;
			continue;
		}
		else
		{
			//calc small area sum,get the max
			if (pre_linePos==OnValley)
			{	
				//clear for a new start
				elem_calc.clearAll();
			}
			elem_calc.addone(i,sumAtlinePos);
			current_linePos = OnMoutain;
		}
		pre_linePos = current_linePos;
	}

	int purity = final_calc.PurifyTheData(Purfactor);
	int pitch=final_calc.getLinePitch(continuesLinecount);
	return pitch;
}

