#ifndef _LINE_IMAGE_
#define _LINE_IMAGE_
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>
#include <iostream>
#include <list>
#include <algorithm>
//黑白图像，黑背景+白线条



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

//所有计算过程在此，与图像无关

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
	class isElementSumEqual//remove_if的函数子(必须重载perator()).此类在remove_if中可以当作函数来调用
	{//相当于自定义一个满足一定条件的回调函数给remove_if
	public:
		isElementSumEqual(const int& outerParam):m_outParam(outerParam){}
		bool operator ()(LINEIFO inerParam)
		{
			if (inerParam.sum==m_outParam)//所要满足的条件
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