//跟数理统计相关的一些函数

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
	int line_num;//行号	
	unsigned line_wight;//行重量。非0元素重量按1算
	int line_direct;//此行相对于上一行的改变
	int line_direct_duration;//此行相对于前面行数的重量方向累积量。
}line_status;//用于findThejiZhiLine

typedef struct
{
	int start_num;
	int end_num;
	int min_or_max;//两个数组是小重量还是大重量pair
	int slid_sum_diff;//2个数组的最小差值
}line_pair;


/*
int start_line ,起始行，表示从哪一行开始计算
int jitter_tolerance 代表朝某一方向发展时的抖动误差，即所允许的局部反向误差
返回值:为极值点，line_direct为-1时为极小点，为1时极大点
			返回line_status成员中line_num为-1时，表示到达图像尾部
 */
line_status  findThejiZhiLine(const void *srcImg ,int start_line = 0,int arrow_len = 3);


/*findThejixiaoLine
 *功能：找到极小值所在行,调用findThejiZhiLine对其做大小判断，再返回
 返回：返回line_status类型，
 */
line_status findThejixiaoLine(const void *srcImg,int start_line, int jitter_tolerance);

//功能：找到极小值所在行,调用findThejiZhiLine对其做大小判断，再返回
//返回：返回line_status类型，
line_status findThejidaLine(const void *srcImg,int start_line, int jitter_tolerance);

/*int cal_diff_sum
两个数组滑动后进行对比,求累差。如果两数组相似，累差和将较小。
unsigned char* mather_line, 固定的一维数组，表示一行像素,mather_line: the model line to be compared,。
unsigned char* sample_line,用来与固定数组进行比较的一维数组。
int slide_tolerance，滑动公差，sample_line在tolerance范围内滑动后与mather_line进行对比
int width:  数组的长度
*返回： mather_line与sample_line的各元素差的最小累差和
*/
int cal_diff_sum_uchar(unsigned char* mather_line,unsigned char* sample_line, int width,int slide_tolerance);

//返回： mather_line与sample_line的各元素差的最小累差和
int cal_diff_sum_double(double* mather_line,double* sample_line, int width,int slide_tolerance);

//返回： mather_line与sample_line的各元素差的最小累差和
int cal_diff_sum_int(int* mather_line,int* sample_line, int width,int slide_tolerance);

//返回值：line_pair.包括起始行，结束行，以及最大还是最小行循环。
//错误：当end_line==-1，表示直到图像结束没有找到匹配的行。
line_pair findMinCycle(const void* srcImg,int start_line_type, int jitter_tolerance, int slide_tolerance);
extern int jitter_tolerance;
extern int slide_tolerance;
extern int start_line_type;

#endif