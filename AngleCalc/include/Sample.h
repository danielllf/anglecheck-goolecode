#ifndef  _SAMPLE_H
#define _SAMPLE_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "ImgeBase.h"


class Sample:public ImageBase
{

public:
	//friend class Mather;
	
	Sample(IplImage *SampleSrc, CvRect rectInSrc,float scale=1.0);
	~Sample();
	//void getPrmFromMather(IplImage *mather);
	//void loadSamplePic(IplImage *sample);
	int shrinkSample(int left, int right, int top, int bottom);
	void setPatchNum(int num);

	/*return:0 成功获取
	*			-1：获取失败
	*/
	int getPatchImg(IplImage ** temp);
	int getLastPatch_x_cordinate();
	void markPatchRect();
	void stepTmplate();
	void showPic();
	IplImage *getImage();

	CvRect m_rectPatch;

	

private:
	IplImage *m_sampleBeforeShrink;//进行缩边前的图。从boundingrect获取的边缘可能为不纯属的前景成分，需向内缩一下
	IplImage *m_sample;//缩边后的图
	IplImage *m_patchImg;//sample的多个子矩形区域中的一个
	int m_iterTemplate;
	int m_totPatchNum;//sample的子矩形区域的个数

	//template postion info

	CvSize m_patchSize;//子区域的大小
	
	//template info from mather pic
	int m_depth;
	int m_nChannels;
	int m_widthStep;
};
#endif