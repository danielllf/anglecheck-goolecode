
#ifndef _TEMPLATE_H
#define _TEMPLATE_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "Sample.h"
class Mather:public ImageBase
{
	typedef struct MatchResult 
	{
		CvPoint pt;
		double minValue;
		double maxValue;
		int besetMethod;
	}MatchResult;
public:
	
	Mather(IplImage *src,float scale=1.0);
	void  fetchSamplePatchImg(IplImage *samplePatch);
	void releaseSamplePatchCopy(IplImage **samplePatchCopy);
	void catchSample(Sample *sample);

	void getSampleShinked(int left, int right, int top, int bottom);
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
	//经验总结：1.由于CV_TM_CCORR方法匹配太差，所以最多只使用2个方法，CV_TM_CCOEFF+CV_TM_SQDIFF。或单独使用		
	//	2.可看出“是否归一化”对时间的影响比较大，几乎翻倍		
	//	3.CV_TM_CCOEFF方法要比CV_TM_SQDIFF费时多一点，但差别不大		
	//	4.两方法都用+nornamled，，sample块个数==10，	sample块大小==89*109，	母板大小	==905*678时,最大耗时494ms		
	CvPoint findMatchPoint(IplImage *subSrc, int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);

	CvPoint findMatchPointTogetCycle(IplImage *subSrc, MatchResult &result,int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);
	CvPoint MatchPatch(int sampleRect_x_cordinate, int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);
	CvPoint GetCycle(CvRect stripRect, int assumedMinCycle, int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);
	CvPoint MatchHead(CvRect stripHdrRect, int  deltWidth, int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);

	//获取每一块sample patch映射在模板中所在的较大的矩形区域图像
	IplImage *getSubTmplateImg(IplImage *matherImg, CvRect samplePatchContainer);
	//void releaseSubSrc(IplImage **subSrc);
	CvRect getSamplePatchContainer(int sampleRect_x_cordinate);
	CvRect getSamplePatchContainerLast(int sampleRect_x_cordinate,int matchedLineinCycle);
	void markTemplateRect();
	void showPic();

	//用来计算在多大的矩形区域内进行patch匹配,(而不是在全部母板图中进行匹配查找)
	void setMaxAngle(int angle);
	double getFindAngle();
	//assumedMinCycle应该比实际的cycle小。
	//strip为sample的替代品,sartline为strip的y值，在此line的下面应该至少包含一个循环，为保证一个mather就是一个循环的情况下，此值设为0
	//对于大循环的图assumedMinCycle应该设大些,但应该不超过真正的cycle。小循环的图assumedMinCycle应该设小些,如5，但应大于真正的cycle
	int findMinCycle(int stripHeight,int startLine=0,int assumedMinCycle=5, int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);

	void setTmplateNum(int num);//设置mplate个数,catchsample后，会将sample 的patch个数一同设置
	void mkLineColor(int starLine, int lineWight=4,int lineColor=255);
	//在mather中查找sample中位于sampleRect区域图像的位置
	//deltWidthShift因图像旋转而导致的sample图像左边的丢失
	int findSampleHdrPosWithin1stCycle( int widthDiv, int deltWidthShift=10,int nMethods=3, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);
	//deltWidthShift，布匹向右的偏移，母板不应该出现，但应该考虑在内
	int findSampleHdrPos( int widthDiv, int deltWidthShift=10,int nMethods=3, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);
	IplImage *getImage();
	
	//hdr_size.height 必须是3,5,7奇数

private:
	IplImage *m_mather_org;
	IplImage *m_mather;//缩放后的图
	Sample *m_sample;
	
	int m_maxTol;//按一定角度算出最大长度时的上限偏移
	int m_sampleHdrMatchLine;//以此为基准进行向右匹配
	int m_sampleHdrMatchLine1st;//以0为starline的cycle中的matchline
	int m_iterTemplate;
	int m_deltWidth;
	//int m_deltHeight;
	int m_maxAngle;//最大允许角度，用来限制m_matherRect的大小。
	double m_resultAngle;//最终计算出的角度
	CvPoint m_absolutePt;//在母板中的坐标
	CvPoint m_relativePt;//在临时m_matherRect中的相对坐标
	CvPoint m_leftMatchPt;//在母板中最左边匹配的坐标
	CvPoint m_rightMatchPt;//在母板中最右边匹配的坐标
	int m_minCycle;//最小循环
	CvSize m_samplePatchSize;
	int m_totTmplateNum;
	int m_sampleLeftShinked;

public:
	IplImage *m_cpsrc;//此图用来显示结果，不参与计算过程
	IplImage *m_samplePatchCopy;
	CvRect m_matherRect;

};
#endif
