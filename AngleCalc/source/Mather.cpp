
#include "mather.h"

Mather::Mather(IplImage *src)
{
		m_src = cvCreateImage(cvSize(src->width,src->height),src->depth, src->nChannels);
		cvCopy(src,m_src);
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

CvPoint Mather:: findMatchPoint( IplImage *templ, int nMethods, int priorityMethod, bool useNormed)
{
	IplImage *ftmp[6]={NULL};
	int method=0;
	int patchx = templ->width;
	int patchy = templ->height;
	int iwidth = m_src->width - patchx + 1;
	int iheight = m_src->height - patchy + 1;
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
		cvMatchTemplate( m_src, templ, ftmp[i], i); 
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
	
	//printf("pos:%d,cnt:%d\n",temIter,temp);
	return  resultPoints[temIter];
}


