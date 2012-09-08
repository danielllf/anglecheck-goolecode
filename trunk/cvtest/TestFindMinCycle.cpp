#include <stdio.h>
#include <string.h>

#include "global.h"

#if 0

int main()
{
	IplImage *dst_bw=NULL;
	parse_configfile("calangle.conf");


	//�õ���ͨ���Ҷ�ͼ��
	IplImage *src = cvLoadImage("geZiBu320.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	//IplImage *src = cvLoadImage("lianpeng.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	if(!src)	perror("erro file\n");
	cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
	cvShowImage("src",src);
	printf("src=>height:%d\twidth:%d\twstep:%d\tdepth:%d\talign:%d\tchannel:%d,nsize:%d\t\n",src->height,src->width,src->widthStep,src->depth,src->align,src->nChannels,src->nSize);
   
	//�ߴ����,��С��������ͬʱ��С����
	IplImage *src1=cvCreateImage(cvSize(cvGetSize(src).width,cvGetSize(src).height),IPL_DEPTH_8U,1);
	cvResize(src,src1);
	IMG_SHOW(src1,src1);
	printf("src1=>height:%d\twidth:%d\twstep:%d\tdepth:%d\talign:%d\tchannel:%d,nsize:%d\t\n",src1->height,src1->width,src1->widthStep,src->depth,src1->align,src1->nChannels,src1->nSize);


	//��ֵ�˲�������С��������
	cvSmooth(src1, src1, CV_MEDIAN, 3, 0, 0, 0); 

	//ת���Ҷ�Ϊ�ڰ�
	 dst_bw = cvCreateImage(cvGetSize(src1),IPL_DEPTH_8U,1);
	 cvThreshold( src1, dst_bw ,50, 255, CV_THRESH_BINARY_INV ); //ȡ��ֵΪ50��ͼ��תΪ��ֵͼ��
	 cvNamedWindow("blackw",CV_WINDOW_AUTOSIZE);
	cvShowImage("blackw",dst_bw);

	//��̬ѧ��������ʴ������
	IplImage *erd = cvCreateImage( cvGetSize(dst_bw), dst_bw->depth, dst_bw->nChannels);
	IplImage *src_erode = cvCreateImage( cvGetSize(erd), erd->depth, erd->nChannels);
	IplConvKernel *element = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_CROSS, 0);
    
	cvErode(dst_bw,erd,element,7);
	cvDilate(erd,src_erode,element,7);
	IMG_SHOW(src_erode,src_erode);



	cv::Mat ma = cv::cvarrToMat(src_erode);
    
	////����Сcycle
	line_pair pair = findMinCycle(src_erode,start_line_type,jitter_tolerance,slide_tolerance);
	printf("mincycle start:%d,end:%d,diff_sum:%d\n",pair.start_num,pair.end_num,pair.slid_sum_diff);
	mklinecolor(&ma,pair.start_num,color_line_tolerance,line_color);
	mklinecolor(&ma,pair.end_num,color_line_tolerance,line_color);

	cvNamedWindow("blkline");
	cvShowImage("blkline",src_erode);

	cvWaitKey();
	printf("exit with 0\n");
	getchar();
	return 0;
}


unsigned char testarray[WIDTH_MATHER_COL*HEIGHT_MATHER_ROW]={
	0,0,0,0,1,1,1,0,1,0,0,0,0,0,1,1,1,1,1,0,
	0,0,0,0,1,1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,
	0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,//[1][2]
	0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,
	0,0,0,1,1,1,1,1,0,0, 0,0,0,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,
	0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,//[7][10]
	0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,//[8][6]
	0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,//[9][2]
	0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,//[10][2]
	0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,//[11][6]
	0,0,0,1,1,1,1,1,0,0, 0,0,0,1,1,1,1,1,0,0,//
	0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,
	0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,//
	0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0//
};


//int main()
//{
//	//�õ���ͨ���Ҷ�ͼ��
//	cvNamedWindow("win2",CV_WINDOW_AUTOSIZE);
//	//load to 1 changel grayscale
//	IplImage *img = cvLoadImage("geZiBu.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//	if(!img)printf("erro file\n");
//
//
//	cvShowImage("win2",img);
//	printf("height:%d\twidth:%d\twstep:%d\tdepth:%d\talign:%d\tchannel:%d,imageSize:%d,nsize:%d\t\n",img->height,img->width,img->widthStep,img->depth,img->align,img->nChannels,img->imageSize,img->nSize);
//	// a.����ʵ���ֵ��b.������Ӧ��ֵ�����ж�ֵ��
//	
//	//a.1
//	//a.2
//	//ת���Ҷ�Ϊ�ڰ�
//	cvNamedWindow("blackw",CV_WINDOW_AUTOSIZE);
//	IplImage *dst_bw = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
//	 cvThreshold( img, dst_bw ,50, 255, CV_THRESH_BINARY ); //ȡ��ֵΪ50��ͼ��תΪ��ֵͼ��
//	
//	cvShowImage("blackw",dst_bw);
//   cvWaitKey(0);
//	return 0;
//}

void foreach()
{
	IplImage *img = cvLoadImage("geZiBu.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	if(!img)printf("erro file\n");

	int i,j;
	uchar *pchar;
	for (i=0;i<img->height;i++)
	{
		pchar = (uchar *)img->imageData+i*img->widthStep;
		for (j=0;j<img->width;j++)
		{
			uchar* tmp = pchar+j*img->nChannels;
			if (*tmp>=128)
			{
				*tmp = 255;
			}
			else{
				*tmp = 0;
			}
		}
	}
}

#endif