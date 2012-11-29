#ifndef SAMPLE_WITH_BACK_GROUND
#define SAMPLE_WITH_BACK_GROUND


#include "cv.h"				// define all of the opencv classes etc.
#include "highgui.h"
#include "cxcore.h"
#include "cv_yuv_codebook.h"
class SampleWithBackGroud{

public:
	//avgbackΪǰ��ģ�ͣ�newsampleΪ�µĲ���ͼ
 SampleWithBackGroud(IplImage *avgback, IplImage *newsample, int thresHold=10);
 ~SampleWithBackGroud();

// Create a binary: 0,255 mask where 255 means forground pixel
// Imask	mask image to be created, 1 channel 8u
void backgroundDiffllf() ; //Mask should be grayscale
CvRect calcBoundingRect();//����ǰ������Ӿ���
CvRect getBoundingRect();//��ȡǰ������Ӿ��Σ�ǰ���Ѿ�������ˣ�
void showImg();
public:
	IplImage  *InewSample;

private:
	IplImage *IhiF, *IlowF, *Imask,*IavgBack, *IthresHold;
	IplImage *ImaskAVG ,*ImaskAVGCC ;
	CvRect m_boundingRect;
	int m_thresHold;
};


#endif

