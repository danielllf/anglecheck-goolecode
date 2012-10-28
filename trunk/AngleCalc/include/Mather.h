
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
	CvPoint findMatchPoint(IplImage *temp, int nMethods=3, int priorityMethod=CV_TM_SQDIFF, bool useNormed=true);
	

private:
	IplImage *m_src;
public:
	IplImage *m_template;
};
#endif
