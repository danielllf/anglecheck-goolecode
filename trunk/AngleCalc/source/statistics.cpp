#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "global.h"
#include <core/core_c.h>
#include "statistic.h"
//using namespace cv;
int jitter_tolerance = 0;
int slide_tolerance = 0;
int start_line_type = 0;

line_status findThejiZhiLine(const void*srcImg,int start_line, int jitter_tolerance)
{
	cv::Mat src = cv::cvarrToMat(srcImg);

	cv::Size size = src.size();


	int i=0,j=0;
	ARROW_t culmulate_direct = LEFT;//��ʼΪ�����ֵ��
	ARROW_t pre_direct=culmulate_direct;
	int arrow_len = jitter_tolerance*2;// ��ĳһ����չ����������ɢʱ�䳤��
	RESULT_t result = NOT_FOUND;
	line_status *backward_line = new line_status[arrow_len];//[0]Ϊ��һ���к�,[1]Ϊ����һ���к�...��ʱ����(����)��󿴣���ɢʱ��㣩������ʱ���״̬��¼
   	line_status *tmp_status = new line_status; 
	line_status jizhidian;
	memset(backward_line,0,arrow_len*sizeof(line_status));
	memset(&jizhidian,-1,sizeof(line_status));//��ʼ��״̬Ϊ-1��.

	for( i=start_line;i<size.height;i++) 
	{
		memset(tmp_status,0,sizeof(line_status));
		//*****fill tmp first****/
	
		// cal line num
		tmp_status->line_num = i;

		//cal  line weitht
		const uchar* pdata = (const uchar*)(src.data + src.step*i);//*

		for( j=0;j<size.width;j++)	
		{
				tmp_status->line_wight += (pdata[j]);	
		}
		//cal line dirction
		if(tmp_status->line_wight > backward_line[0].line_wight)////�����������
			tmp_status->line_direct = 1;
		else//������С
			tmp_status->line_direct = -1;
		
		//cal line total dirct duration
		tmp_status->line_direct_duration =  backward_line[0].line_direct_duration+tmp_status->line_direct;
		//printf("~line:%d,weith:%d,direct:%d,calmu:%d\n",tmp_status->line_num,tmp_status->line_wight,tmp_status->line_direct,tmp_status->line_direct_duration);

		//����ֲ�����under the specified jitter_tolerance	
		//����Ϊjitter_tolerance������£���þֲ���С��
		 pre_direct=culmulate_direct;

		 //Min:f(tmp)-f(T-1)<=-T
		 //Max:f(tmp)-f(T-1)>=T
		if(tmp_status->line_direct_duration - backward_line[jitter_tolerance-1].line_direct_duration>=jitter_tolerance)
		{//�ҵ��ֲ�������������
			culmulate_direct = UP;
			if (DOWN==pre_direct)
			{

				//trace(trace_level,"down->up,find min at line:%d,weight:%d\n",backward_line[jitter_tolerance-1].line_num,backward_line[jitter_tolerance-1].line_wight);

				result = FOUND;
				break;//��֮ǰ�ǲ�ͬ�����ƣ��ҵ���С��,����ѭ��
			}
		}
		else if(tmp_status->line_direct_duration-backward_line[jitter_tolerance-1].line_direct_duration<=-jitter_tolerance)
		{
			culmulate_direct = DOWN;
			if (UP==pre_direct)
			{
				//trace(trace_level,"up->down,find max at line:%d,weight:%d\n",backward_line[jitter_tolerance-1].line_num,backward_line[jitter_tolerance-1].line_wight);
				result = FOUND;
				break;//��֮ǰ�ǲ�ͬ�����ƣ��ҵ���С��,����ѭ��
			}
		}
		//update direction chain status 
		for(int k=arrow_len-1;k>0;k--)
			backward_line[k]=backward_line[k-1];		
		//update the last node
		backward_line[0] = *tmp_status;
	}

	if (result == FOUND)
	{
		jizhidian = backward_line[jitter_tolerance-1];
	}

	
	delete tmp_status;
	delete [] backward_line;
	return jizhidian;	
	
}

line_status findThejixiaoLine(const void *srcImg,int start_line, int jitter_tolerance)
{


	line_status tmp;
		memset(&tmp,0,sizeof(line_status));
		tmp.line_num = start_line;
	do
	{
		tmp = findThejiZhiLine(srcImg,tmp.line_num,jitter_tolerance);
		if(tmp.line_num==-1)//�쳣���������ͼ��ײ���û���ҵ���Сֵ��
			return tmp;

	}while(tmp.line_direct!= -1);

	trace(trace_level,"find jixiao line:%d,weight:%d\n",tmp.line_num,tmp.line_wight);

	return tmp;

}

//
line_status findThejidaLine(const void *srcImg,int start_line, int jitter_tolerance)
{
	line_status tmp;
	memset(&tmp,0,sizeof(line_status));
	tmp.line_num = start_line;
	do 
	{//��������
		tmp = findThejiZhiLine(srcImg,tmp.line_num,jitter_tolerance);
		if(tmp.line_num==-1)
			return tmp;

		
	} while (tmp.line_direct!=1);
	trace(trace_level,"find jida line:%d,weight:%d\n",tmp.line_num,tmp.line_wight);
	return tmp;
}



int cal_diff_sum_uchar(unsigned char* mather_line,unsigned char* sample_line, int width,int slide_tolerance)
{

	unsigned diff_sum=0,diff_sum_left=0,diff_sum_right=0;
	unsigned int *result_arry = new unsigned int[2*slide_tolerance+1];//2T+1
	memset(result_arry,0,2*slide_tolerance*sizeof(unsigned int));

		//������ʱ
		int shift=0;
		for (int j=0;j<width;j++)
		{
			diff_sum += abs((mather_line[j])-(sample_line[j]));

		}
		
		if(diff_sum==0)
			goto FIND_MIN;
		else 
			result_arry[0] = diff_sum; 

	//���һ���ʱ
	for (int i=1;i<=slide_tolerance;i++)
	{
		diff_sum = 0;
		shift++;
		//��mather_line[0]Ϊ����������similarline,������,similarline ��������ȳ�,�ȳ����ֲ�����	
			for (int j=0;j<width-i;j++)
			{		
				diff_sum_left += abs((mather_line[j])-(sample_line[j+i]));
				diff_sum_right += abs((mather_line[j+i])-(sample_line[j]));

			}
			//trace(trace_level,"shift  step:%d ,sum_left:%d,sum_right:%d\n",i,diff_sum_left,diff_sum_right);

			
			if(diff_sum_left==0)
			{
				diff_sum = diff_sum_left;
				goto FIND_MIN;
			}
			else if(diff_sum_right==0)
			{
				diff_sum = diff_sum_right;
				goto FIND_MIN;
			}
			else
			{
				result_arry[i*2-1] = diff_sum_left;
				result_arry[i*2] = diff_sum_right;
			}
		
	}
	//��result_arry��������СԪ�ء���Ϊ����ֵ
	//diff_sum��Ϊ��Сֵ��ʱֵ����ð�ݣ�ֻðһ����
	
	diff_sum = result_arry[0];
	for (int i=1;i<2*slide_tolerance+1;i++)
	{
		if (result_arry[i]<diff_sum)	
			diff_sum = result_arry[i];		
	}


FIND_MIN:
		delete [] result_arry;

		trace(trace_level,"find the min diff_sum after slide: %d step,diff sum:%d \n",shift,diff_sum);
		return diff_sum;
}

int cal_diff_sum_double(double* mather_line,double* sample_line, int width,int slide_tolerance)
{

	double diff_sum=0,diff_sum_left=0,diff_sum_right=0;
	double *result_arry = new double[2*slide_tolerance+1];//2T+1
	memset(result_arry,0,2*slide_tolerance*sizeof(double));

	//������ʱ
	int shift=0;
	for (int j=0;j<width;j++)
	{
		diff_sum += abs((mather_line[j]-sample_line[j]));

	}

	if(diff_sum==0)
		goto FIND_MIN;
	else 
		result_arry[0] = diff_sum; 

	//���һ���ʱ
	for (int i=1;i<=slide_tolerance;i++)
	{
		diff_sum = 0;
		shift++;
		//��mather_line[0]Ϊ����������similarline,������,similarline ��������ȳ�,�ȳ����ֲ�����	
		for (int j=0;j<width-i;j++)
		{		
			diff_sum_left += abs((mather_line[j])-(sample_line[j+i]));
			diff_sum_right += abs((mather_line[j+i])-(sample_line[j]));

		}
		//trace(trace_level,"shift  step:%d ,sum_left:%d,sum_right:%d\n",i,diff_sum_left,diff_sum_right);


		if(diff_sum_left==0)
		{
			diff_sum = diff_sum_left;
			goto FIND_MIN;
		}
		else if(diff_sum_right==0)
		{
			diff_sum = diff_sum_right;
			goto FIND_MIN;
		}
		else
		{
			result_arry[i*2-1] = diff_sum_left;
			result_arry[i*2] = diff_sum_right;
		}

	}
	//��result_arry��������СԪ�ء���Ϊ����ֵ
	//diff_sum��Ϊ��Сֵ��ʱֵ����ð�ݣ�ֻðһ����

	diff_sum = result_arry[0];
	for (int i=1;i<2*slide_tolerance+1;i++)
	{
		if (result_arry[i]<diff_sum)	
			diff_sum = result_arry[i];		
	}


FIND_MIN:
	delete [] result_arry;

	trace(trace_level,"find the min diff_sum after slide: %d step,diff sum:%.2f\n",shift,diff_sum);
	return diff_sum;
}

int cal_diff_sum_int(int* mather_line,int* sample_line, int width,int slide_tolerance)
{

	int diff_sum=0,diff_sum_left=0,diff_sum_right=0;
	int *result_arry = new int[2*slide_tolerance+1];//2T+1
	memset(result_arry,0,2*slide_tolerance*sizeof(int));

	//������ʱ
	int shift=0;
	for (int j=0;j<width;j++)
	{
		diff_sum += abs((mather_line[j]-sample_line[j]));

	}

	if(diff_sum==0)
		goto FIND_MIN;
	else 
		result_arry[0] = diff_sum; 

	//���һ���ʱ
	for (int i=1;i<=slide_tolerance;i++)
	{
		diff_sum = 0;
		shift++;
		//��mather_line[0]Ϊ����������similarline,������,similarline ��������ȳ�,�ȳ����ֲ�����	
		for (int j=0;j<width-i;j++)
		{		
			diff_sum_left += abs((mather_line[j])-(sample_line[j+i]));
			diff_sum_right += abs((mather_line[j+i])-(sample_line[j]));

		}
		//trace(trace_level,"shift  step:%d ,sum_left:%d,sum_right:%d\n",i,diff_sum_left,diff_sum_right);


		if(diff_sum_left==0)
		{
			diff_sum = diff_sum_left;
			goto FIND_MIN;
		}
		else if(diff_sum_right==0)
		{
			diff_sum = diff_sum_right;
			goto FIND_MIN;
		}
		else
		{
			result_arry[i*2-1] = diff_sum_left;
			result_arry[i*2] = diff_sum_right;
		}

	}
	//��result_arry��������СԪ�ء���Ϊ����ֵ
	//diff_sum��Ϊ��Сֵ��ʱֵ����ð�ݣ�ֻðһ����

	diff_sum = result_arry[0];
	for (int i=1;i<2*slide_tolerance+1;i++)
	{
		if (result_arry[i]<diff_sum)	
			diff_sum = result_arry[i];		
	}


FIND_MIN:
	delete [] result_arry;

	trace(trace_level,"find the min diff_sum after slide: %d step,diff sum:%d\n",shift,diff_sum);
	return diff_sum;
}


line_pair findMinCycle(const void* srcImg,int start_line_type, int jitter_tolerance, int slide_tolerance)
{
	cv::Mat src = cv::cvarrToMat(srcImg);
	cv::Size size = src.size();
	assert(src.channels() == 1);

	trace(trace_level,"findMinCycle():start_line_type:%s,jitter_tolerance:%d,slide_tolerance:%d\n",(start_line_type==MIN?"MIN":"MAX"),jitter_tolerance,slide_tolerance);

	SList *list = NULL;
	 line_pair pair;
	line_status tmp;
	memset(&tmp,0,sizeof(line_status));
	memset(&pair,0,sizeof(line_pair));
	int mather_line =-1;
	int most_similar_line = -1;

	//get the line pair
	if (start_line_type==MAX)
	{
		//find first line
		tmp = findThejixiaoLine(srcImg,tmp.line_num,jitter_tolerance);
		 pair.start_num = tmp.line_num;

		 //find the other line
		 while(1) 
		 {
			 tmp = findThejixiaoLine(srcImg,tmp.line_num,jitter_tolerance);
			 if(tmp.line_num!=-1)
			 {
				 list = slist_append_int(list,tmp.line_num);
				continue;
			 }
			 else
				 break;

		 } 

	}
	else 
	{
		assert(start_line_type==MAX);
		tmp  = findThejidaLine(srcImg,tmp.line_num,jitter_tolerance);
		pair.start_num = tmp.line_num;

		while(1)
		{
			tmp = findThejidaLine(srcImg,tmp.line_num,jitter_tolerance);
			if(tmp.line_num!=-1)
			{
				list = slist_append_int(list,tmp.line_num);
				continue;
			}
			else
				break;
		}

	}
	if (list==NULL)
	{
		llf_error("no simlilar line find for line:%d\n",pair.start_num);
		pair.end_num = -1;
		return pair;
	}
		
	 //cal the similar line

	SList *tmp_list = list;
	int diff_sum=0;
	int width = size.width;
	//����listԪ��Ϊ�кţ���cal_diff_sum�󣬽����ƶȲ�����������
	while (tmp_list)
	{ 
		uchar *pstartline = src.data + src.step*pair.start_num;
		uchar *pendline = src.data + src.step*tmp_list->value;
		
		diff_sum = cal_diff_sum_uchar(pstartline,pendline,width,slide_tolerance);
		 //����Сsum
		 if (tmp_list == list)
		 {
			 pair.slid_sum_diff = diff_sum;
			 pair.end_num = tmp_list->value;
		 }
		 if(diff_sum<pair.slid_sum_diff)
		 {
			 pair.slid_sum_diff = diff_sum;
			pair.end_num = tmp_list->value;

		 }

		tmp_list = tmp_list->next;
	}

	trace(trace_level," find start_num:%d, end_num:%d, type:%s\n",pair.start_num,pair.end_num,start_line_type == MIN?"MIN":"MAX");
		return pair;
}


