// Example 7-5. Template matching
// Yes, I switched the order of image and template from the text.  It's now
// 
// Usage: matchTemplate template image
//
// Puts results of all types of matching methods listed i help() below. 
//        Gary Bradski Oct 3, 2008
//
/* *************** License:**************************
   Oct. 3, 2008
   Right to use this code in any way you want without warrenty, support or any guarentee of it working.

   BOOK: It would be nice if you cited it:
   Learning OpenCV: Computer Vision with the OpenCV Library
     by Gary Bradski and Adrian Kaehler
     Published by O'Reilly Media, October 3, 2008
 
   AVAILABLE AT: 
     http://www.amazon.com/Learning-OpenCV-Computer-Vision-Library/dp/0596516134
     Or: http://oreilly.com/catalog/9780596516130/
     ISBN-10: 0596516134 or: ISBN-13: 978-0596516130    

   OTHER OPENCV SITES:
   * The source code is on sourceforge at:
     http://sourceforge.net/projects/opencvlibrary/
   * The OpenCV wiki page (As of Oct 1, 2008 this is down for changing over servers, but should come back):
     http://opencvlibrary.sourceforge.net/
   * An active user group is at:
     http://tech.groups.yahoo.com/group/OpenCV/
   * The minutes of weekly OpenCV development meetings are at:
     http://pr.willowgarage.com/wiki/OpenCV
   ************************************************** */
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

void help(){
printf("\n"
"Example of using cvMatchTemplate().  The call is:\n"
"\n"
"ch7_ex7_5 template image_to_be_searched\n"
"\n"
"   This routine will search using all methods:\n"
"         CV_TM_SQDIFF        0\n"
"         CV_TM_SQDIFF_NORMED 1\n"
"         CV_TM_CCORR         2\n"
"         CV_TM_CCORR_NORMED  3\n"
"         CV_TM_CCOEFF        4\n"
"         CV_TM_CCOEFF_NORMED 5\n"
"\n"
"The function prototype is:\n"
"   cvMatchTemplate( const CvArr* image, const CvArr* templ,\n"
"                              CvArr* result, int method );\n"
"      image\n"
"         Image to be searched. It should be 8-bit or 32-bit floating-point. \n"
"      templ\n"
"         template which must not larger than the above image and is the same type as the image. \n"
"      result\n"
"         A map of comparison results; single-channel 32-bit floating-point. \n"
"      method\n"
"         See the above methods 0-5 starting with CM_TM_SQDIFF\n"
"         \n"
"	If image is W×H and templ is w×h then result must be W-w+1×H-h+1.		\n"
"\n");
}
#if 1
//getSubImage使用最后需要调用releaseSubImage来释放
//CvPoint* findMatchPoint( IplImage* srcImg, IplImage *temp)
//{
//	IplImage *copyimg=NULL;
//	IplImage *sub_img = cvCreateImageHeader(
//		cvSize(temp->width, temp->height),
//		srcImg->depth,
//		srcImg->nChannels);
//	sub_img->origin = srcImg->origin;
//	sub_img->widthStep = srcImg->widthStep;
//	sub_img->imageData = srcImg->imageData + temp->he.y*srcImg->widthStep +	rect.x;
//	return sub_img;
//}

CvPoint findMatchPoint( IplImage* src, IplImage *templ, int nMethods, int priorityMethod, bool useNormed)
{
	IplImage *ftmp[6]={NULL};
	int method=0;
	int patchx = templ->width;
	int patchy = templ->height;
	int iwidth = src->width - patchx + 1;
	int iheight = src->height - patchy + 1;
	if(nMethods==1)
	{//priority
		ftmp[priorityMethod] = cvCreateImage( cvSize(iwidth,iheight),32,1);		
		if(useNormed)
		{
			ftmp[priorityMethod+1] = cvCreateImage( cvSize(iwidth,iheight),32,1);		
		}

	}
	else if (nMethods==2)
	{//priority+1  method:0,2,4
		int anotherMethod =((priorityMethod + 2) > 4) ?(priorityMethod - 2):(priorityMethod + 2);

		ftmp[priorityMethod] = cvCreateImage( cvSize(iwidth,iheight),32,1);		
		ftmp[anotherMethod] = cvCreateImage( cvSize(iwidth,iheight),32,1);	
		if(useNormed)
		{
			ftmp[priorityMethod+1] = cvCreateImage( cvSize(iwidth,iheight),32,1);
			ftmp[anotherMethod+1] = cvCreateImage( cvSize(iwidth,iheight),32,1);	
		}
	}
	else
	{//use all
		for(int i=0; i<6; ++i){
			if(!useNormed&&(1==i||3==i||5==i))continue;
			ftmp[i] = cvCreateImage( cvSize(iwidth,iheight),32,1);
		}
	}
	
	//DO THE MATCHING OF THE TEMPLATE WITH THE IMAGE
	CvPoint resultPoints[6];
	#ifndef IMPOSSIBLE_CORRDINATE
	#define IMPOSSIBLE_CORRDINATE -1
	#endif
	const CvPoint impossiblePt = cvPoint(IMPOSSIBLE_CORRDINATE,IMPOSSIBLE_CORRDINATE);
	//CvPoint tempPoint[6] = {cvPoint(IMPOSSIBLE_CORRDINATE,IMPOSSIBLE_CORRDINATE)};

	for(int i=0; i<6; ++i){
		resultPoints[i] = impossiblePt;//
		
		if(ftmp[i]==NULL)continue;
		cvMatchTemplate( src, templ, ftmp[i], i); 
		double min,max;
		CvPoint maxPoint,minPoint;
		cvMinMaxLoc(ftmp[i],&min,&max,&minPoint, &maxPoint);
		if (i <= 1){
			resultPoints[i] = minPoint;
		}
		else{
			resultPoints[i] = maxPoint;
		}
		printf("(::%d,%d)\t",resultPoints[i]);
				
		printf("method:%d, find maxpoint(%d,%d), minPoint(%d,%d), min:%.3f,Max:%.3f\n",i,maxPoint.x,maxPoint.y, minPoint.x, minPoint.y, min, max);

	}
	
	//find the most likly point
	
	int markArry[6] = {0};
	for (int i=0;i<6;++i)
	{
		//printf("(%d,%d)\t",resultPoints[i]);
		if (resultPoints[i].x == impossiblePt.x&&resultPoints[i].y == impossiblePt.y)continue;
		else
		{
			if(markArry[i]==0)//0 代表尚未查找过自值
			{
				for (int j=i+1;j<6;j++)
				{
					 if(resultPoints[j].x==resultPoints[i].x&&resultPoints[j].y==resultPoints[i].y)
					 {
						
						 markArry[i] += 1;
						 resultPoints[j] = impossiblePt;
						 
					 }
				  
				}
				markArry[i] +=1;//自己一个也要算上
			}
		}

	}
	int temIter = 0;
	int temp=markArry[0];
	for (int i=0;i<6;i++)
	{
		if(resultPoints[i].x == impossiblePt.x&&resultPoints[i].y == impossiblePt.y)continue;
		if (markArry[i]>temp)
		{
			temp = markArry[i];
			temIter = i;
		}
	
	}
	//printf("pos:%d,cnt:%d\n",temIter,temp);
	return  resultPoints[temIter];
}

// Call is
// matchTemplate template image 
//
// Will Display the results of the matchs
// 
int main( int argc, char** argv ) {

    IplImage *src, *templ,*ftmp[6], *srcImg; //ftmp is what to display on
    int i;
 
		//Read in the template to be used for matching:
		if ((templ = cvLoadImage("../cvtest/temp1.jpg",0))==NULL)printf("load sample erro\n");

		//if ((srcImg = cvLoadImage("../cvtest/rotate degree1.00.jpg",0))==NULL)printf("load sample erro\n");
		if ((src = cvLoadImage("../cvtest/rotate degree1.00.jpg",0))==NULL)printf("load src erro\n");
		
		CvPoint reultPt = findMatchPoint(src,templ,2,4,true);
		printf("result:pt(%d,%d)\n",reultPt.x,reultPt.y);
#if 0
		CvRect rect = cvRect(256,156,89,40);
		//templ = getSubImage(rect,srcImg);

		
		
		IplImage *cloneimage=NULL;
		IplImage *sub_img = cvCreateImageHeader(
			cvSize(rect.width,rect.height),
			srcImg->depth,
			srcImg->nChannels);
		sub_img->origin = srcImg->origin;
		sub_img->widthStep = srcImg->widthStep;
		sub_img->imageData = srcImg->imageData + rect.y*srcImg->widthStep +	rect.x;
		cloneimage = cvCloneImage(sub_img);
		templ = cloneimage;

	templ = sub_img;
	cvNamedWindow( "Template", 0 );
	cvShowImage(   "Template", templ );
	//cvWaitKey(0);
#endif	

  //      //DISPLAY
		//cvNamedWindow( "Template", 0 );
  //      cvShowImage(   "Template", templ );
		//cvWaitKey(0);
  //      cvNamedWindow( "Image", 0 );
  //      cvShowImage(   "Image", src );

		//cvNamedWindow( "SQDIFF", 0 );
  //      cvShowImage(   "SQDIFF", ftmp[0] );

		//cvNamedWindow( "SQDIFF_NORMED", 0 );
  //      cvShowImage(   "SQDIFF_NORMED", ftmp[1] );

		//cvNamedWindow( "CCORR", 0 );
  //      cvShowImage(   "CCORR", ftmp[2] );

		//cvNamedWindow( "CCORR_NORMED", 0 );
  //      cvShowImage(   "CCORR_NORMED", ftmp[3] );

		//cvNamedWindow( "CCOEFF", 0 );
  //      cvShowImage(   "CCOEFF", ftmp[4] );

		//cvNamedWindow( "CCOEFF_NORMED", 0 );
  //      cvShowImage(   "CCOEFF_NORMED", ftmp[5] );


		////LET USER VIEW RESULTS
		//cvWaitKey(0);
	//cvReleaseImageHeader(&sub_img);
	//cvReleaseImage(&templ);
	//cvReleaseImage(&sampleImg);
	//cvReleaseImage(&src);
	getchar();
}

#endif