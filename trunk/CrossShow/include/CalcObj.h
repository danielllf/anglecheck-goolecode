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
	//���ش��������ݵĴ��ȣ�1-sdv/mean��
	float PurifyTheData(float  Purfactor);
	float getLinedistanceSdvPercentToMean(int continuesLineCount,const std::list<LINEIFO>:: iterator  it_start,CvScalar &mean
																		,CvScalar &sdv,std::list<int> &rlt_list);
	int getLinePitch(int continuesCount,float &continuesTol,std::list<int> &rlt_list);
	std::list<LINEIFO> m_list;
private:

	class isElementValueEqual//remove_if�ĺ�����(��������perator()).������remove_if�п��Ե�������������
	{//�൱���Զ���һ������һ�������Ļص�������remove_if
	public:
		isElementValueEqual(const int& outerParam):m_outParam(outerParam){}
		bool operator ()(LINEIFO inerParam)
		{
			if (inerParam.value==m_outParam)//��Ҫ���������
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