
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
	//�����ܽ᣺1.����CV_TM_CCORR����ƥ��̫��������ֻʹ��2��������CV_TM_CCOEFF+CV_TM_SQDIFF���򵥶�ʹ��		
	//	2.�ɿ������Ƿ��һ������ʱ���Ӱ��Ƚϴ󣬼�������		
	//	3.CV_TM_CCOEFF����Ҫ��CV_TM_SQDIFF��ʱ��һ�㣬����𲻴�		
	//	4.����������+nornamled����sample�����==10��	sample���С==89*109��	ĸ���С	==905*678ʱ,����ʱ494ms		
	CvPoint findMatchPoint(IplImage *subSrc, int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);

	CvPoint findMatchPointTogetCycle(IplImage *subSrc, MatchResult &result,int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);
	CvPoint MatchPatch(int sampleRect_x_cordinate, int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);
	CvPoint GetCycle(CvRect stripRect, int assumedMinCycle, int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);
	CvPoint MatchHead(CvRect stripHdrRect, int  deltWidth, int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);

	//��ȡÿһ��sample patchӳ����ģ�������ڵĽϴ�ľ�������ͼ��
	IplImage *getSubTmplateImg(IplImage *matherImg, CvRect samplePatchContainer);
	//void releaseSubSrc(IplImage **subSrc);
	CvRect getSamplePatchContainer(int sampleRect_x_cordinate);
	CvRect getSamplePatchContainerLast(int sampleRect_x_cordinate,int matchedLineinCycle);
	void markTemplateRect();
	void showPic();

	//���������ڶ��ľ��������ڽ���patchƥ��,(��������ȫ��ĸ��ͼ�н���ƥ�����)
	void setMaxAngle(int angle);
	double getFindAngle();
	//assumedMinCycleӦ�ñ�ʵ�ʵ�cycleС��
	//stripΪsample�����Ʒ,sartlineΪstrip��yֵ���ڴ�line������Ӧ�����ٰ���һ��ѭ����Ϊ��֤һ��mather����һ��ѭ��������£���ֵ��Ϊ0
	//���ڴ�ѭ����ͼassumedMinCycleӦ�����Щ,��Ӧ�ò�����������cycle��Сѭ����ͼassumedMinCycleӦ����СЩ,��5����Ӧ����������cycle
	int findMinCycle(int stripHeight,int startLine=0,int assumedMinCycle=5, int nMethods=2, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);

	void setTmplateNum(int num);//����mplate����,catchsample�󣬻Ὣsample ��patch����һͬ����
	void mkLineColor(int starLine, int lineWight=4,int lineColor=255);
	//��mather�в���sample��λ��sampleRect����ͼ���λ��
	//deltWidthShift��ͼ����ת�����µ�sampleͼ����ߵĶ�ʧ
	int findSampleHdrPosWithin1stCycle( int widthDiv, int deltWidthShift=10,int nMethods=3, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);
	//deltWidthShift����ƥ���ҵ�ƫ�ƣ�ĸ�岻Ӧ�ó��֣���Ӧ�ÿ�������
	int findSampleHdrPos( int widthDiv, int deltWidthShift=10,int nMethods=3, int priorityMethod=CV_TM_CCOEFF, bool useNormed=true);
	IplImage *getImage();
	
	//hdr_size.height ������3,5,7����

private:
	IplImage *m_mather_org;
	IplImage *m_mather;//���ź��ͼ
	Sample *m_sample;
	
	int m_maxTol;//��һ���Ƕ������󳤶�ʱ������ƫ��
	int m_sampleHdrMatchLine;//�Դ�Ϊ��׼��������ƥ��
	int m_sampleHdrMatchLine1st;//��0Ϊstarline��cycle�е�matchline
	int m_iterTemplate;
	int m_deltWidth;
	//int m_deltHeight;
	int m_maxAngle;//�������Ƕȣ���������m_matherRect�Ĵ�С��
	double m_resultAngle;//���ռ�����ĽǶ�
	CvPoint m_absolutePt;//��ĸ���е�����
	CvPoint m_relativePt;//����ʱm_matherRect�е��������
	CvPoint m_leftMatchPt;//��ĸ���������ƥ�������
	CvPoint m_rightMatchPt;//��ĸ�������ұ�ƥ�������
	int m_minCycle;//��Сѭ��
	CvSize m_samplePatchSize;
	int m_totTmplateNum;
	int m_sampleLeftShinked;

public:
	IplImage *m_cpsrc;//��ͼ������ʾ�����������������
	IplImage *m_samplePatchCopy;
	CvRect m_matherRect;

};
#endif
