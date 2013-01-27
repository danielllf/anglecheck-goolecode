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

	/*return:0 �ɹ���ȡ
	*			-1����ȡʧ��
	*/
	int getPatchImg(IplImage ** temp);
	int getLastPatch_x_cordinate();
	void markPatchRect();
	void stepTmplate();
	void showPic();
	IplImage *getImage();

	CvRect m_rectPatch;

	

private:
	IplImage *m_sampleBeforeShrink;//��������ǰ��ͼ����boundingrect��ȡ�ı�Ե����Ϊ��������ǰ���ɷ֣���������һ��
	IplImage *m_sample;//���ߺ��ͼ
	IplImage *m_patchImg;//sample�Ķ���Ӿ��������е�һ��
	int m_iterTemplate;
	int m_totPatchNum;//sample���Ӿ�������ĸ���

	//template postion info

	CvSize m_patchSize;//������Ĵ�С
	
	//template info from mather pic
	int m_depth;
	int m_nChannels;
	int m_widthStep;
};
#endif