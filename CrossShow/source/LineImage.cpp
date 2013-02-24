#include "../include/global_def.h"
#include "../include/LineImage.h"
int WHITE_PART=100;//�ڰ�ͼƬ�а�ɫֵ����ΪС��255��һ�룬��������ͼ��Ӻ���С��255�������洦��
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
	m_list.push_back(info);

}

LINEIFO CalcObj::getTheMaxElement()
{
	std::list<LINEIFO>:: iterator  it=m_list.begin();
	 LINEIFO maxSum;
	
	maxSum.sum = it->sum;
	 for (it;it!=m_list.end();++it)
	 {
		 if (it->sum>maxSum.sum)
		 {
			 maxSum.sum = it->sum;
			 maxSum.lineNum = it->lineNum;
		 }
	 }
	 return maxSum;
}	
void CalcObj::clearAll()
{
	m_list.clear();
}

void CalcObj::removeWhereSumis(const int& value)
{
	m_list.remove_if( isElementSumEqual(value));
}
int CalcObj::listSize()
{
	return m_list.size();
}