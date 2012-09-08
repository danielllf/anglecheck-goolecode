#ifndef _STATISTIC_H
#define _STATISTIC_H
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#define MAX_PIX_VALUE 255

typedef enum {
	DOWN = 0x01,
	UP   = 0x02,
	LEFT = 0x03
}ARROW_t;
typedef enum{
	FOUND = 0x01,
	NOT_FOUND = 0x02
}RESULT_t;

typedef enum{
	MIN = 0x00,
	MAX = 0x01
};
typedef struct 
{
	int line_num;//�к�	
	int line_wight;//����������0Ԫ��������1��
	int line_direct;//�����������һ�еĸı�
	int line_direct_duration;//���������ǰ�����������������ۻ�����
}line_status;//����findThejiZhiLine

typedef struct
{
	int start_num;
	int end_num;
	int min_or_max;//����������С�������Ǵ�����pair
	int slid_sum_diff;//2���������С��ֵ
}line_pair;


/*
 *int findTheLighterLine(char ** arr,int row, int col)
 *���ܣ�find the lighter line in 2-dim array.��Ѱ�Ҿֲ�������С�С�
 *ÿ��������ÿ��Ԫ���ۼӣ��������Ԫ�ؼ�1.
 *return the number of the line which has the lighter wight.
 *note:�ӵ�0�п�ʼ�����£�����������һֱ�ڼ�С��ķ�������������ȡ������2�������������к�ֹͣ������
 **********************����������һֱ�������ķ����½�
unsigned char **arr, �����άͼ������
int row, ��
int col,��
int start_line ,��ʼ�У���ʾ����һ�п�ʼ����
int jitter_tolerance ����ĳһ����չʱ�Ķ�������������ľֲ��������
����ֵ:Ϊ��ֵ�㣬line_directΪ-1ʱΪ��С�㣬Ϊ1ʱ�����
			����line_status��Ա��line_numΪ-1ʱ����ʾ����ͼ��β��
 */
line_status  findThejiZhiLine(unsigned char ** arr,int row, int col ,int start_line = 0,int arrow_len = 3);


/*findThejixiaoLine
 *���ܣ��ҵ���Сֵ������,����findThejiZhiLine��������С�жϣ��ٷ���
 ���أ�����line_status���ͣ�
 */
line_status findThejixiaoLine(unsigned char **arr,int row, int col,int start_line, int jitter_tolerance);

/*int cal_diff_sum
�������黬������жԱ�,���۲������������ƣ��۲�ͽ���С��
unsigned char* cmp_line, �̶���һά���飬��ʾһ�����ء�
unsigned char* similar_line,������̶�������бȽϵ�һά���顣
int col,����ά��
int slide_tolerance���������similar_line��tolerance��Χ�ڻ�������cmp_line���жԱ�
*���أ� cmp_line��similar_line�ĸ�Ԫ�ز����С�۲��
*/
int cal_diff_sum(unsigned char* cmp_line,unsigned char* similar_line, int col,int tolerance=1);


//����ֵ��line_pair.������ʼ�У������У��Լ��������С��ѭ����
//���󣺵�end_line==-1����ʾֱ��ͼ�����û���ҵ�ƥ����С�
line_pair findMinCycle(unsigned char **arr,int row, int col,int start_line_type, int jitter_tolerance, int slide_tolerance);

extern int jitter_tolerance;
extern int slide_tolerance;
extern int start_line_type;

#endif