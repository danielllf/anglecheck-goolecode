#include <assert.h>
#include "../include/llfutility.h"
#include "../include/CalcObj.h"
/****************CalcObjVector************************/
CalcObjVector::CalcObjVector()
{

}
CalcObjVector::CalcObjVector( std::list<LINEIFO> &srclist)
{
	std::list<LINEIFO>:: iterator  it=srclist.begin();
	LINEIFO info;
	for (it;it!=srclist.end();++it)
	{
		info.lineNum = it->lineNum;
		info.value = it->value;
		m_vect.push_back(info);
	}
}
CalcObjVector::~CalcObjVector()
{

}
void CalcObjVector::addone(int value,int linenum)
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
int CalcObjVector::getTheMaxElement_Linenum()
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
	return maxSum.lineNum;

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
	float stainlity=1.0;//污染率
	if(m_list.size()==0)
	{
		log_erro("list count in CalcObjList is 0\n");
		return -1;
	}
	//convert list to mat for using cvAvgSdv
	CvScalar s_mean;
	CvScalar s_sdv;
	CvMat *mat;
	std::list<LINEIFO>:: iterator  it;
	int i;
	

	const float EPSINON = 0.0001f;//float 与0值比较时注意不能直接比较

	while(1)
	{
		mat=	cvCreateMat(1,(int)m_list.size(),CV_16U);
		for (it=m_list.begin(),i=0;it!=m_list.end();++it,++i)
		{
			 *((unsigned int*)CV_MAT_ELEM_PTR(*mat,0,i)) = it->value;
		}
		//PrintMat(mat);printf("\n+++++++++++\n");
		cvAvgSdv(mat,&s_mean,&s_sdv);
		stainlity = (float)s_sdv.val[0]/s_mean.val[0];


		float diff = stainlity-(1.0f-Purfactor);//杂草率

		if(diff >  EPSINON)//杂草率大于标准,/*float 0值的比较不能用==*/
		{
			//purify
			for (it=m_list.begin();it!=m_list.end();/*++it*/)
			{
				//remove element that below the mean*facor_below_remove
				if (it->value<(int)(s_mean.val[0]))
				{
					//m_list.remove_if( isElementValueEqual(it->value));//用remove可能会导致错误
					it=m_list.erase(it);
				}
				else
				{
					it++;
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
float CalcObjList::getLinedistanceSdvPercentToMean(int continuesLineCount,const std::list<LINEIFO>:: iterator  it_start,CvScalar &mean,CvScalar &sdv,
																					   std::list<int> &rlt_list)
{
	std::list<LINEIFO>:: iterator  it=it_start;
	CvScalar s_mean;
	CvScalar s_sdv;
	int matElementCnt = continuesLineCount-1;
	CvMat *mat=	cvCreateMat(1,matElementCnt,CV_16U);
	int preLinenum;
    rlt_list.clear();
	for (int i=0;i<matElementCnt;++i)
	{
		preLinenum = it->lineNum;
		rlt_list.push_back(preLinenum);
		++it;
		rlt_list.push_back(it->lineNum);
		int linedistance = it->lineNum-preLinenum;

		*((unsigned int*)CV_MAT_ELEM_PTR(*mat,0,i)) = linedistance;

	}	
	//PrintMat(mat);printf("\n+++++++++++\n");
	cvAvgSdv(mat,&s_mean,&s_sdv);
	mean = s_mean;
	sdv = s_sdv;
	return (float)(s_sdv.val[0]/s_mean.val[0]);

}
//当continuesTol=0.5时，认为获取失败
int CalcObjList::getLinePitch(int continuesCount,float &continuesTol,std::list<int> &rlt_list)
{
	assert(continuesCount>=3);
	std::list<LINEIFO>:: iterator  it;
	int it_startOfthisTurn;
	float percentsdvtomean;
	const float EPSINON = 0.0001f;//float 与0值比较时注意不能直接比较
	int listlen = (int)m_list.size();	
	CvScalar mean;
	CvScalar sdv;
	bool findSUCCESS=false;//
    while(1)
	{
		if (findSUCCESS ==true)break;
		//if false becaseof to the end of pic,the go on ,change continuesTol to find again
		it=m_list.begin();
		int i=0;
		 mean=cvScalar(0);
		 sdv=cvScalar(0);
		 if(continuesTol<0.05)continuesTol=0.05f;
		else if(continuesTol<0.1&&continuesTol>=0.05)continuesTol=0.1f;
		 else if(continuesTol<0.15&&continuesTol>=0.1)continuesTol=0.15f;
		  else if(continuesTol<0.2&&continuesTol>=0.15)continuesTol=0.2f;
			else if(continuesTol<0.25&&continuesTol>=0.2)continuesTol=0.25f;
			 else if(continuesTol<0.3&&continuesTol>=0.25)continuesTol=0.3f;
			  else if(continuesTol<0.35&&continuesTol>=0.3)continuesTol=0.35f;
			   else if(continuesTol<0.4&&continuesTol>=0.35)continuesTol=0.4f;
			   else continuesTol=0.5;


		while(1)
		{
			if (listlen-i<continuesCount)//to the end of pic
			{
				findSUCCESS = false;
				break;
			}
			percentsdvtomean=getLinedistanceSdvPercentToMean(continuesCount,it,mean,sdv,rlt_list);
			if (percentsdvtomean>(continuesTol-EPSINON))
			{
				it++;
				i++;
			}
			else
			{
				findSUCCESS = true;
				break;
			}
		}

	}
	return (int)mean.val[0];

}