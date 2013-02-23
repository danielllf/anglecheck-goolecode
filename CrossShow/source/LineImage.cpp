#include "../include/global_def.h"
#include "../include/LineImage.h"
int WHITE_PART=100;
LineImage::LineImage(CvSize size)
{
	m_pImage = cvCreateImage(size,IPL_DEPTH_8U,1);
	cvSet(m_pImage,cvScalar(0));
}
LineImage::~LineImage()
{
	cvReleaseImage(&m_pImage);
}
IplImage* LineImage::getImage()
{
	return	m_pImage; 
}
int LineImage::resetImageLine(CvPoint startPt,int lineLen)
{
		cvSet(m_pImage,cvScalar(0));
		cvLine(m_pImage,startPt,cvPoint(lineLen,startPt.y),cvScalar(WHITE_PART));
	
	return 0;
}

/****************CalcObj************************/
CalcObj::CalcObj()
{
	
}
CalcObj::~CalcObj()
{

}
void CalcObj::addone(int linenum,int sum)
{
	LINEIFO info;
	info.lineNum = linenum;
	info.sum = sum;
	m_vect.push_back(info);
}

LINEIFO CalcObj::getTheMaxElement()
{
	std::vector<LINEIFO>:: iterator  it;
	 LINEIFO maxSum;
	/* maxSum.sum =m_vect.begin()->lineNum;
	for (it=m_vect.begin();it!=m_vect.end(); it++)
	{
		if(it->sum>maxSum.sum)
		{
			maxSum.sum=it->sum;
			maxSum.lineNum = it->lineNum;
		}
	}*/
	maxSum.sum = m_vect[0].sum;
	 for (int i=0;i<m_vect.size();++i)
	 {
		 if (m_vect[i].lineNum>maxSum.sum)
		 {
			 maxSum.sum = m_vect[i].sum;
			 maxSum.lineNum = m_vect[i].lineNum;
		 }
	 }
	 return maxSum;
}	
void CalcObj::clearAll()
{
	m_vect.clear();
}