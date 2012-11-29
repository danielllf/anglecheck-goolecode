
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
	void  fetchSamplePatchImg(IplImage *samplePatch);
	void releaseSamplePatchCopy(IplImage **samplePatchCopy);
	~Mather();
	//ƥ�䷽�����£�
	//CV_TM_SQDIFF        =0,ƽ����ƥ��
	//CV_TM_SQDIFF_NORMED =1,��һ��ƽ����ƥ��
	//CV_TM_CCORR         =2,���ƥ��
	//CV_TM_CCORR_NORMED  =3,
	//CV_TM_CCOEFF        =4,���ƥ���
	//CV_TM_CCOEFF_NORMED =5
	//��tempͼ����srcImg��ƥ�䣬���ҵ�ƥ��ʱ������ƥ��㡣���򷵻أ�-1��-1����
	//ƽ����Ϊ����ƥ�䷨��Ϊ�˼ӿ��ٶȿɽ�nMethods��Ϊ3,�������ܻ���ߡ�
	//ʹ�ü����һ������������cvNormalize,this is just for display�������ƥ�侫�ȣ�������������
	//priorityMethod=0,2,4
	CvPoint findMatchPoint(int sampleRect_x_cordinate, int nMethods=3, int priorityMethod=CV_TM_SQDIFF, bool useNormed=true);
	CvPoint findMatchPointToGetCycle(CvRect stripRect,  int nMethods=3, int priorityMethod=CV_TM_SQDIFF, bool useNormed=true);
	CvPoint findMatchPointInCycle(CvRect stripHdrRect, int  deltWidth, int nMethods=3, int priorityMethod=CV_TM_SQDIFF, bool useNormed=true);

	//��ȡÿһ��sample patchӳ����ģ�������ڵĽϴ�ľ�������ͼ��
	IplImage *getSubTmplateImg(IplImage *matherImg, CvRect samplePatchContainer);
	//void releaseSubSrc(IplImage **subSrc);
	CvRect getSamplePatchContainer(int sampleRect_x_cordinate);
	void markTemplateRect();
	void showPic();

	//���������ڶ��ڵľ��������ڽ���patchƥ��,(��������ȫ��ĸ��ͼ�н���ƥ�����)
	void setMaxAngle(int angle);
	double getFindAngle();

	int findMinCycle(int stripHeight,int startLine);

	void setTmplateNum(int num);//����mplate����
	void mkLineColor(int starLine, int lineWight=5,int lineColor=255);
	//��mather�в���sample��λ��sampleRect����ͼ���λ��
	int findSampleHdrPos(IplImage *sample, CvRect stripHdrRect, int deltWidthShift=10);

	//hdr_size.height ������3,5,7����

private:
	IplImage *m_src;
	IplImage *m_cpsrc;//��ͼ������ʾ�����������������
	int m_maxTol;//��һ���Ƕ������󳤶�ʱ������ƫ��
	int m_sampleHdrMatchLine;//�Դ�Ϊ��׼��������ƥ��
	int m_iterTemplate;
	int m_deltWidth;
	int m_deltHeight;
	int m_maxAngle;//�������Ƕȣ���������m_matherRect�Ĵ�С��
	double m_resultAngle;//���ռ�����ĽǶ�
	CvPoint m_absolutePt;//��ĸ���е�����
	CvPoint m_relativePt;//����ʱm_matherRect�е��������
	CvPoint m_leftMatchPt;//��ĸ���������ƥ�������
	CvPoint m_rightMatchPt;//��ĸ�������ұ�ƥ�������
	int m_minCycle;//��Сѭ��
	CvSize m_samplePatchSize;
	int m_totTmplateNum;
public:
	IplImage *m_samplePatchCopy;
	CvRect m_matherRect;

};
#endif
