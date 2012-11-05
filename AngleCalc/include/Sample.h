#ifndef  _SAMPLE_H
#define _SAMPLE_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include "Mather.h"
class Sample
{

public:
	friend class Mather;
	Sample(IplImage *mather, IplImage *sample);
	~Sample();
	void getPrmFromMather(IplImage *mather);
	void loadSamplePic(IplImage *sample);
	void initTmplate(int starLine, CvSize templateSize);

	/*return:0 �ɹ���ȡ
	*			-1����ȡʧ��
	*/
	int getTmplate(IplImage ** temp);
	void markTmpRect();
	void stepTmplate();
	void showPic();
	CvRect m_rectTmplate;
	IplImage *m_sample;
private:
	
	IplImage *m_templateImg;
	int m_iterTemplate;
	int m_totTmplateNum;

	//template postion info

	CvSize m_tmplateSize;
	int m_cmpStartLine;
	
	
	//template info from mather pic
	int m_depth;
	int m_nChannels;
	int m_widthStep;
};
#endif