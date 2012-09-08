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
	int line_num;//行号	
	int line_wight;//行重量。非0元素重量按1算
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
 *int findTheLighterLine(char ** arr,int row, int col)
 *功能：find the lighter line in 2-dim array.即寻找局部重量最小行。
 *每行重量对每个元素累加，大于零的元素加1.
 *return the number of the line which has the lighter wight.
 *note:从第0行开始，向下，考虑重量在一直在减小后的反向上升，则在取到连续2个连续上升的行后，停止搜索。
 **********************不考虑重量一直在增大后的反向下降
unsigned char **arr, 输入二维图像数组
int row, 行
int col,列
int start_line ,起始行，表示从哪一行开始计算
int jitter_tolerance 代表朝某一方向发展时的抖动误差，即所允许的局部反向误差
返回值:为极值点，line_direct为-1时为极小点，为1时极大点
			返回line_status成员中line_num为-1时，表示到达图像尾部
 */
line_status  findThejiZhiLine(unsigned char ** arr,int row, int col ,int start_line = 0,int arrow_len = 3);


/*findThejixiaoLine
 *功能：找到极小值所在行,调用findThejiZhiLine对其做大小判断，再返回
 返回：返回line_status类型，
 */
line_status findThejixiaoLine(unsigned char **arr,int row, int col,int start_line, int jitter_tolerance);

/*int cal_diff_sum
两个数组滑动后进行对比,求累差。如果两数组相似，累差和将较小。
unsigned char* cmp_line, 固定的一维数组，表示一行像素。
unsigned char* similar_line,用来与固定数组进行比较的一维数组。
int col,数组维数
int slide_tolerance，滑动公差，similar_line在tolerance范围内滑动后与cmp_line进行对比
*返回： cmp_line与similar_line的各元素差的最小累差和
*/
int cal_diff_sum(unsigned char* cmp_line,unsigned char* similar_line, int col,int tolerance=1);


//返回值：line_pair.包括起始行，结束行，以及最大还是最小行循环。
//错误：当end_line==-1，表示直到图像结束没有找到匹配的行。
line_pair findMinCycle(unsigned char **arr,int row, int col,int start_line_type, int jitter_tolerance, int slide_tolerance);

extern int jitter_tolerance;
extern int slide_tolerance;
extern int start_line_type;

#endif