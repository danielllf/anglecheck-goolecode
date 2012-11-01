
#ifndef _TEMPLATE_H
#define _TEMPLATE_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>

class Mather
{
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
	IplImage *getSubSrc(IplImage *src, CvRect rect);
	void releaseSubSrc(IplImage **subSrc);
	void CalcMatherRect(CvRect sampleRect);
	void markTmpRect();
	void showPic();

private:
	IplImage *m_src;
	IplImage *m_cpsrc;
	int m_maxTol;//按一定角度算出最大长度时的上限偏移
	int m_iterTemplate;
public:
	IplImage *m_template;
	CvRect m_matherRect;

};
#endif
