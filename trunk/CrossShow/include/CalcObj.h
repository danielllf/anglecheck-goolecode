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
private:

	std::list<LINEIFO> m_list;
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
	~CalcObjVector();
	void addone(int linenum,int value);
	LINEIFO getTheMaxElement();
	int getTheAvgElementValue();
	void clearAll();
	int Size();
private:
	std::vector<LINEIFO> m_vect;
};

#endif