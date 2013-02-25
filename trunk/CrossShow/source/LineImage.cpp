#include "../include/global_def.h"
#include "../include/LineImage.h"
int WHITE_PART=100;//黑白图片中白色值设置为小于255的一半，所以在两图相加后，仍小于255，不会益处。
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
		cvLine(m_pImage,startPt,cvPoint(lineLen-1,startPt.y),cvScalar(WHITE_PART));
	
	return 0;
}

/****************CalcObj************************/
CalcObj::CalcObj()
{
	
}
CalcObj::~CalcObj()
{

}
void CalcObj::addone(int linenum,int value)
{
	LINEIFO info;
	info.lineNum = linenum;
	info.value = value;
	m_list.push_back(info);

}

LINEIFO CalcObj::getTheMaxElement()
{
	std::list<LINEIFO>:: iterator  it=m_list.begin();
	 LINEIFO maxSum;
	assert(m_list.size()>0);
	maxSum.value = it->value;
	maxSum.lineNum = it->lineNum;
	 for (it;it!=m_list.end();++it)
	 {
		 if (it->value>maxSum.value)
		 {
			 maxSum.value = it->value;
			 maxSum.lineNum = it->lineNum;
		 }
	 }
	 return maxSum;
}	
int CalcObj::getTheAvgElementValue()
{
	std::list<LINEIFO>:: iterator  it=m_list.begin();
	int sum=0;
	for (it;it!=m_list.end();++it)
	{
		sum+=it->value;
	}
	return sum/(int)m_list.size();
}
void CalcObj::clearAll()
{
	m_list.clear();
}

void CalcObj::removeWhereValueis(const int& value)
{
	m_list.remove_if( isElementSumEqual(value));
}
int CalcObj::listSize()
{
	return (int)m_list.size();
}