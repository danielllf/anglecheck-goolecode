#include "cv.h"
#include "highgui.h"
#include "../include/global_def.h"
#include "headers.h"
#include "llfutility.h"
#include <list>
#include "prepare.h"
void get_arg(char *buf, int buf_size, const char **pp);
int parse_configfile(const char* filename);
void initParms();

/*
1>��ȡlinepitch����ʱ��ͼ��linelen�����ڲ����д�ֱ�ֽ��ߣ�
2>���ÿ����ͼ�Ĵ�ֱ�ֽ���
3>����ÿ����ͼ��ƫ���������
4>��ÿ����ͼ���������ϲ���һ��ͼ����������

*/
#if 0
int main(int argc, char ** argv)
{
	int thresholdBW = 10;
	//cvNamedWindow("image");
	IplImage * src = cvLoadImage("6955_600_400_1.bmp", 0);
	if(src==NULL){printf("load file erro\n");return -1;}


IplImage *subImg =g_CopyRectFromImg(src,cvRect(440,0,src->width-440,src->height));
cvSaveImage("6955_600_400_1_right.bmp",subImg);
return 0;
	IplImage *scaledImg = g_resizeImage(subImg,1.00);

	IplImage * temp = cvCreateImage(cvGetSize(scaledImg), 8,1);
	IplImage * img=cvCreateImage(cvGetSize(scaledImg), 8, 1);
	cvCopyImage(scaledImg,temp);
	cvCopyImage(scaledImg, img);

	 IplConvKernel *element = 0; //������̬ѧ�ṹָ��
    element = cvCreateStructuringElementEx(3,3, 1, 1, CV_SHAPE_RECT, 0);//3,5,7
	
	
	//����ñ��
	cvMorphologyEx(
		scaledImg,
		img,
		temp,
		NULL,//element,// //default 3*3
		CV_MOP_BLACKHAT,//CV_MOP_BLACKHAT,
		1);
	//cvShowImage("src", src);
	cvShowImage("imagecaled", scaledImg);
	cvShowImage("image", img);
	cvWaitKey();
	
	IplImage* dst = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	cvThreshold( img, dst,thresholdBW, 255, CV_THRESH_BINARY ); //ȡ��ֵ��ͼ��תΪ��ֵͼ��
  

	//cvErode( scaledImg, scaledImg,element);//��ʴ
	//cvThreshold( dst, dst,thresholdBW, 255, CV_THRESH_BINARY );
	 //cvAdaptiveThreshold(img,dst,255.0,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,3,6);
	cvSaveImage("dstnosmooElipseshizix2.bmp",dst);
	cvShowImage("scaledImg", dst);
	cvWaitKey();
	 
	cvReleaseImage(&temp);
	cvReleaseImage(&src);
	cvReleaseImage(&img);
	cvReleaseImage(&scaledImg);
	cvDestroyAllWindows();
	return 0;
}
#endif

#if 0
int main()
{
	initParms();
	int thresholdBW = 10;
	IplImage * src1 = cvLoadImage("6955_600_400.bmp", 0);
	if(src1==NULL){printf("load file erro\n");return -1;}  
  log_erro("hello\n");


	//CORDINATE_PAIR cordPair= getX_cordinateofVerticalBar(src1,14,1.5,TARGETTYPE_RIGHT,10,305,true);
	

	//cvSaveImage("6955_600_400.bmp",subImg);
	
	IplImage* src=getMorphologyImg(src1,CV_MOP_TOPHAT,false,10);
	
	PrintTime("mainStarrrr");
	int pitch= getLinePitchProcess(src);
	printf("pitch:%d\n",pitch);
	PrintTime("mainEnd");

   vectorPoint vec;
   int secCnt = 15;
   int startline = getShiftPos(src,pitch, allowedPercentTOLwhenShifting,continuesLinecount,secCnt,vec);
   printf("start line:%d\n",startline);
	printVecPoint(vec);

	PrintTime("after getShiftPos");

   	IMG_SHOW("src",src);
	cvSaveImage("aftersrc.jpg",src);
	 cvWaitKey();
	cvReleaseImage(&src);
	return 0;
}
#endif
#if 9
int main()
{
	initParms();
	int subImgCnt=3;
	vectorPoint vec1,vec2,vec3;
	int secCnt = 8;

	//IplImage * src1 = cvLoadImage("mid_300.bmp", 0);
		log_process("mainStarrrr\n");	
	IplImage * src1 = cvLoadImage("run_back_002.bmp", 0);
	if(src1==NULL)
	{printf("load file erro\n");return -1;
	}  
	//CORDINATE_PAIR cordPair1= getX_cordinateofVerticalBar(src1,14,2,TARGETTYPE_MIDDLE,10,0,true);
	CORDINATE_PAIR cordPair1={0,src1->width};
	if(-1==getShiftPosProcess(src1,vec1,cordPair1,secCnt,false,15))
	{
		log_erro("getshitPosProcess failed\n");
		//return -1;
	};
     cvWaitKey();
	//IplImage * src2 = cvLoadImage("mid_340.bmp", 0);
	//if(src2==NULL){printf("load file erro\n");return -1;}  
	//CORDINATE_PAIR cordPair2= getX_cordinateofVerticalBar(src2,14,2,TARGETTYPE_LEFT,10,0,true);
	//getShiftPosProcess(src2,vec2,cordPair2,secCnt,false,10);

	//IplImage * src3 = cvLoadImage("right_440.bmp", 0);
	//if(src3==NULL){printf("load file erro\n");return -1;}  
	//CORDINATE_PAIR cordPair3= getX_cordinateofVerticalBar(src3,14,2,TARGETTYPE_LEFT,10,0,true);
	//getShiftPosProcess(src3,vec3,cordPair3,secCnt,false,10);

	//IplImage *rlt = cvLoadImage("6955_600_400_1.bmp", 0);
	//if(rlt==NULL){printf("load file erro\n");return -1;}  
    

	return 0;
}
#endif
static void get_arg(char *buf, int buf_size, const char **pp)
{
	const char *p;
	char *q;
	int quote;

	p = *pp;
	while (isspace(*p)) p++;
	q = buf;
	quote = 0;
	if (*p == '\"' || *p == '\'')
		quote = *p++;
	for(;;) {
		if (quote) {
			if (*p == quote)
				break;
		} else {
			if (isspace(*p))
				break;
		}
		if (*p == '\0')
			break;
		if ((q - buf) < buf_size - 1)
			*q++ = *p;
		p++;
	}
	*q = '\0';
	if (quote && *p == quote)
		p++;
	*pp = p;
}
int parse_configfile(const char* filename)
{
	FILE *f;
	char line[1024];
	char cmd[64];
	char arg[1024];
	const char *p;
	int val;

	f = fopen(filename, "r");
	if (!f) {
		perror(filename);
		return -1;
	}

	for(;;) {
		if (fgets(line, sizeof(line), f) == NULL)
			break;
		p = line;
		while (isspace(*p))
			p++;
		if (*p == '\0' || *p == '#')
			continue;

		get_arg(cmd, sizeof(cmd), &p);

		if (!strcmp(cmd, "TraceLevel")) {
			get_arg(arg, sizeof(arg), &p);
			val = atoi(arg);
			if (val < 0 || val > 2) {
				log_erro("Invalid parm TraceLevel: %s\n", arg);
				return -1;
			}
			trace_level = val;
		}
		else if(!strcmp(cmd,"whiteLineWeight"))
		{
			get_arg(arg, sizeof(arg), &p);
			val = atoi(arg);
			if (val<0||val>150)
			{
				log_erro("Invalid parm whiteLineWeight:%s\n",arg);
				return -1;
			}
			whiteLineWeight = val;
		}
		else if(!strcmp(cmd,"log2file"))
		{
			get_arg(arg, sizeof(arg), &p);
			val = atoi(arg);
			if (val<0)
			{
				log_erro("Invalid param log2file:%s\n",arg);
				return -1;
			}
			log2file = val;
		}

	}

	fclose(f);
	return 0;
}
void initParms()
{ 

	parse_configfile("config.txt");


}