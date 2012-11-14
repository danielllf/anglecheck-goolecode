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
	int grads_len;//�ݶ����鳤��,ÿ��Ԫ��Ϊһ������ʱ����ֵ����sect.width;�������ΪһԪ��ʱ����grads_len��=sect length
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

typedef struct PenParam{
	 Section_t *pSect;//ָ��mather pic section�����׵�ַ
	 Section_t * pMatherBodySect;//ָ��mather pen body sect �ڴ�����
	 Section_t * pSampleBodySect;//ָ��sample body sect �ڴ�����
	 int hdrSecLen;//header ����ĳ���
	 int bodySecLen;//body ÿ��������ĳ���
	int gradsLen;
	int gradStep;
	int matherSecHdrCnt;// Ǧ����ģ���еĸ���(����)
	int bodySecCnt;//һ��Ǧ���а�������������
	CvSize bodySecSize;
}PenParam;
/**
*��ʼ���������ò������������ļ�����
*/
void initParms();

/*
*fun:����һ��Ǧ��ͷ����������Ϣ
*src:���ڼ����Դͼ�� 
*rect:Ǧ��ͷ��������
*sec:������Ľ������sec��
*/
void cal_one_pen_sect(IplImage* src,Section_t*  sec);

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
*gradStep:�ݶ����ֳɵ�С����������������
*/
void initMatherHeaderSects(IplImage* matherImg,int headWidth,int headHeight,int gradStep);

/*
*free matherͼ������header sec[]
*return:-1 erro
*/
int freeMatherHeadSecs();
#endif