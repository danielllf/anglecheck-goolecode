#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/global.h"
#include "../include/statistic.h"

int jitter_tolerance = 0;
int slide_tolerance = 0;
int start_line_type = 0;


line_status findThejiZhiLine(unsigned char **arr,int row, int col,int start_line, int jitter_tolerance)
{
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

	for( i=start_line;i<row;i++) 
	{
		memset(tmp_status,0,sizeof(line_status));
		//*****fill tmp first****/
	
		// cal line num
		tmp_status->line_num = i;

		//cal  line weitht
		for( j=0;j<col;j++)	
				tmp_status->line_wight += (arr[i][j]+MAX_PIX_VALUE-1)/MAX_PIX_VALUE;			

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

				trace(trace_level,"down->up,find min at line:%d\n",backward_line[jitter_tolerance-1].line_num);

				result = FOUND;
				break;//��֮ǰ�ǲ�ͬ�����ƣ��ҵ���С��,����ѭ��
			}
		}
		else if(tmp_status->line_direct_duration-backward_line[jitter_tolerance-1].line_direct_duration<=-jitter_tolerance)
		{
			culmulate_direct = DOWN;
			if (UP==pre_direct)
			{
				trace(trace_level,"up->down,find max at line:%d\n",backward_line[jitter_tolerance-1].line_num);
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

line_status findThejixiaoLine(unsigned char **arr,int row, int col,int start_line, int jitter_tolerance)
{
	line_status tmp;
		memset(&tmp,0,sizeof(line_status));
		tmp.line_num = start_line;
	do
	{
		tmp = findThejiZhiLine(arr,row,col,tmp.line_num,jitter_tolerance);
		if(tmp.line_num==-1)//�쳣���������ͼ��ײ���û���ҵ���Сֵ��
			return tmp;

	}while(tmp.line_direct!= -1);

	trace(trace_level,"find jixiao line:%d\n",tmp.line_num);

	return tmp;

}

//
line_status findThejidaLine(unsigned char **arr,int row, int col,int start_line, int jitter_tolerance)
{
	line_status tmp;
	memset(&tmp,0,sizeof(line_status));
	tmp.line_num = start_line;
	do 
	{//��������
		tmp = findThejiZhiLine(arr,row,col,tmp.line_num,jitter_tolerance);
		if(tmp.line_num==-1)
			return tmp;

		
	} while (tmp.line_direct!=1);
   trace(trace_level,"find jida line:%d\n",tmp.line_num);
	return tmp;
}



int cal_diff_sum(unsigned char* cmp_line,unsigned char* similar_line, int col,int slide_tolerance)
{

	unsigned diff_sum=0,diff_sum_left=0,diff_sum_right=0;
	unsigned int *result_arry = new unsigned int[2*slide_tolerance+1];//2T+1
	memset(result_arry,0,2*slide_tolerance*sizeof(unsigned int));

		//������ʱ
		for (int j=0;j<col;j++)
		{
			diff_sum += abs(cmp_line[j]-similar_line[j]);

		}
		
		if(diff_sum==0)
			goto FIND_MIN;
		else 
			result_arry[0] = diff_sum; 

	//���һ���ʱ
	for (int i=1;i<=slide_tolerance;i++)
	{
		diff_sum = 0;	
		//��cmp_line[0]Ϊ����������similarline,������,similarline ��������ȳ�,�ȳ����ֲ�����	
			for (int j=0;j<col-i;j++)
			{		
				diff_sum_left += abs(cmp_line[j]-similar_line[j+i]);
				diff_sum_right += abs(cmp_line[j+i]-similar_line[j]);

			}
			trace(trace_level,"shift  step:%d ,sum_left:%d,sum_right:%d\n",i,diff_sum_left,diff_sum_right);

			
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

	trace(trace_level,"find the min diff_sum after slide,diff sum:%d \n",diff_sum);

FIND_MIN:
		delete [] result_arry;
		return diff_sum;
}



line_pair findMinCycle(unsigned char **arr,int row, int col,int start_line_type, int jitter_tolerance, int slide_tolerance)
{
	trace(trace_level,"findMinCycle():start_line_type:%s,jitter_tolerance:%d,slide_tolerance:%d\n",(start_line_type==MIN?"MIN":"MAX"),jitter_tolerance,slide_tolerance);

	SList *list = NULL;
	 line_pair pair;
	line_status tmp;
	memset(&tmp,0,sizeof(line_status));
	memset(&pair,0,sizeof(line_pair));
	int cmp_line =-1;
	int most_similar_line = -1;

	//get the line pair
	if (start_line_type==MIN)
	{
		//find first line
		tmp = findThejixiaoLine(arr,row,col,tmp.line_num,jitter_tolerance);
		 pair.start_num = tmp.line_num;

		 //find the other line
		 while(1) 
		 {
			 tmp = findThejixiaoLine(arr,row,col,tmp.line_num,jitter_tolerance);
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
		tmp  = findThejidaLine(arr,row,col,tmp.line_num,jitter_tolerance);
		pair.start_num = tmp.line_num;

		while(1)
		{
			tmp = findThejidaLine(arr,row,col,tmp.line_num,jitter_tolerance);
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
	//����listԪ��Ϊ�кţ���cal_diff_sum�󣬽����ƶȲ�����������
	while (tmp_list)
	{
		diff_sum = cal_diff_sum(arr[pair.start_num],arr[tmp_list->value],col,slide_tolerance);
		 //˳������Сsum
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


