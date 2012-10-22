#include "cv.h"
#include "highgui.h"
#include <cxcore.h>
#include <stdio.h>
#include <ctype.h>
#include "utility.h"
#include "pencil.h"

#if 0
int main( int argc, char** argv )
{
    IplImage *src = 0;
    IplImage *histimg = 0;
    CvHistogram *hist = 0;
    
    int hbins = 255;     // 划分HIST的个数，越高越精确
    float hranges_arr[] = {0,255};
    float* hranges = hranges_arr;
    int bin_w;  
    float max_val;
    int i;
    
    if( argc != 2 || (src=cvLoadImage("geZiBu.jpg", CV_LOAD_IMAGE_GRAYSCALE)) == NULL)  // force to gray image
        return -1;
    
    cvNamedWindow( "Histogram", 0 );
    cvNamedWindow( "src", 0);
    
    hist = cvCreateHist( 1, &hbins, CV_HIST_ARRAY, &hranges, 1 );  // 计算直方图
    histimg = cvCreateImage( cvSize(320,200), 8, 3 );
    cvZero( histimg );
    cvCalcHist( &src, hist, 0, 0 ); // 计算直方图

    cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );  // 只找最大值
	printf("max_val:%.4f\n",max_val);
cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 ); // 缩放 bin 到区间 [0,255] 
    cvZero( histimg );
    bin_w = histimg->width / hbins;  // hbins: 条的个数，则 bin_w 为条的宽度
    
    // 画直方图
    for( i = 0; i < hbins; i++ )
    {
        double val = ( cvGetReal1D(hist->bins,i)*histimg->height/255 );
		
        CvScalar color = CV_RGB(255,255,0); //(hsv2rgb(i*180.f/hbins);
        cvRectangle( histimg, cvPoint(i*bin_w,histimg->height),
            cvPoint((i+1)*bin_w,(int)(histimg->height - val)),
            color, 1, 8, 0 );
		printf("(%d,%d),(%d,%d)\n",i*bin_w,histimg->height,(i+1)*bin_w,(int)(histimg->height - val));
    }
    
    cvShowImage( "src", src);
    cvShowImage( "Histogram", histimg );
    cvWaitKey(0);

    cvDestroyWindow("src");
    cvDestroyWindow("Histogram");
    cvReleaseImage( &src );
    cvReleaseImage( &histimg );
    cvReleaseHist ( &hist );
    
    return 0; 
}
#endif

#if 0
int main()
{
#define  MAX_SEC_CNT 100
	int pitch=10;
	IplImage *src = 0;
	if((src = cvLoadImage("lena.jpg",CV_LOAD_IMAGE_GRAYSCALE))==NULL)printf("load erro\n");
	printf("src height:%d,src channel:%d,depth:%d\n",src->height,src->nChannels,src->depth);
	IplImage* src2 = cvCloneImage(src);
    formatImg(src2,pitch);
	IMG_SHOW(src2,src2);


	Section_t sec[MAX_SEC_CNT];
	memset(sec,0,sizeof(Section_t));
	FILE* logfile = fopen("log.txt","w");
	double cmp;
	CvRect rect = cvRect(0,0,src->width/10,3); 
	for (int i=0;i<MAX_SEC_CNT;i++)
	{
		
		cal_one_pen_sect(src,rect,&sec[i]);
		 cmp = cvCompareHist(sec[0].hist,sec[i].hist,CV_COMP_CHISQR);
		fprintf(logfile,"rect.y:%d,mean:%d,sdv:%d,CMP:%.2f\n",rect.y,sec[i].mean,sec[i].std_dev,cmp);
		rect.y+=pitch;
		if(rect.y>src->height)break;
	}

	fclose(logfile);
	
	

		cvWaitKey();
	//getchar();
	return 0;
}
#endif
#if 0
int main()
{
	int pitch=10;
	IplImage *matherImg = 0;
	IplImage* sampleImg =0;
	if ((sampleImg = cvLoadImage("../cvtest/rotate degree1.00.jpg",0))==NULL)printf("load sample erro\n");
	printf("src height:%d,width:%d,src channel:%d,depth:%d\n",sampleImg->height,sampleImg->width,sampleImg->nChannels,sampleImg->depth);

	if((matherImg = cvLoadImage("../cvtest/geZiBu.jpg",0))==NULL)printf("load erro\n");

	printf("src height:%d,width:%d,src channel:%d,depth:%d\n",matherImg->height,sampleImg->width,matherImg->nChannels,matherImg->depth);
	IplImage* lineImg = cvCloneImage(matherImg);
	formatImg(lineImg,pitch);
	IMG_SHOW(lineImg,lineImg);

	
	int hdrWidth = 30;
	int hdrHeight = 11;//3,5,7...
	int gradSlideTol = 2;
	int targetLineNum = 400;
	int gradStep =3;
	 initParms();
	initMatherHeaderSects(matherImg,hdrWidth,hdrHeight,gradStep);

	
	cal_mather_header(matherImg,getMatherSecByIdx(0),hdrWidth,hdrHeight);


	int rst = find_sample_hdr_pos(sampleImg,targetLineNum,hdrWidth,hdrHeight,gradSlideTol);
	printf("targetline:%d,find line:%d\n",targetLineNum,rst);

	llf_printf("the end of print\n");
	formatImg(sampleImg,pitch);
	IMG_SHOW(sampleImg,sampleImg);
	cvWaitKey();
	freeMatherHeadSecs();
	return 0;
}
#endif
