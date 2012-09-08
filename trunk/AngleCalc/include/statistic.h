//������ͳ����ص�һЩ����

#ifndef _STATISTIC_H
#define _STATISTIC_H
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#define MAX_PIX_VALUE 255
//#define STDVAL(val) ((val+MAX_PIX_VALUE-1)/MAX_PIX_VALUE) //if val == 0:return 0,else return 1

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
	unsigned line_wight;//����������0Ԫ��������1��
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
int start_line ,��ʼ�У���ʾ����һ�п�ʼ����
int jitter_tolerance ����ĳһ����չʱ�Ķ�������������ľֲ��������
����ֵ:Ϊ��ֵ�㣬line_directΪ-1ʱΪ��С�㣬Ϊ1ʱ�����
			����line_status��Ա��line_numΪ-1ʱ����ʾ����ͼ��β��
 */
line_status  findThejiZhiLine(const void *srcImg ,int start_line = 0,int arrow_len = 3);


/*findThejixiaoLine
 *���ܣ��ҵ���Сֵ������,����findThejiZhiLine��������С�жϣ��ٷ���
 ���أ�����line_status���ͣ�
 */
line_status findThejixiaoLine(const void *srcImg,int start_line, int jitter_tolerance);

//���ܣ��ҵ���Сֵ������,����findThejiZhiLine��������С�жϣ��ٷ���
//���أ�����line_status���ͣ�
line_status findThejidaLine(const void *srcImg,int start_line, int jitter_tolerance);

/*int cal_diff_sum
�������黬������жԱ�,���۲������������ƣ��۲�ͽ���С��
unsigned char* mather_line, �̶���һά���飬��ʾһ������,mather_line: the model line to be compared,��
unsigned char* sample_line,������̶�������бȽϵ�һά���顣
int slide_tolerance���������sample_line��tolerance��Χ�ڻ�������mather_line���жԱ�
int width:  ����ĳ���
*���أ� mather_line��sample_line�ĸ�Ԫ�ز����С�۲��
*/
int cal_diff_sum_uchar(unsigned char* mather_line,unsigned char* sample_line, int width,int slide_tolerance);

//���أ� mather_line��sample_line�ĸ�Ԫ�ز����С�۲��
int cal_diff_sum_double(double* mather_line,double* sample_line, int width,int slide_tolerance);

//���أ� mather_line��sample_line�ĸ�Ԫ�ز����С�۲��
int cal_diff_sum_int(int* mather_line,int* sample_line, int width,int slide_tolerance);

//����ֵ��line_pair.������ʼ�У������У��Լ��������С��ѭ����
//���󣺵�end_line==-1����ʾֱ��ͼ�����û���ҵ�ƥ����С�
line_pair findMinCycle(const void* srcImg,int start_line_type, int jitter_tolerance, int slide_tolerance);
extern int jitter_tolerance;
extern int slide_tolerance;
extern int start_line_type;

#endif