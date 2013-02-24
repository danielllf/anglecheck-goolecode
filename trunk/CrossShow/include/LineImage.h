#ifndef _LINE_IMAGE_
#define _LINE_IMAGE_
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include <iostream>
#include <list>
#include <algorithm>
//�ڰ�ͼ�񣬺ڱ���+������



class LineImage{
public:
	LineImage(CvSize size);
	~LineImage();
	IplImage *getImage();
	int resetImageLine(CvPoint startPt,int lineLen);
public:
	IplImage *m_pImage;
	int m_linePitch;
};

//���м�������ڴˣ���ͼ���޹�

struct LINEIFO{
	int lineNum;
	int sum;
};


class CalcObj{
public:
	CalcObj();
	~CalcObj();
	void addone(int linenum,int sum);
	LINEIFO getTheMaxElement();
	void clearAll();
	int listSize();
	void removeWhereSumis(const int& value);
private:

	std::list<LINEIFO> m_list;
	class isElementSumEqual//remove_if�ĺ�����(��������perator()).������remove_if�п��Ե�������������
	{//�൱���Զ���һ������һ�������Ļص�������remove_if
	public:
		isElementSumEqual(const int& outerParam):m_outParam(outerParam){}
		bool operator ()(LINEIFO inerParam)
		{
			if (inerParam.sum==m_outParam)//��Ҫ���������
			{
				return true;
			}
			else
				return false;
		}
	private:
		int m_outParam;
	};
};


#endif