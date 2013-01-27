
#include "mather.h"
#include "utility.h"

//mather中，模板图中的多个子矩形区域中的一个叫做：template

Mather::Mather(IplImage *src,float scale)
{
		initParms();
		 m_mather = src;
		if(scale!=1.0)
		{ 
			 m_mather  = g_resizeImage(m_mather,scale);
		}
		// m_mather = cvCreateImage(cvSize(src->width,src->height),src->depth, src->nChannels);
		//cvCopy(src,m_mather);

		m_cpsrc = cvCreateImage(cvSize(m_mather->width,m_mather->height),m_mather->depth, m_mather->nChannels);
		cvCopy(m_mather,m_cpsrc);

		//m_maxTol = 90;//to be changed by future
		m_iterTemplate = 0;
		m_minCycle = 0;
		m_sampleHdrMatchLine = 0;
		m_sampleHdrMatchLine1st = 0;
		m_totTmplateNum = 0;
		m_sampleLeftShinked  = 0;
		
}
Mather::~Mather()
{
		cvReleaseImage(&m_mather);
}
IplImage *Mather::getImage()
{
	return m_mather;
}

void Mather::setTmplateNum(int num)
{
	m_totTmplateNum = num;
	m_sample->setPatchNum(num);
}

CvPoint Mather::findMatchPointTogetCycle(IplImage *subSrc, MatchResult &result,int nMethods, int priorityMethod, bool useNormed)
{
	IplImage *ftmp[6]={NULL};
	//int method=0;
	int patchx = m_samplePatchCopy->width;
	int patchy = m_samplePatchCopy->height;
	int iwidth = subSrc->width - patchx + 1;
	int iheight = subSrc->height - patchy + 1;
	if(nMethods==1)
	{//priority

		ftmp[priorityMethod] = cvCreateImage( cvSize(iwidth,iheight),32,1);		
		if(useNormed)
		{
			ftmp[priorityMethod+1] = cvCreateImage( cvSize(iwidth,iheight),32,1);		
		}

	}
	else if (nMethods==2)
	{//priority+1  method:0,2,4
		//int anotherMethod =((priorityMethod + 2) > 4) ?(priorityMethod - 2):(priorityMethod + 2);
		int anotherMethod =(CV_TM_SQDIFF==priorityMethod) ?(CV_TM_CCOEFF):(CV_TM_SQDIFF);

		ftmp[priorityMethod] = cvCreateImage( cvSize(iwidth,iheight),32,1);		
		ftmp[anotherMethod] = cvCreateImage( cvSize(iwidth,iheight),32,1);	
		if(useNormed)
		{
			ftmp[priorityMethod+1] = cvCreateImage( cvSize(iwidth,iheight),32,1);
			ftmp[anotherMethod+1] = cvCreateImage( cvSize(iwidth,iheight),32,1);	
		}
	}
	else
	{//use all
		for(int i=0; i<6; ++i){
			if(!useNormed&&(1==i||3==i||5==i))continue;
			ftmp[i] = cvCreateImage( cvSize(iwidth,iheight),32,1);
		}
	}

	//DO THE MATCHING OF THE TEMPLATE WITH THE IMAGE
	CvPoint resultPoints[6];
	double resultValue[6];
#ifndef IMPOSSIBLE_CORRDINATE
#define IMPOSSIBLE_CORRDINATE -1
#define IMPOSSIBLE_VALUE 0XFFFFFFFFFFFFFFF
#endif
	const CvPoint impossiblePt = cvPoint(IMPOSSIBLE_CORRDINATE,IMPOSSIBLE_CORRDINATE);

	for(int i=0; i<6; ++i){
		resultPoints[i] = impossiblePt;//
		resultValue[i]=IMPOSSIBLE_VALUE;// init to impossible value

		if(ftmp[i]==NULL)continue;
		cvMatchTemplate( subSrc, m_samplePatchCopy, ftmp[i], i); 
		double min,max;
		CvPoint maxPoint,minPoint;
		cvMinMaxLoc(ftmp[i],&min,&max,&minPoint, &maxPoint);
		//printf("method:%d,min:%.2f,max:%.2f,minpt:(%d,%d),maxpt(%d,%d)\n",i,min,max,minPoint.x,minPoint.y,maxPoint.x,maxPoint.y);
		if (i <= 1){
			resultPoints[i] = minPoint;
			resultValue[i] = min;
		}
		else{
			resultPoints[i] = maxPoint;
			resultValue[i] = max;
		}
		//printf("(::%d,%d)\t",resultPoints[i]);

		//printf("method:%d, find maxpoint(%d,%d), minPoint(%d,%d), min:%.3f,Max:%.3f\n",i,maxPoint.x,maxPoint.y, minPoint.x, minPoint.y, min, max);

	}

	//find the most likly point

	int markArry[6] = {0};
	for (int i=0;i<6;++i)
	{
		//printf("(%d,%d)\t",resultPoints[i]);
		if (resultPoints[i].x == impossiblePt.x&&resultPoints[i].y == impossiblePt.y)continue;
		else
		{
			if(markArry[i]==0)//0 代表尚未查找过自值
			{
				for (int j=i+1;j<6;j++)
				{
					if(resultPoints[j].x==resultPoints[i].x&&resultPoints[j].y==resultPoints[i].y)
					{

						markArry[i] += 1;
						resultPoints[j] = impossiblePt;

					}

				}
				markArry[i] +=1;//自己一个也要算上
			}
		}

	}
	//find which index has the most number of pt
	int temIter = 0;//the index we want to find
	int temp=markArry[0];
	for (int i=0;i<6;i++)
	{
		if(resultPoints[i].x == impossiblePt.x&&resultPoints[i].y == impossiblePt.y)continue;
		if (markArry[i]>=temp)//加上“=”，则当有相同项时，会优先选择后面的方法(宏定义值更高的方法)。
		{
			temp = markArry[i];
			temIter = i;
		}
		printf("markarry i:%d,count:%d\n",i,markArry[i]);
		result.besetMethod = temIter;
	}
	//release img
	for (int i = 0;i<6;i++)
	{
		if (ftmp[i]!=NULL)
		{
			cvReleaseImage(&ftmp[i]);
		}

	}
	//cvReleaseImage(&subSrc);
	//printf("pos:%d,cnt:%d\n",temIter,temp);
	m_relativePt = resultPoints[temIter];
	//find the min or max "matched value" this index have
	switch(temIter)
	{
	case 0:
	case 1:
				result.minValue = resultValue[temIter];
				break;
	default:
			result.maxValue = resultValue[temIter];
			break;		
	}
	CvPoint abspt = cvPoint(m_relativePt.x+m_matherRect.x, m_relativePt.y+m_matherRect.y);
	result.pt = abspt;
	return abspt;
}

CvPoint Mather::findMatchPoint(IplImage *subSrc, int nMethods, int priorityMethod, bool useNormed)
{
	IplImage *ftmp[6]={NULL};
	//int method=0;
	int patchx = m_samplePatchCopy->width;
	int patchy = m_samplePatchCopy->height;
	int iwidth = subSrc->width - patchx + 1;
	int iheight = subSrc->height - patchy + 1;
	if(nMethods==1)
	{//priority
		ftmp[priorityMethod] = cvCreateImage( cvSize(iwidth,iheight),32,1);		
		if(useNormed)
		{
			ftmp[priorityMethod+1] = cvCreateImage( cvSize(iwidth,iheight),32,1);		
		}

	}
	else if (nMethods==2)
	{//priority+1  method:0,2,4
		//int anotherMethod =((priorityMethod + 2) > 4) ?(priorityMethod - 2):(priorityMethod + 2);
		int anotherMethod =(CV_TM_SQDIFF==priorityMethod) ?(CV_TM_CCOEFF):(CV_TM_SQDIFF);

		ftmp[priorityMethod] = cvCreateImage( cvSize(iwidth,iheight),32,1);		
		ftmp[anotherMethod] = cvCreateImage( cvSize(iwidth,iheight),32,1);	
		if(useNormed)
		{
			ftmp[priorityMethod+1] = cvCreateImage( cvSize(iwidth,iheight),32,1);
			ftmp[anotherMethod+1] = cvCreateImage( cvSize(iwidth,iheight),32,1);	
		}
	}
	else
	{//use all
		for(int i=0; i<6; ++i){
			if(!useNormed&&(1==i||3==i||5==i))continue;
			ftmp[i] = cvCreateImage( cvSize(iwidth,iheight),32,1);
		}
	}

	//DO THE MATCHING OF THE TEMPLATE WITH THE IMAGE
	CvPoint resultPoints[6];
#ifndef IMPOSSIBLE_CORRDINATE
#define IMPOSSIBLE_CORRDINATE -1
#endif
	const CvPoint impossiblePt = cvPoint(IMPOSSIBLE_CORRDINATE,IMPOSSIBLE_CORRDINATE);

	for(int i=0; i<6; ++i){
		resultPoints[i] = impossiblePt;//

		if(ftmp[i]==NULL)continue;
		cvMatchTemplate( subSrc, m_samplePatchCopy, ftmp[i], i); 
		double min,max;
		CvPoint maxPoint,minPoint;
		cvMinMaxLoc(ftmp[i],&min,&max,&minPoint, &maxPoint);
		if (i <= 1){
			resultPoints[i] = minPoint;
		}
		else{
			resultPoints[i] = maxPoint;
		}
		//printf("(::%d,%d)\t",resultPoints[i]);

		//printf("method:%d, find maxpoint(%d,%d), minPoint(%d,%d), min:%.3f,Max:%.3f\n",i,maxPoint.x,maxPoint.y, minPoint.x, minPoint.y, min, max);

	}

	//find the most likly point

	int markArry[6] = {0};
	for (int i=0;i<6;++i)
	{
		//printf("(%d,%d)\t",resultPoints[i]);
		if (resultPoints[i].x == impossiblePt.x&&resultPoints[i].y == impossiblePt.y)continue;
		else
		{
			if(markArry[i]==0)//0 代表尚未查找过自值
			{
				for (int j=i+1;j<6;j++)
				{
					if(resultPoints[j].x==resultPoints[i].x&&resultPoints[j].y==resultPoints[i].y)
					{

						markArry[i] += 1;
						resultPoints[j] = impossiblePt;

					}

				}
				markArry[i] +=1;//自己一个也要算上
			}
		}

	}
	int temIter = 0;
	int temp=markArry[0];
	for (int i=0;i<6;i++)
	{
		if(resultPoints[i].x == impossiblePt.x&&resultPoints[i].y == impossiblePt.y)continue;
		if (markArry[i]>=temp)
		{
			temp = markArry[i];
			temIter = i;
		}
//printf("markarry i:%d,count:%d\n",i,markArry[i]);
	}
	//release img
	for (int i = 0;i<6;i++)
	{
		if (ftmp[i]!=NULL)
		{
			cvReleaseImage(&ftmp[i]);
		}

	}
	//cvReleaseImage(&subSrc);
	//printf("pos:%d,cnt:%d\n",temIter,temp);
	m_relativePt = resultPoints[temIter];
	CvPoint abspt = cvPoint(m_relativePt.x+m_matherRect.x, m_relativePt.y+m_matherRect.y);
	return abspt;
}

int Mather::findMinCycle(int stripHeight,int startLine,int assumedMinMinCycle, int nMethods, int priorityMethod, bool useNormed)
{
	
	IplImage *stripImg = cvCreateImageHeader(cvSize(m_mather->width,stripHeight),m_mather->depth,m_mather->nChannels);
	stripImg->imageData = m_mather->imageData+m_mather->widthStep*startLine;
	stripImg->origin = m_mather->origin;
	stripImg->widthStep = m_mather->widthStep;
	
	//copy data to m_samplePatchCopy
	fetchSamplePatchImg(stripImg);//get patch from sample,for this function, only has one patch
	cvReleaseImageHeader(&stripImg);
	CvRect stripRect = cvRect(0,startLine,m_mather->width,stripHeight);
	CvPoint pt = GetCycle(stripRect, assumedMinMinCycle, nMethods,  priorityMethod,  useNormed);
	int cycleHeight = pt.y - startLine;

	
	mkLineColor(startLine,1);
	mkLineColor(startLine+cycleHeight,1);
	m_minCycle = cycleHeight;
	return cycleHeight;
}
int Mather::findSampleHdrPos( int widthDiv , int deltWidthShift,int nMethods, int priorityMethod, bool useNormed)
{
	int firstHdrPos = findSampleHdrPosWithin1stCycle(widthDiv,deltWidthShift,nMethods,priorityMethod,useNormed);
	int last_samplepatch_x_cordinate = m_sample->getLastPatch_x_cordinate();
	int lowerHdrPos = firstHdrPos;
	CvRect rect;
	while (1)
	{
		//calc if the container's up edge is 0
		 rect = getSamplePatchContainerLast(last_samplepatch_x_cordinate,lowerHdrPos);

		if (rect.y>0)
		{
			break;
		}
		if(rect.height+rect.y>=m_mather->height)
		{
			lowerHdrPos -= m_minCycle;
			break;
		}
		//move to the next cycle
		lowerHdrPos+=m_minCycle;
		Sleep(10);
	}
	m_sampleHdrMatchLine = lowerHdrPos;
	return m_sampleHdrMatchLine;

}
int Mather::findSampleHdrPosWithin1stCycle(int widthDiv , int deltWidthShift,int nMethods, int priorityMethod, bool useNormed)
{
	assert(m_sample->getImage()->width<=m_mather->width&&m_sample->getImage()->height<=m_mather->height);

	int realwidthShift = deltWidthShift+m_sampleLeftShinked;//须加上因sample shrink而导致的左边丢失。在match时，需将丢边考虑在内。
	//get sample hdr rect
	int headerWidth = m_sample->getImage()->width/widthDiv;
	int headerHeight = m_sample->getImage()->height;
	//init constainer relate to 1st cycle
	int partContainerStartLine = 0;
	CvRect partContainer = cvRect(0,partContainerStartLine,headerWidth,headerHeight);

		IplImage *stripImg = cvCreateImageHeader(cvSize(partContainer.width,partContainer.height),m_sample->getImage()->depth,m_sample->getImage()->nChannels);
		stripImg->imageData = m_sample->getImage()->imageData+m_sample->getImage()->widthStep*partContainer.y;
		stripImg->origin = m_sample->getImage()->origin;
		stripImg->widthStep = m_sample->getImage()->widthStep;
		fetchSamplePatchImg(stripImg);//get patch from sample
		cvReleaseImageHeader(&stripImg);

		CvPoint pt = MatchHead(partContainer, realwidthShift ,nMethods,priorityMethod,useNormed);
		m_sampleHdrMatchLine1st = pt.y;
		return pt.y;
}
//this template mem is hold and released by mather objet manualy
//duplicate of sample rect,after create this, mather will not need sample in this turn
void Mather::fetchSamplePatchImg(IplImage *samplePatch)
{
	m_samplePatchCopy = cvCreateImage(cvSize(samplePatch->width, samplePatch->height),samplePatch->depth, samplePatch->nChannels);
	cvCopy(samplePatch,m_samplePatchCopy);
	m_samplePatchSize = cvGetSize(m_samplePatchCopy);
	
}

void Mather::releaseSamplePatchCopy(IplImage **patchCopy)
{
		cvReleaseImage(patchCopy);
}


IplImage * Mather::getSubTmplateImg( IplImage *src, CvRect samplePatchContainer)
{
	return g_CopyRectFromImg(src,  samplePatchContainer);
}

//max angle should < 20 degree
void Mather::setMaxAngle(int angle)
{
	assert(angle<20);//不使用大于20度的角
	m_maxAngle = angle;
	m_deltWidth = (m_mather->height)*sin(m_maxAngle*CV_PI/180);//对于理想情况左边扇出较大

}
CvRect Mather::getSamplePatchContainerLast(int sampleRect_x_cordinate,int matchedLineinCycle)
{
	//assert(matchedLineinCycle!=0);
	int upEge = 0;
	int leftEge = 0;
	int rightEge = 0;
	int bottomEge = 0;

	int deltHeight = (sampleRect_x_cordinate+m_samplePatchSize.width)*sin(m_maxAngle*CV_PI/180);
	//up
	if ((upEge = matchedLineinCycle-deltHeight)<=0){
		upEge = 0;
	}
	//left
	if (( leftEge = sampleRect_x_cordinate-m_deltWidth)<=0){
		leftEge = 0;
	}
	//right
	if((rightEge=m_samplePatchSize.width+sampleRect_x_cordinate+m_deltWidth)>= m_mather->width){
		rightEge=m_mather->width;
	}
	//bottom
	if((bottomEge = matchedLineinCycle+m_samplePatchSize.height+deltHeight)>=m_mather->height){
		bottomEge = m_mather->height;
	}

	return cvRect(leftEge, upEge, rightEge-leftEge,bottomEge-upEge);
}
CvRect Mather::getSamplePatchContainer(int sampleRect_x_cordinate)
{//上下左右四个方向都留有余量

	int upEge = 0;
	int leftEge = 0;
	int rightEge = 0;
	int bottomEge = 0;
	
	//int deltHeight = (sampleRect_x_cordinate+m_samplePatchSize.width)*sin(m_maxAngle*CV_PI/180);
	//up
	int sampleHdrMatchLine;
	if (m_iterTemplate==0)
	{
		sampleHdrMatchLine = m_sampleHdrMatchLine;
	}
	else
	{
		sampleHdrMatchLine = m_absolutePt.y;
	}
	if ((upEge = sampleHdrMatchLine-m_minCycle/2)<=0){
		upEge = 0;
	}
	//left
	if (( leftEge = sampleRect_x_cordinate-m_deltWidth)<=0){
		leftEge = 0;
	}
	//right
	if((rightEge=m_samplePatchSize.width+sampleRect_x_cordinate+m_deltWidth)>= m_mather->width){
			rightEge=m_mather->width;
	}
	//bottom
	if((bottomEge = sampleHdrMatchLine+m_samplePatchSize.height+m_minCycle/2)>=m_mather->height){
		bottomEge = m_mather->height;
	}

	m_matherRect =cvRect(leftEge, upEge, rightEge-leftEge,bottomEge-upEge);
	return m_matherRect;

	
}

CvPoint Mather::MatchHead(CvRect stripHdrRect, int  deltWidth, int nMethods, int priorityMethod, bool useNormed)
{
	
	assert(m_minCycle!=0);//&&m_minCycle>stripHdrRect.height);
	//int hdrRectHeight = stripHdrRect.x+m_minCycle;
	int matherRect_y = stripHdrRect.y+m_minCycle+stripHdrRect.height;//考虑stripHdrRect可能大于cycle，需加上stripHdrRect.height
	if(matherRect_y>m_mather->height)matherRect_y = m_mather->height;
	//get container
	m_matherRect = cvRect(stripHdrRect.x, stripHdrRect.y,stripHdrRect.width+deltWidth, matherRect_y);
	IplImage *subSrc = getSubTmplateImg(m_mather,m_matherRect);
	//g_showImgRect(getImage(),m_matherRect);
	CvPoint abspt = findMatchPoint(subSrc,nMethods,priorityMethod,useNormed);
	cvReleaseImage(&subSrc);

	return  abspt;
}


CvPoint Mather::GetCycle(CvRect stripRect, int assumedMinCycle, int nMethods, int priorityMethod, bool useNormed)
{
	//CalcMatherRect(sampleRect);
	m_matherRect = cvRect(stripRect.x, stripRect.y+assumedMinCycle,stripRect.width,m_mather->height-assumedMinCycle-stripRect.y);
	IplImage *subSrc = getSubTmplateImg(m_mather,m_matherRect);
	MatchResult matchResult;
	CvPoint abspt = findMatchPointTogetCycle(subSrc,matchResult, nMethods, priorityMethod, useNormed);
	//printf("bestmothod:%d,min:%.2f,max:%.2f\n",matchResult.besetMethod,matchResult.minValue,matchResult.maxValue);
	
	return  abspt;
}

CvPoint Mather::MatchPatch(int sampleRect_x_cordinate, int nMethods, int priorityMethod, bool useNormed)
{
	
	IplImage *subSrc = getSubTmplateImg(m_mather,getSamplePatchContainer( sampleRect_x_cordinate));

    m_absolutePt = findMatchPoint( subSrc, nMethods,  priorityMethod,  useNormed);//save pt to m_absolutePt
	cvReleaseImage(&subSrc);
	int factor=1;
	if (0==m_iterTemplate)
	{
		m_leftMatchPt = m_absolutePt;
	}
	else if (m_totTmplateNum-1==m_iterTemplate)
	{
		m_rightMatchPt = m_absolutePt;
		if (m_rightMatchPt.y-m_leftMatchPt.y<0)
		{
			factor = -1;
		}
		m_resultAngle = factor * cvFastArctan(abs(m_rightMatchPt.y-m_leftMatchPt.y),m_rightMatchPt.x-m_leftMatchPt.x);

	}
	m_iterTemplate+=1;
	return  m_absolutePt;
}
double Mather::getFindAngle()
{
	return m_resultAngle;
}
//color the rect
void Mather::markTemplateRect()
{	
	cvSetImageROI(m_cpsrc,m_matherRect);
	if(m_iterTemplate%2==0)
		cvAddS(m_cpsrc,cvScalar(ADD_VALUE),m_cpsrc);
	else
		cvAddS(m_cpsrc,cvScalar(-ADD_VALUE),m_cpsrc);
	cvResetImageROI(m_cpsrc);
}
void Mather::showPic()
{
	cvNamedWindow("mather");
	cvShowImage("mather",m_cpsrc);

}
void Mather::mkLineColor(int starLine, int lineWight,int lineColor)
{
	cvSetImageROI(m_cpsrc,cvRect(0,starLine,m_mather->width,lineWight));
	cvSet(m_cpsrc,cvScalar(lineColor));
	cvResetImageROI(m_cpsrc);
}

void Mather::catchSample(Sample *sample)
{
	m_sample = sample;
}
void Mather::getSampleShinked(int left, int right, int top, int bottom)
{
	m_sampleLeftShinked = m_sample->shrinkSample(left,right,top,bottom);
	
}
