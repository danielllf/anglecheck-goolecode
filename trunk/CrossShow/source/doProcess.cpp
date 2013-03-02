
#include "../include/global_def.h"
#include "../include/headers.h"
#include "../include/llfutility.h"
unsigned char cmpLineLen=500;
float Purfactor = 0.9;
int continuesLinecount = 6;//�����ȵ������ߵ�����
int safeVoidpitchCycleCntIngetStartLine = 2;//�ڼ���cmp starlineʱ��ȥ��ͼƬ���ļ���cycle,��ֹѭ���������Ϊpitch��һ��ͳ�ƾ�ֵ����һ��׼ȷ��
//��ֵ���δ�ҵ�������0.05��ʼ����̨��0.05���ϵ�����ֱ���ҵ�Ϊֹ
float continuesTol=0.03;//������ʱ<0.05�����������߱���Ϊ�������Ҿ���ʱ��sdv/mean,ԽСԽ��


enum Pos_STATUS{
	 OnValley=0,
	 OnMoutain
};

//������lineimg��lineλ�ø����µ���ͼ��֮��,��lineΪmask
int getSumOfLineMask(IplImage* src,IplImage* lineImg)
{
	IplImage *rltimg = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
	cvSet(rltimg,cvScalar(0));
	 cvAdd(src,lineImg,rltimg,lineImg);
	 int sum = (int)cvSum(rltimg).val[0];
	 cvReleaseImage(&rltimg);
	return  sum;
}
void setImgLineGroup(IplImage* src,std::list<int> linelist)
{
	std::list<int>::iterator it=linelist.begin();
	for (it;it!=linelist.end();++it)
	{
		cvLine(src,cvPoint(0,*it),cvPoint(src->width,*it),cvScalar(255));
	}
}

int getLinePitchProcess(IplImage &src)
{
	LineImage lineimgObj(cvGetSize(&src) );

	int sumAtlinePos;
	CvScalar sumSrc = cvSum(&src);
	Pos_STATUS current_linePos,pre_linePos;
	CalcObjVector elem_calc;
	CalcObjList final_calc;
    LINEIFO elementLineInfo;//��С�����������SUM����һ����,���ռ������element 
	LINEIFO finalLineInfo;
	int src_height = cvGetSize(&src).height;

	lineimgObj.resetImageLine(cvPoint(0,10),cmpLineLen);//just for getting sumLineimg,10��Ϊ��������
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
			elem_calc.addone(sumAtlinePos,i);
			current_linePos = OnMoutain;
		}
		pre_linePos = current_linePos;
	}
#ifdef debug_ShowTime
	GetLocalTime( &sys ); 
	printf( "before purifydata...%4d/%02d/%02d %02d:%02d:%02d.%03d \n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 
#endif

	int purity = final_calc.PurifyTheData(Purfactor);

#ifdef debug_ShowTime
	GetLocalTime( &sys ); 
	printf( "after purifydata...%4d/%02d/%02d %02d:%02d:%02d.%03d \n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 
#endif

	std::list<int> line_list;
	int pitch=final_calc.getLinePitch(continuesLinecount,continuesTol,line_list);
#ifdef showLineGroup
		setImgLineGroup(&src,line_list);
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
	cvLine(src,cvPoint(startPT.x,findLine),cvPoint(startPT.x+lineLen-1,findLine),cvScalar(255));
#endif
	return findLine;
}
 int getShiftPos(IplImage *src,int linePitch, double pitchTol,int lineCntInGroup,int vectElementCount,vectorPoint &vect)
{
	LineImage lineimgObj(cvGetSize(src) );
	int lineLen = src->width/vectElementCount;
	int pitchTolInt = (int)linePitch*pitchTol;
	//find the startline 
   int starLine = getTheStarLine(src,lineimgObj,linePitch,lineCntInGroup,lineLen);
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