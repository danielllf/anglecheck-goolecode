/*
*铅笔匹配法
*/
#ifndef PENCIL_H_
#define PENCIL_H_

#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>

#define MAX_PENCIL_CNT 1024 //标准图中铅笔最大个数
#define HIST_BINS 100 //每个直方图中BIN的个数,本设计中只有一维直方图
//#define MAX_HDR_WIDTH 1024

//代表计算区段的结构体
typedef struct{
	CvRect rect;
	CvHistogram *hist;
	double mean;//均值
	int* grads;//梯度
	int grads_len;//梯度数组长度,每个元素为一个像素时，其值等于sect.width
	int grads_step;//梯度数组的每个元素rect的宽度
	int line_num;//铅笔心所在的行号
	
}Section_t;

typedef struct{
	int line_num;
	double leicha;
	double histDiff;
	double meanDiff;
	double gradDiff;
}Cmp_t;

/*
*fun:计算一个铅笔头部的特征信息
*src:用于计算的源图像 
*rect:铅笔头所在区域
*sec:将计算的结果存入sec中
*/
void cal_one_pen_head(IplImage* src,Section_t*  sec);

//need all arrays:sec[] and grads[] allocted first.
void cal_mather_header(IplImage* src,Section_t* sec, int hdr_width,int hdr_height);

/*
*fun:基于pencil header, 用1个sample与1个model 进行对比。
*sample:head采样区
*slide_tolerance：用于进行梯度对比时的滑动像素个数.
							当为-1时，将不进行梯度信息计算
							当>=0时，计算梯度信息
*	model:head模板区
*返回：cmp_t{行号，对应的累差}
*/
Cmp_t cmp_head(Section_t* sample,Section_t* model,int slide_tolerance);

//从预存于内存的model(数组或链表)中按索引取其指针
//写此函数为了用于可能的将数组改为动态存储
Section_t* getMatherSecByIdx(int idx);

/*
*查找sample header对应于modes header中的那一个model
*src:sample图
*line_num:从sample图sample head所在的行
*hdr_width,hdr_height:铅笔头所对应矩形的宽度,高度
*grads_slide_tolerance：计算梯度累差时的数组滑动误差，							
									当为-1时，将不进行梯度信息计算
									当>=0时，计算梯度信息
返回： hdr sample对应于model图中的行号
*/
int find_sample_hdr_pos(IplImage *src,int target_line_num,int hdr_width,int hdr_height,int grads_slide_tolerance);

/*
*初始化mather图中所有header sec
*
*/
void initMatherHeaderSects(IplImage* matherImg,int headWidth,int headHeight,int gradStep);

/*
*free mather图中所有header sec[]
*
*/
void freeMatherHeadSecs();
#endif