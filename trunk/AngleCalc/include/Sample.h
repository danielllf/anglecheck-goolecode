#ifndef  _SAMPLE_H
#define _SAMPLE_H
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>

class Sample
{
public:
	Sample(IplImage *mather, IplImage *sample);
	~Sample();
	void getPrmFromMather(IplImage *mather);
	void loadSamplePic(IplImage *sample);
	void initTmplate(int starLine, CvSize templateSize);

	/*return:0 成功获取
	*			-1：获取失败
	*/
	int getTmplate(IplImage ** temp);
	void stepTmplate();
	CvRect m_rectTmplate;
private:
	IplImage *m_sample;
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