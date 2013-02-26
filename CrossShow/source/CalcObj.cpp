#include <assert.h>
#include "../include/llfutility.h"
#include "../include/CalcObj.h"
/****************CalcObjVector************************/
CalcObjVector::CalcObjVector()
{

}
CalcObjVector::~CalcObjVector()
{

}
void CalcObjVector::addone(int linenum,int value)
{
	LINEIFO info;
	info.lineNum = linenum;
	info.value = value;
	m_vect.push_back(info);

}

LINEIFO CalcObjVector::getTheMaxElement()
{
	std::vector<LINEIFO>:: iterator  it=m_vect.begin();
	LINEIFO maxSum;
	assert(m_vect.size()>0);
	maxSum.value = it->value;
	maxSum.lineNum = it->lineNum;
	for (it;it!=m_vect.end();++it)
	{
		if (it->value>maxSum.value)
		{
			maxSum.value = it->value;
			maxSum.lineNum = it->lineNum;
		}
	}
	return maxSum;

}	
int CalcObjVector::getTheAvgElementValue()
{
	std::vector<LINEIFO>:: iterator  it=m_vect.begin();
	int sum=0;
	for (it;it!=m_vect.end();++it)
	{
		sum+=it->value;
	}
	return sum/(int)m_vect.size();
}
void CalcObjVector::clearAll()
{
	m_vect.clear();
}
int CalcObjVector::Size()
{
	return (int)m_vect.size();
}


/****************CalcObjList************************/
CalcObjList::CalcObjList()
{

}
CalcObjList::~CalcObjList()
{

}
void CalcObjList::addone(int linenum,int value)
{
	LINEIFO info;
	info.lineNum = linenum;
	info.value = value;
	m_list.push_back(info);

}

LINEIFO CalcObjList::getTheMaxElement()
{
	std::list<LINEIFO>:: iterator  it=m_list.begin();
	LINEIFO maxSum;
	assert(m_list.size()>0);
	maxSum.value = it->value;
	maxSum.lineNum = it->lineNum;
	for (it;it!=m_list.end();++it)
	{
		if (it->value>maxSum.value)
		{
			maxSum.value = it->value;
			maxSum.lineNum = it->lineNum;
		}
	}
	return maxSum;
}	
int CalcObjList::getTheAvgElementValue()
{
	std::list<LINEIFO>:: iterator  it=m_list.begin();
	int sum=0;
	for (it;it!=m_list.end();++it)
	{
		sum+=it->value;
	}
	return sum/(int)m_list.size();
}
void CalcObjList::clearAll()
{
	m_list.clear();
}

void CalcObjList::removeWhereValueis(const int& value)
{
	m_list.remove_if( isElementValueEqual(value));
}
int CalcObjList::Size()
{
	return (int)m_list.size();
}
float CalcObjList::PurifyTheData(float  Purfactor)
{
	//convert list to mat for using cvAvgSdv
	CvScalar s_mean;
	CvScalar s_sdv;
	int  cols = (int)m_list.size();
	CvMat *mat=	cvCreateMat(1,cols,CV_16U);
	std::list<LINEIFO>:: iterator  it;
	int i;
	int facor_below_remove=0.5;//小于均值一半的，删除它
	float stainlity=1.0;//污染率

	const float EPSINON = 0.0001;//float 与0值比较时注意不能直接比较

	while(1)
	{
		for (it=m_list.begin(),i=0;it!=m_list.end();++it,++i)
		{
			 *((unsigned int*)CV_MAT_ELEM_PTR(*mat,0,i)) = it->value;
		}
		PrintMat(mat);printf("\n+++++++++++\n");
		cvAvgSdv(mat,&s_mean,&s_sdv);
		stainlity = s_sdv.val[0]/s_mean.val[0];


		float diff = stainlity-(1.0-Purfactor);//杂草率

		if(diff >  EPSINON)//杂草率大于标准,/*float 0值的比较不能用==*/
		{
			//purify
			for (it=m_list.begin();it!=m_list.end();++it)
			{
				//remove element that below the mean*facor_below_remove
				if (it->value<(int)(s_mean.val[0]*facor_below_remove))
				{
					m_list.remove_if( isElementValueEqual(it->value));
				}
			}
		
		}
		else
		{
			break;
		}
	}
	return 1-stainlity;
}

