/*
*Ǧ��ƥ�䷨
*/
#ifndef PENCIL_H_
#define PENCIL_H_

#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>

#define MAX_PENCIL_CNT 1024 //��׼ͼ��Ǧ��������
#define HIST_BINS 100 //ÿ��ֱ��ͼ��BIN�ĸ���,�������ֻ��һάֱ��ͼ
//#define MAX_HDR_WIDTH 1024

//����������εĽṹ��
typedef struct{
	CvRect rect;
	CvHistogram *hist;
	double mean;//��ֵ
	int* grads;//�ݶ�
	int grads_len;//�ݶ����鳤��,ÿ��Ԫ��Ϊһ������ʱ����ֵ����sect.width
	int grads_step;//�ݶ������ÿ��Ԫ��rect�Ŀ��
	int line_num;//Ǧ�������ڵ��к�
	
}Section_t;

typedef struct{
	int line_num;
	double leicha;
	double histDiff;
	double meanDiff;
	double gradDiff;
}Cmp_t;

/*
*fun:����һ��Ǧ��ͷ����������Ϣ
*src:���ڼ����Դͼ�� 
*rect:Ǧ��ͷ��������
*sec:������Ľ������sec��
*/
void cal_one_pen_head(IplImage* src,Section_t*  sec);

//need all arrays:sec[] and grads[] allocted first.
void cal_mather_header(IplImage* src,Section_t* sec, int hdr_width,int hdr_height);

/*
*fun:����pencil header, ��1��sample��1��model ���жԱȡ�
*sample:head������
*slide_tolerance�����ڽ����ݶȶԱ�ʱ�Ļ������ظ���.
							��Ϊ-1ʱ�����������ݶ���Ϣ����
							��>=0ʱ�������ݶ���Ϣ
*	model:headģ����
*���أ�cmp_t{�кţ���Ӧ���۲�}
*/
Cmp_t cmp_head(Section_t* sample,Section_t* model,int slide_tolerance);

//��Ԥ�����ڴ��model(���������)�а�����ȡ��ָ��
//д�˺���Ϊ�����ڿ��ܵĽ������Ϊ��̬�洢
Section_t* getMatherSecByIdx(int idx);

/*
*����sample header��Ӧ��modes header�е���һ��model
*src:sampleͼ
*line_num:��sampleͼsample head���ڵ���
*hdr_width,hdr_height:Ǧ��ͷ����Ӧ���εĿ��,�߶�
*grads_slide_tolerance�������ݶ��۲�ʱ�����黬����							
									��Ϊ-1ʱ�����������ݶ���Ϣ����
									��>=0ʱ�������ݶ���Ϣ
���أ� hdr sample��Ӧ��modelͼ�е��к�
*/
int find_sample_hdr_pos(IplImage *src,int target_line_num,int hdr_width,int hdr_height,int grads_slide_tolerance);

/*
*��ʼ��matherͼ������header sec
*
*/
void initMatherHeaderSects(IplImage* matherImg,int headWidth,int headHeight,int gradStep);

/*
*free matherͼ������header sec[]
*
*/
void freeMatherHeadSecs();
#endif