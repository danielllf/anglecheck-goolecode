
#include "mather.h"
//#include <math.h>
int Mather::m_totTmplateNum=0;
Mather::Mather(IplImage *src)
{
		m_src = cvCreateImage(cvSize(src->width,src->height),src->depth, src->nChannels);
		cvCopy(src,m_src);

		m_cpsrc = cvCreateImage(cvSize(src->width,src->height),src->depth, src->nChannels);
		cvCopy(src,m_cpsrc);

		//m_maxTol = 90;//to be changed by future
		m_iterTemplate = 0;
}
Mather::~Mather()
{
		cvReleaseImage(&m_src);
}

//this template mem is hold and released by mather objet manuely
void Mather:: createTmplate(IplImage *templ)
{
	m_template = cvCreateImage(cvSize(templ->width, templ->height),templ->depth, templ->nChannels);
	cvCopy(templ,m_template);
	
}

void Mather::releaseTmplate(IplImage *tmpl)
{
		cvReleaseImage(&m_template);
}
IplImage * Mather::getSubSrc( IplImage *src, CvRect rect)
{
	IplImage *subROI = cvCreateImageHeader(
		cvSize(rect.width,rect.height),src->depth,src->nChannels);

	(subROI)->origin = src->origin;
	(subROI)->widthStep = src->widthStep;
	subROI->imageData =  src->imageData + m_matherRect.y*src->widthStep +	m_matherRect.x;		
	IplImage * subSrc = cvCreateImage(cvSize(subROI->width,subROI->height),subROI->depth,subROI->nChannels);
	cvCopy(subROI,subSrc);
	cvReleaseImageHeader(&subROI);
	return subSrc;
}
void Mather::releaseSubSrc(IplImage **subSrc)
{
	cvReleaseImage(subSrc);
}
//max angle should < 20 degree
void Mather::setMaxAngle(int angle)
{
	assert(angle<20);//不使用大于20度的角
	m_maxAngle = angle;
	m_deltWidth = (m_src->height)*sin(m_maxAngle*CV_PI/180);

}
void Mather::CalcMatherRect(CvRect sampleRect)
{//上下左右四个方向都留有余量
	int upEge = 0;
	int leftEge = 0;
	int rightEge = 0;
	int bottomEge = 0;
	m_deltHeight = (sampleRect.x+sampleRect.width)*sin(m_maxAngle*CV_PI/180);
	//up
	if ((upEge = sampleRect.y-m_deltHeight)<=0){
		upEge = 0;
	}
	//left
	if (( leftEge = sampleRect.x-m_deltWidth)<=0){
		leftEge = 0;
	}
	//right
	if((rightEge=sampleRect.width+sampleRect.x+m_deltWidth)>= m_src->width){
			rightEge=m_src->width;
	}
	//bottom
	if((bottomEge = sampleRect.y+sampleRect.height+m_deltHeight)>=m_src->height){
		bottomEge = m_src->height;
	}

	m_matherRect =cvRect(leftEge, upEge, rightEge-leftEge,bottomEge-upEge);

	
}
CvPoint Mather:: findMatchPoint(CvRect sampleRect, int nMethods, int priorityMethod, bool useNormed)
{
	CalcMatherRect(sampleRect);
	IplImage *subSrc = getSubSrc(m_src,m_matherRect);

	IplImage *ftmp[6]={NULL};
	int method=0;
	int patchx = m_template->width;
	int patchy = m_template->height;
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
		cvMatchTemplate( subSrc, m_template, ftmp[i], i); 
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
	releaseSubSrc(&subSrc);
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
		//此处会有1度的误差.sample rect小时（匹配度误差大）角度误差小，rect大时，（匹配度误差小）角度误差大。
		//m_resultAngle = cvFastArctan(m_rightMatchPt.y-m_leftMatchPt.y,m_rightMatchPt.x-m_leftMatchPt.x-100);
	}
	m_iterTemplate+=1;
	return  m_absolutePt;
}
double Mather::getFindAngle()
{
	return m_resultAngle;
}
//color the rect
void Mather::markTmpRect()
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