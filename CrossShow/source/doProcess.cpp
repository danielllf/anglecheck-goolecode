
#include "../include/global_def.h"
#include "../include/headers.h"
#include "../include/llfutility.h"
#include "../include/prepare.h"
unsigned char cmpLineLen=40;
float Purfactor = 0.9;
int continuesLinecount = 4;//�����ȵ�����ƽ���ߵ�����
int safeVoidpitchCycleCntIngetStartLine = 2;//�ڼ���cmp starlineʱ��ȥ��ͼƬ���ļ���cycle,��ֹѭ���������Ϊpitch��һ��ͳ�ƾ�ֵ����һ��׼ȷ��
//��ֵ���δ�ҵ�������0.05��ʼ����̨��0.05���ϵ�����ֱ���ҵ�Ϊֹ
float continuesTol=0.03;//������ʱ<0.05�����������߱���Ϊ�������Ҿ���ʱ��sdv/mean,ԽСԽ��
int lineThickness=1;
double allowedPercentTOLwhenShifting = 0.5;//Ѱ����Ѹ���λ��ʱ������ƫ�����������pitch�İٷֱ�
int assumedMinLinePitchForCalcPitch = 20;
int MorphologyMethod= CV_MOP_BLACKHAT;//or CV_MOP_BLACKHAT //���ڿ�
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
    LINEIFO elementLineInfo;//��С�����������SUM����һ����,���ռ������element 
	LINEIFO finalLineInfo;
	int src_height = cvGetSize(src).height;

	lineimgObj.resetImageLine(cvPoint(0,10),cmpLineLen);//just for getting sumLineimg,10��Ϊ��������
	int sumLineimg = (int)(cvSum(lineimgObj.getImage()).val[0]);

	for (int i=0;i<src_height;++i)
	{

		lineimgObj.resetImageLine(cvPoint(0,i),cmpLineLen);
		////rect���������ټ���������Чͼ������,������������
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
		//��ȡ����ROI����
		int rectUper = i-linePitch<0?0:i-linePitch;
		int RoiRctHeight = (lineCntInGroup+1)*linePitch-rectUper>src->height?src->height-rectUper:(lineCntInGroup+1)*linePitch;
	CvRect calcRect = cvRect(0,rectUper,lineLen,RoiRctHeight);
		int sum=getSumOfLineMask(src,lineimgObj.getImage(),calcRect);
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
		////rect���������ټ���������Чͼ������
		int rectUper = startPT.y-linePitch;
		int RoiRctHeight = (lineCntInGroup+1)*linePitch+startPT.y>src->height?src->height-rectUper: (lineCntInGroup+1)*linePitch;
		CvRect calcRect = cvRect(startPT.x,rectUper,lineLen,RoiRctHeight);
	    sum=getSumOfLineMask(src,lineimgObj.getImage(),calcRect);
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

		if (lineLen<1.1*linePitch)// ����Ҫ�ܸ���2����
		{
			vectElementCount--;

			continue;
		}
		break;
	}

	int pitchTolInt = (int)linePitch*pitchTol;
	//find the startline 
log_process("before getTheStarLine\n");
   int starLine = getTheStarLine(src,lineimgObj,linePitch,lineCntInGroup,lineLen);
//int starLine = 2*src->height/3;
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

 int getShiftPosProcess(IplImage* src,vectorPoint &rltvec,int secCnt,bool isAdaptiveThres,int thresholdBW, int achorCordnate)
 {

	IplImage *img =src;
	 IplImage* morphImg=getMorphologyImg(img,MorphologyMethod,isAdaptiveThres,thresholdBW,achorCordnate);
	log_process("brfore get pitch\n");
	int pitch= getLinePitchProcess(morphImg);
	if(pitch==-1)
	{
		log_erro("getpich failed\n");
		return -1;
	}
	log_process("after,pitch:%d\n",pitch);

	int startline = getShiftPos(morphImg,pitch, allowedPercentTOLwhenShifting,continuesLinecount,secCnt,rltvec);
	
	printVecPoint(rltvec);
	log_process("after getShiftPos,start line:%d\n",startline);


	//cvWaitKey();
		IMG_SHOW("morphImg",morphImg);
		//cvSaveImage("midrlt.jpg",morphImg);	
	//cvWaitKey();
		
	cvReleaseImage(&morphImg);
	//cvReleaseImage(&img);
	return 0;
 }