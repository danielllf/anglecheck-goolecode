#ifndef CALC_OJB_
#define CALC_OJB_
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
struct LINEIFO{
	LINEIFO(){}
	LINEIFO(int linenum,int Thevalue){lineNum = linenum;value=Thevalue;}
	int lineNum;
	int value;
};


class CalcObjList{
public:
	CalcObjList();
	~CalcObjList();
	void addone(int linenum,int value);
	LINEIFO getTheMaxElement();

	int getTheAvgElementValue();
	void clearAll();
	int Size();
	void removeWhereValueis(const int& value);
	//返回纯化后数据的纯度（1-sdv/mean）
	float PurifyTheData(float  Purfactor);
	float getLinedistanceSdvPercentToMean(int continuesLineCount,const std::list<LINEIFO>:: iterator  it_start,CvScalar &mean
																		,CvScalar &sdv,std::list<int> &rlt_list);
	int getLinePitch(int continuesCount,float &continuesTol,std::list<int> &rlt_list);
	std::list<LINEIFO> m_list;
private:

	class isElementValueEqual//remove_if的函数子(必须重载perator()).此类在remove_if中可以当作函数来调用
	{//相当于自定义一个满足一定条件的回调函数给remove_if
	public:
		isElementValueEqual(const int& outerParam):m_outParam(outerParam){}
		bool operator ()(LINEIFO inerParam)
		{
			if (inerParam.value==m_outParam)//所要满足的条件
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

class CalcObjVector{
public:
	CalcObjVector();
	CalcObjVector( std::list<LINEIFO> &srclist);
	~CalcObjVector();
	void addone(int value,int linenum=0);
	LINEIFO getTheMaxElement();
	int getTheMaxElement_Linenum();
	int getTheAvgElementValue();
	void clearAll();
	int Size();

private:
	std::vector<LINEIFO> m_vect;
};

#endif