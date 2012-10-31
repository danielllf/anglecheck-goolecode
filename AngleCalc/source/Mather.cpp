
#include "mather.h"

Mather::Mather(IplImage *src)
{
		m_src = cvCreateImage(cvSize(src->width,src->height),src->depth, src->nChannels);
		cvCopy(src,m_src);
		m_maxTol = 80;
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
void Mather::CalcMatherRect(CvRect sampleRect)
{
	int newWidthtSrc = sampleRect.width+m_maxTol;
	if (newWidthtSrc>=m_src->width)
	{
		newWidthtSrc = m_src->width - sampleRect.x-1;
	}
	m_matherRect =cvRect(sampleRect.x,sampleRect.y-m_maxTol,newWidthtSrc,sampleRect.height+2*m_maxTol);

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
	return  resultPoints[temIter];
}


