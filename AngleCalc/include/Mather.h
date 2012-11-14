
#ifndef _TEMPLATE_H
#define _TEMPLATE_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "pencil.h"

class Mather
{
	friend class Sample;
public:
	Mather(IplImage *src);
	void  createTmplate(IplImage *templ);
	void releaseTmplate(IplImage *tmpl);
	~Mather();
	//匹配方法如下：
	//CV_TM_SQDIFF        =0,平方差匹配
	//CV_TM_SQDIFF_NORMED =1,归一化平方差匹配
	//CV_TM_CCORR         =2,相关匹配
	//CV_TM_CCORR_NORMED  =3,
	//CV_TM_CCOEFF        =4,相关匹配二
	//CV_TM_CCOEFF_NORMED =5
	//将temp图块在srcImg中匹配，当找到匹配时，返回匹配点。否则返回（-1，-1）。
	//平方差为优先匹配法。为了加快速度可将nMethods设为3,但误差可能会提高。
	//使用计算归一化法（区别于cvNormalize,this is just for display）可提高匹配精度，但计算量更大
	//priorityMethod=0,2,4
	CvPoint findMatchPoint(CvRect sampleRect, int nMethods=3, int priorityMethod=CV_TM_SQDIFF, bool useNormed=true);
	CvPoint findMatchPointToGetCycle(CvRect stripRect,  int nMethods=3, int priorityMethod=CV_TM_SQDIFF, bool useNormed=true);
	IplImage *getSubSrc(IplImage *src, CvRect rect);
	void releaseSubSrc(IplImage **subSrc);
	void CalcMatherRect(CvRect sampleRect);
	void markTmpRect();
	void showPic();
	void setMaxAngle(int angle);
	double getFindAngle();
	int findMinCycle(int stripHeight,int startLine);
	void mkLineColor(int starLine, int lineWight=5,int lineColor=255);
	int findSampleHdrPos(IplImage *sample, CvRect sampleRect, int deltWidthShift=10);
	CvPoint findMatchPointInCycle(CvRect stripRect, int  deltWidth, int nMethods=3, int priorityMethod=CV_TM_SQDIFF, bool useNormed=true);

	//hdr_size.height 必须是3,5,7奇数
protected:
	static int m_totTmplateNum;
private:
	IplImage *m_src;
	IplImage *m_cpsrc;//此图用来显示结果，不参与计算过程
	int m_maxTol;//按一定角度算出最大长度时的上限偏移
	
	int m_iterTemplate;
	int m_deltWidth;
	int m_deltHeight;
	int m_maxAngle;//最大允许角度，用来限制m_matherRect的大小。
	int m_resultAngle;//最终计算出的角度
	CvPoint m_absolutePt;//在母板中的坐标
	CvPoint m_relativePt;//在临时m_matherRect中的相对坐标
	CvPoint m_leftMatchPt;//在母板中最左边匹配的坐标
	CvPoint m_rightMatchPt;//在母板中最右边匹配的坐标
	int m_minCycle;//最小循环
public:
	IplImage *m_samplePatchCopy;
	CvRect m_matherRect;

};
#endif
