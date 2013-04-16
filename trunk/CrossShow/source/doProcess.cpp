
#include "../include/global_def.h"
#include "../include/headers.h"
#include "../include/llfutility.h"
#include "../include/prepare.h"
unsigned char cmpLineLen=40;
float Purfactor = 0.9;
int continuesLinecount = 4;//间距均匀的连续平行线的条数
int safeVoidpitchCycleCntIngetStartLine = 2;//在计算cmp starline时，去除图片最后的几个cycle,防止循环溢出。因为pitch是一个统计均值，不一定准确。
//此值如果未找到，则会从0.05开始，以台阶0.05向上递增，直到找到为止
float continuesTol=0.03;//（设置时<0.05）连续几条线被视为是连续且均匀时的sdv/mean,越小越好
int lineThickness=1;
double allowedPercentTOLwhenShifting = 0.5;//寻求最佳覆盖位置时的上下偏移量，相对于pitch的百分比

enum Pos_STATUS{
	 OnValley=0,
	 OnMoutain
};


void setImgLineGroup(IplImage* src,std::list<int> linelist)
{
	std::list<int>::iterator it=linelist.begin();
	for (it;it!=linelist.end();++it)
	{
		cvLine(src,cvPoint(0,*it),cvPoint(src->width,*it),cvScalar(255));
	}
}

int getLinePitchProcess(IplImage *src)
{
	LineImage lineimgObj(cvGetSize(src),lineThickness,whiteLineWeight);

	int sumAtlinePos;
	CvScalar sumSrc = cvSum(src);
	Pos_STATUS current_linePos,pre_linePos;
	CalcObjVector elem_calc;
	CalcObjList final_calc;
    LINEIFO elementLineInfo;//很小区域内算出的SUM最大的一条线,是终极计算的element 
	LINEIFO finalLineInfo;
	int src_height = cvGetSize(src).height;

	lineimgObj.resetImageLine(cvPoint(0,10),cmpLineLen);//just for getting sumLineimg,10可为任意数字
	int sumLineimg = (int)(cvSum(lineimgObj.getImage()).val[0]);

	for (int i=0;i<src_height;++i)
	{

		lineimgObj.resetImageLine(cvPoint(0,i),cmpLineLen);
		
		sumAtlinePos=getSumOfLineMask(src,lineimgObj.getImage());		

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
			elem_calc.addone(sumAtlinePos,i);
			current_linePos = OnMoutain;
		}
		pre_linePos = current_linePos;
	}
 
	log_process("before purifydata\n");
 

	int purity = final_calc.PurifyTheData(Purfactor);
	if(purity==-1)
	{log_erro("purify failed\n");return -1;}
	log_process("after purifydata\n");


	std::list<int> line_list;
	int pitch=final_calc.getLinePitch(continuesLinecount,continuesTol,line_list);
#ifdef showLineGroup
		setImgLineGroup(src,line_list);
#endif
   
	return pitch;
}

	//find the startline 
int getTheStarLine(IplImage *src,LineImage &lineimgObj,int linePitch,int lineCntInGroup,int lineLen)
{
	CalcObjVector vectObj;
	int searchEndline = src->height-linePitch*safeVoidpitchCycleCntIngetStartLine;

	for (int i=0;i<searchEndline;++i)
	{
		lineimgObj.resetImageLineGroup(cvPoint(0,i),lineLen,lineCntInGroup,linePitch);

		int sum=getSumOfLineMask(src,lineimgObj.getImage());
		vectObj.addone(sum,i);
	}
		int TheLine = vectObj.getTheMaxElement_Linenum();
	return TheLine;
}

int getMaxLineGroupSumLineWithinTol(IplImage *src,LineImage &lineimgObj,int linePitch,int lineCntInGroup,
								int lineLen,CvPoint startPT,int  pitchTolInt)
{
	CalcObjVector vectObj;
	int uperLineLimit = startPT.y-pitchTolInt;
	int botLineLimit = startPT.y+pitchTolInt;
	int sum=0;
	for (int i=uperLineLimit;i<=botLineLimit;++i)
	{
		lineimgObj.resetImageLineGroup(cvPoint(startPT.x,i),lineLen,lineCntInGroup,linePitch);
#ifdef ShowLineSectGroup
		IMG_SHOW("tmp",lineimgObj.getImage());cvWaitKey();
#endif
		

	    sum=getSumOfLineMask(src,lineimgObj.getImage());
		vectObj.addone(sum,i);
	}
	int findLine = vectObj.getTheMaxElement_Linenum();
#ifdef ShowLineSect
	cvLine(src,cvPoint(startPT.x,findLine),cvPoint(startPT.x+lineLen-1,findLine),cvScalar(255),lineThickness);
#endif
	return findLine;
}
 int getShiftPos(IplImage *src,int linePitch, double pitchTol,int lineCntInGroup,int &vectElementCount,vectorPoint &vect)
{
	LineImage lineimgObj(cvGetSize(src) , lineThickness,whiteLineWeight);
	int lineLen = 0;
	while(1)
	{
		 lineLen = src->width/vectElementCount;

		if (lineLen<1.1*linePitch)// 至少要能覆盖2个点
		{
			vectElementCount--;

			continue;
		}
		break;
	}

	int pitchTolInt = (int)linePitch*pitchTol;
	//find the startline 
   int starLine = 2*src->height/3;//getTheStarLine(src,lineimgObj,linePitch,lineCntInGroup,lineLen);

   log_process("after getTheStarLine\n");


   int preLine=starLine;
   CvPoint startPt;
	for (int i=0;i<vectElementCount;++i)
	{
		startPt.x = i*lineLen;
		startPt.y = preLine;
		preLine = getMaxLineGroupSumLineWithinTol(src,lineimgObj,linePitch,lineCntInGroup,lineLen,startPt,pitchTolInt);
		vect.push_back(cvPoint(startPt.x,preLine));
	}
   return starLine;
}

 int getShiftPosProcess(IplImage* src,vectorPoint &rltvec,CORDINATE_PAIR edgePair,int secCnt,bool isAdaptiveThres,int thresholdBW, int achorCordnate)
 {

	IplImage *img =g_CopyRectFromImg(src,cvRect(edgePair.left_cordinate,0,edgePair.right_cordinate-edgePair.left_cordinate,src->height));
	
	 IplImage* morphImg=getMorphologyImg(img,CV_MOP_TOPHAT,isAdaptiveThres,thresholdBW,achorCordnate);
	
	int pitch= 11;//getLinePitchProcess(morphImg);
	if(pitch==-1)
	{
		log_erro("getpich failed\n");
		return -1;
	}
	log_process("pitch:%d\n",pitch);

	int startline = getShiftPos(morphImg,pitch, allowedPercentTOLwhenShifting,continuesLinecount,secCnt,rltvec);
	
	printVecPoint(rltvec);
	log_process("after getShiftPos,start line:%d\n",startline);


	//cvWaitKey();
		IMG_SHOW("morphImg",morphImg);
		//cvSaveImage("midrlt.jpg",morphImg);	
	//cvWaitKey();
		
	cvReleaseImage(&morphImg);
	cvReleaseImage(&img);
	return 0;
 }