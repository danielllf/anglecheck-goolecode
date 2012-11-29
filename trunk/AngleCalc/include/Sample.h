#ifndef  _SAMPLE_H
#define _SAMPLE_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "SampleWithBackGroud.h"


class Sample
{

public:
	//friend class Mather;
	Sample(SampleWithBackGroud  &swbg);
	~Sample();
	//void getPrmFromMather(IplImage *mather);
	//void loadSamplePic(IplImage *sample);
	void shrinkSample(int shrinkTol_x, int shrinkTol_y);
	void setPatchNum(int num);

	/*return:0 �ɹ���ȡ
	*			-1����ȡʧ��
	*/
	int getPatchImg(IplImage ** temp);
	void markPatchRect();
	void stepTmplate();
	void showPic();
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