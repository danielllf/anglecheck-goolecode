#include "cv.h"
#include "highgui.h"
#include <cxcore.h>
#include <stdio.h>
#include <ctype.h>
#include "utility.h"
#if 0
int main( )  
{  
	//IplImage * src= cvLoadImage("../cvtest/7newsample.jpg");  
	IplImage* gray_plane =cvLoadImage("../saved1.jpg",CV_LOAD_IMAGE_GRAYSCALE);   //cvCreateImage(cvGetSize(src),8,1);  
	//cvCvtColor(src,gray_plane,CV_BGR2GRAY);  

	int hist_size = 256;    //直方图尺寸  
	int hist_height = 256;  
	float range[] = {0,255};  //灰度级的范围  
	float* ranges=range;  
	 int Div = 5;
	 int wid =1+ gray_plane->width/Div;
	 int left = 0;
	 int thred = 10;
	 //for (int i=0;i<Div;i++)
	 //{
		// cvSetImageROI(gray_plane,cvRect());
	 //}
	//创建一维直方图，统计图像在[0 255]像素的均匀分布  
	CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,&ranges,1);  
	//计算灰度图像的一维直方图  
	cvCalcHist(&gray_plane,gray_hist,0,0);  
	//归一化直方图  
	cvNormalizeHist(gray_hist,1.0);  

	int scale = 1;  
	//创建一张一维直方图的“图”，横坐标为灰度级，纵坐标为像素个数（*scale）  
	IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale,hist_height),8,3);  
	cvZero(hist_image);  
	//统计直方图中的最大直方块  
	float max_value = 0;  
	cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);  

	//分别将每个直方块的值绘制到图中  
	int data[256]={0};
	for(int i=0;i<hist_size;i++)  
	{  
		float bin_val = cvQueryHistValue_1D(gray_hist,i); //像素i的概率  
		int intensity = cvRound(bin_val*hist_height/max_value);  //要绘制的高度  
		cvRectangle(hist_image,  
			cvPoint(i*scale,hist_height-1),  
			cvPoint((i+1)*scale - 1, hist_height - intensity),  
			CV_RGB(255,255,255));    
		if(hist_size<128)
		if(intensity<thred)data[i] = 0;

		printf("i:%d,count:%d\n",i,intensity);
	} 

	for (int i=0;i<gray_plane->height;i++)
	{
		char* pData=gray_plane->imageData+gray_plane->widthStep*i;
		for (int j=0;j<gray_plane->width;j++)
		{
			for(int k=0;k<256;k++)
			{
				if(pData[j]>100)
				{
					pData[j] = 255;
					 
				}
				else
					pData[j]=0;
			}
		
		}
	}
	cvNamedWindow( "GraySource");  
	cvShowImage("GraySource",gray_plane);  
	cvNamedWindow( "H-S Histogram");  
	cvShowImage( "H-S Histogram", hist_image );  

	cvWaitKey(0);  
}  
#endif
#if 0
int main( int argc, char** argv )
{
    IplImage *src = 0;
    IplImage *histimg = 0;
    CvHistogram *hist = 0;
    
    int hbins = 255;     // 划分HIST的个数，越高越精确
	int maxcount = 255;
    float hranges_arr[] = {0,maxcount};//
    float* hranges = hranges_arr;
    int bin_w;  
    float max_val;
    int i;
	int data[255]={-1};
    
    if((src=cvLoadImage("../cvtest/7newsample.jpg", CV_LOAD_IMAGE_GRAYSCALE)) == NULL)  // force to gray image
        return -1;
    
    cvNamedWindow( "Histogram", 0 );
    cvNamedWindow( "src", 0);
    
    hist = cvCreateHist( 1, &hbins, CV_HIST_ARRAY, &hranges, 1 );  // 计算直方图
    histimg = cvCreateImage( cvSize(320,200), 8, 3 );
    cvZero( histimg );
    cvCalcHist( &src, hist, 0, 0 ); // 计算直方图
   float min_val = 0;
    cvGetMinMaxHistValue( hist, &min_val, &max_val, 0, 0 );  // 只找最大值
	printf("max_val:%.4f,min:%.4f\n",max_val,min_val);
	cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 ); // 缩放 bin 到区间 [0,255] 
	int imax = max_val*(max_val ? 255. / max_val : 0.);
	int imin = min_val*(max_val ? 255. / max_val : 0.);
    cvZero( histimg );
    bin_w = histimg->width / hbins;  // hbins: 条的个数，则 bin_w 为条的宽度
    
    // 画直方图
    for( i = 0; i < hbins; i++ )
    {
        int val = cvRound( cvGetReal1D(hist->bins,i)*histimg->height/maxcount );
		
        CvScalar color = CV_RGB(255,255,0);
        cvRectangle( histimg, cvPoint(i*bin_w,histimg->height), cvPoint((i+1)*bin_w, (int)(histimg->height - val)),  color);
		//printf("(%d,%d),(%d,%d)\n",i*bin_w,histimg->height,(i+1)*bin_w,(int)(histimg->height - val));
		data[i] = val;
	/*	if(data[i]==0)
		{
			data[i] = 255;
		}*/

	}
    for (int i=0;i<src->height;i++)
    {
		char* pData=src->imageData+src->widthStep*i;
		for (int j=0;j<src->width;j++)
		{
			if(pData[j]!=123)
			pData[j]=255;
				//.fl+row*mat->step/4;
		}
		
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
	int targetLineNum = 200;
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
