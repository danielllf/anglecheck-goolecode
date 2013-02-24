#include "../include/doProcess.h"
#include "../include/LineImage.h"
int cmpLineLen=100;
enum Pos_STATUS{
	 OnValley=0,
	 OnMoutain
};
int getLinePitch(IplImage &src)
{
	LineImage lineimg(cvGetSize(&src) );
	IplImage* sumResult = cvCreateImage(cvGetSize(&src),src.depth,src.nChannels);
	cvSet(sumResult,cvScalar(0));
	CvScalar sum;
	CvScalar sumSrc = cvSum(&src);
	Pos_STATUS current_linePos,pre_linePos;
	pre_linePos=current_linePos = OnValley;
	CalcObj elem_calc;
	CalcObj final_calc;
    LINEIFO elementLineInfo;//很小区域内算出的SUM最大的一条线,是终极计算的element 
	LINEIFO finalLineInfo;
	int src_height = cvGetSize(&src).height;

	for (int i=0;i<src_height;++i)
	{
		lineimg.resetImageLine(cvPoint(0,i),cmpLineLen);
		cvAdd(&src,lineimg.getImage(),sumResult);
		sum=cvSum(sumResult);

		if(sum.val==sumSrc.val)
		{
			current_linePos = OnValley;
			if (pre_linePos==OnMoutain)
			{
				//end the calc for this small area
				elementLineInfo=elem_calc.getTheMaxElement();
				final_calc.addone(elementLineInfo.lineNum,elementLineInfo.sum);
			}
			continue;
		}
		else
		{
			//calc small area sum,get the max
			current_linePos = OnMoutain;
			if (pre_linePos==OnValley)
			{//clear for a new start
				elem_calc.clearAll();
			}
			elem_calc.addone(i,(int)sum.val);
		}
		pre_linePos = current_linePos;
	}
    //finalLineInfo=	final_calc.getTheMaxElement();
	cvReleaseImage(&sumResult);
	return -1;
}

