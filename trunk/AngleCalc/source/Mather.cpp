
#include "mather.h"
#include "utility.h"

//mather�У�ģ��ͼ�еĶ���Ӿ��������е�һ��������template

Mather::Mather(IplImage *src)
{
		initParms();
		m_src = cvCreateImage(cvSize(src->width,src->height),src->depth, src->nChannels);
		cvCopy(src,m_src);

		m_cpsrc = cvCreateImage(cvSize(src->width,src->height),src->depth, src->nChannels);
		cvCopy(src,m_cpsrc);

		//m_maxTol = 90;//to be changed by future
		m_iterTemplate = 0;
		m_minCycle = 0;
		m_sampleHdrMatchLine = 0;
		m_totTmplateNum = 0;
}
Mather::~Mather()
{
		cvReleaseImage(&m_src);
}
void Mather::setTmplateNum(int num)
{
	m_totTmplateNum = num;
}
int Mather::findMinCycle(int stripHeight,int startLine)
{
	IplImage *stripImg = cvCreateImageHeader(cvSize(m_src->width,stripHeight),m_src->depth,m_src->nChannels);
	stripImg->imageData = m_src->imageData+m_src->widthStep*startLine;
	stripImg->origin = m_src->origin;
	stripImg->widthStep = m_src->widthStep;

	fetchSamplePatchImg(stripImg);//get patch from sample
	cvReleaseImageHeader(&stripImg);
	CvRect stripRect = cvRect(0,startLine,m_src->width,stripHeight);
	CvPoint pt = findMatchPointToGetCycle(stripRect,1,CV_TM_CCOEFF);
	int cycleHeight = pt.y - startLine;

	
	mkLineColor(startLine);
	mkLineColor(startLine+cycleHeight);
	m_minCycle = cycleHeight;
	return cycleHeight;
}

int Mather::findSampleHdrPos(IplImage *sample, CvRect stripHdrRect, int deltWidthShift)
{
	//get sample hdr rect
		IplImage *stripImg = cvCreateImageHeader(cvSize(stripHdrRect.width,stripHdrRect.height),sample->depth,sample->nChannels);
		stripImg->imageData = sample->imageData+sample->widthStep*stripHdrRect.y;
		stripImg->origin = sample->origin;
		stripImg->widthStep = sample->widthStep;
		fetchSamplePatchImg(stripImg);//get patch from sample
		cvReleaseImageHeader(&stripImg);

		CvPoint pt = findMatchPointInCycle(stripHdrRect, deltWidthShift ,1,CV_TM_CCOEFF);
		m_sampleHdrMatchLine = pt.y;
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
	assert(angle<20);//��ʹ�ô���20�ȵĽ�
	m_maxAngle = angle;
	m_deltWidth = (m_src->height)*sin(m_maxAngle*CV_PI/180);

}
CvRect Mather::getSamplePatchContainer(int sampleRect_x_cordinate)
{//���������ĸ�������������
	assert(m_sampleHdrMatchLine!=0);
	int upEge = 0;
	int leftEge = 0;
	int rightEge = 0;
	int bottomEge = 0;
	
	m_deltHeight = (sampleRect_x_cordinate+m_samplePatchSize.width)*sin(m_maxAngle*CV_PI/180);
	//up
	//if ((upEge = sampleRect.y-m_deltHeight)<=0){
	//	upEge = 0;
	//}
	if ((upEge = m_sampleHdrMatchLine-m_deltHeight)<=0){
		upEge = 0;
	}
	//left
	if (( leftEge = sampleRect_x_cordinate-m_deltWidth)<=0){
		leftEge = 0;
	}
	//right
	if((rightEge=m_samplePatchSize.width+sampleRect_x_cordinate+m_deltWidth)>= m_src->width){
			rightEge=m_src->width;
	}
	//bottom
	if((bottomEge = m_sampleHdrMatchLine+m_samplePatchSize.height+m_deltHeight)>=m_src->height){
		bottomEge = m_src->height;
	}

	m_matherRect =cvRect(leftEge, upEge, rightEge-leftEge,bottomEge-upEge);
	return m_matherRect;

	
}
CvPoint Mather::findMatchPointInCycle(CvRect stripHdrRect, int  deltWidth, int nMethods, int priorityMethod, bool useNormed)
{
	
	assert(m_minCycle!=0&&m_minCycle>stripHdrRect.height);
	int hdrRectHeight = stripHdrRect.x+m_minCycle;
	if(hdrRectHeight>m_src->height)hdrRectHeight = m_src->height;
	m_matherRect = cvRect(stripHdrRect.x, stripHdrRect.y,stripHdrRect.width+deltWidth, hdrRectHeight);
	IplImage *subSrc = getSubTmplateImg(m_src,m_matherRect);

	IplImage *ftmp[6]={NULL};
	int method=0;
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
		int anotherMethod =((priorityMethod + 2) > 4) ?(priorityMethod - 2):(priorityMethod + 2);

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
			if(markArry[i]==0)//0 ������δ���ҹ���ֵ
			{
				for (int j=i+1;j<6;j++)
				{
					if(resultPoints[j].x==resultPoints[i].x&&resultPoints[j].y==resultPoints[i].y)
					{

						markArry[i] += 1;
						resultPoints[j] = impossiblePt;

					}

				}
				markArry[i] +=1;//�Լ�һ��ҲҪ����
			}
		}

	}
	int temIter = 0;
	int temp=markArry[0];
	for (int i=0;i<6;i++)
	{
		if(resultPoints[i].x == impossiblePt.x&&resultPoints[i].y == impossiblePt.y)continue;
		if (markArry[i]>temp)
		{
			temp = markArry[i];
			temIter = i;
		}

	}
	//release img
	for (int i = 0;i<6;i++)
	{
		if (ftmp[i]!=NULL)
		{
			cvReleaseImage(&ftmp[i]);
		}

	}
	cvReleaseImage(&subSrc);
	//printf("pos:%d,cnt:%d\n",temIter,temp);
	m_relativePt = resultPoints[temIter];
	m_absolutePt = cvPoint(m_relativePt.x+m_matherRect.x, m_relativePt.y+m_matherRect.y);
	//if (0==m_iterTemplate)
	//{
	//	m_leftMatchPt = m_absolutePt;
	//}
	//else if (m_totTmplateNum-1==m_iterTemplate)
	//{
	//	m_rightMatchPt = m_absolutePt;
	//	m_resultAngle = cvFastArctan(m_rightMatchPt.y-m_leftMatchPt.y,m_rightMatchPt.x-m_leftMatchPt.x);
	//	//�˴�����1�ȵ����.sample rectСʱ��ƥ������󣩽Ƕ����С��rect��ʱ����ƥ������С���Ƕ�����
	//	//m_resultAngle = cvFastArctan(m_rightMatchPt.y-m_leftMatchPt.y,m_rightMatchPt.x-m_leftMatchPt.x-100);
	//}
	//m_iterTemplate+=1;
	return  m_absolutePt;
}


CvPoint Mather::findMatchPointToGetCycle(CvRect stripRect, int nMethods, int priorityMethod, bool useNormed)
{
	//CalcMatherRect(sampleRect);
	m_matherRect = cvRect(stripRect.x, stripRect.y+stripRect.height,stripRect.width, m_src->height-stripRect.height-stripRect.y);
	IplImage *subSrc = getSubTmplateImg(m_src,m_matherRect);

	IplImage *ftmp[6]={NULL};
	int method=0;
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
		int anotherMethod =((priorityMethod + 2) > 4) ?(priorityMethod - 2):(priorityMethod + 2);

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
			if(markArry[i]==0)//0 ������δ���ҹ���ֵ
			{
				for (int j=i+1;j<6;j++)
				{
					if(resultPoints[j].x==resultPoints[i].x&&resultPoints[j].y==resultPoints[i].y)
					{

						markArry[i] += 1;
						resultPoints[j] = impossiblePt;

					}

				}
				markArry[i] +=1;//�Լ�һ��ҲҪ����
			}
		}

	}
	int temIter = 0;
	int temp=markArry[0];
	for (int i=0;i<6;i++)
	{
		if(resultPoints[i].x == impossiblePt.x&&resultPoints[i].y == impossiblePt.y)continue;
		if (markArry[i]>temp)
		{
			temp = markArry[i];
			temIter = i;
		}

	}
	//release img
	for (int i = 0;i<6;i++)
	{
		if (ftmp[i]!=NULL)
		{
			cvReleaseImage(&ftmp[i]);
		}

	}
	cvReleaseImage(&subSrc);
	//printf("pos:%d,cnt:%d\n",temIter,temp);
	m_relativePt = resultPoints[temIter];
	m_absolutePt = cvPoint(m_relativePt.x+m_matherRect.x, m_relativePt.y+m_matherRect.y);
	//if (0==m_iterTemplate)
	//{
	//	m_leftMatchPt = m_absolutePt;
	//}
	//else if (m_totTmplateNum-1==m_iterTemplate)
	//{
	//	m_rightMatchPt = m_absolutePt;
	//	m_resultAngle = cvFastArctan(m_rightMatchPt.y-m_leftMatchPt.y,m_rightMatchPt.x-m_leftMatchPt.x);
	//	//�˴�����1�ȵ����.sample rectСʱ��ƥ������󣩽Ƕ����С��rect��ʱ����ƥ������С���Ƕ�����
	//	//m_resultAngle = cvFastArctan(m_rightMatchPt.y-m_leftMatchPt.y,m_rightMatchPt.x-m_leftMatchPt.x-100);
	//}
	//m_iterTemplate+=1;
	return  m_absolutePt;
}

CvPoint Mather::findMatchPoint(int sampleRect_x_cordinate, int nMethods, int priorityMethod, bool useNormed)
{
	
	IplImage *subSrc = getSubTmplateImg(m_src,getSamplePatchContainer( sampleRect_x_cordinate));

	IplImage *ftmp[6]={NULL};
	int method=0;
	//int patchx = m_samplePatchCopy->width;
	//int patchy = m_samplePatchCopy->height;
	int patchx = m_samplePatchSize.width;
	int patchy = m_samplePatchSize.height;
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
		int anotherMethod =((priorityMethod + 2) > 4) ?(priorityMethod - 2):(priorityMethod + 2);

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
			if(markArry[i]==0)//0 ������δ���ҹ���ֵ
			{
				for (int j=i+1;j<6;j++)
				{
					 if(resultPoints[j].x==resultPoints[i].x&&resultPoints[j].y==resultPoints[i].y)
					 {
						
						 markArry[i] += 1;
						 resultPoints[j] = impossiblePt;
						 
					 }
				  
				}
				markArry[i] +=1;//�Լ�һ��ҲҪ����
			}
		}

	}
	int temIter = 0;
	int temp=markArry[0];
	for (int i=0;i<6;i++)
	{
		if(resultPoints[i].x == impossiblePt.x&&resultPoints[i].y == impossiblePt.y)continue;
		if (markArry[i]>temp)
		{
			temp = markArry[i];
			temIter = i;
		}
	
	}
	//release img
	for (int i = 0;i<6;i++)
	{
		if (ftmp[i]!=NULL)
		{
			cvReleaseImage(&ftmp[i]);
		}
		
	}
	cvReleaseImage(&subSrc);
	//printf("pos:%d,cnt:%d\n",temIter,temp);
	m_relativePt = resultPoints[temIter];
	m_absolutePt = cvPoint(m_relativePt.x+m_matherRect.x, m_relativePt.y+m_matherRect.y);
	if (0==m_iterTemplate)
	{
		m_leftMatchPt = m_absolutePt;
	}
	else if (m_totTmplateNum-1==m_iterTemplate)
	{
		m_rightMatchPt = m_absolutePt;
		m_resultAngle = cvFastArctan(m_rightMatchPt.y-m_leftMatchPt.y,m_rightMatchPt.x-m_leftMatchPt.x);

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
		cvAddS(m_cpsrc,cvScalar(100),m_cpsrc);
	else
		cvAddS(m_cpsrc,cvScalar(-100),m_cpsrc);
	cvResetImageROI(m_cpsrc);
}
void Mather::showPic()
{
	cvNamedWindow("mather");
	cvShowImage("mather",m_cpsrc);
}
void Mather::mkLineColor(int starLine, int lineWight,int lineColor)
{
	cvSetImageROI(m_cpsrc,cvRect(0,starLine,m_src->width,lineWight));
	//cvAddS(m_cpsrc,cvScalar(100),m_cpsrc);
	cvSet(m_cpsrc,cvScalar(lineColor));
	cvResetImageROI(m_cpsrc);
}