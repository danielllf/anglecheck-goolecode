#include "utility.h"
#include "pencil.h"

#define CACL_GRADES 1
Section_t *g_pSect=NULL;//指向section区域首地址
static int g_hdrSecLen = 0;//header 区域的长度
int g_gradsLen = 0;
int g_gradStep = 0;
int g_matherSecHdrCnt = 0;// 铅笔在模板中的个数
FILE* g_logfile=NULL; 
//计算水平方向梯度,以每一列为计算单元
//grads为指向梯度数组指针
void cal_horiz_grads(IplImage *src, Section_t * sec)
{
	#if CACL_GRADES

	memset(sec->grads,0,sizeof(unsigned)*sec->grads_len);
	CvRect rect = cvRect(sec->rect.x,sec->rect.y,sec->grads_step,sec->rect.height);
	CvScalar rectSum;

	IplImage *sub_img = cvCreateImageHeader(
		cvSize(rect.width,rect.height),
		src->depth,
		src->nChannels);
	sub_img->origin = src->origin;
	sub_img->widthStep = src->widthStep;

	for (int i=0;i<sec->grads_len;i++)
	{
		//calc sum grads	
		sub_img->imageData = src->imageData + rect.y*src->widthStep +	rect.x;
		rectSum = cvAvg(sub_img);

		RoudToInt(sec->grads[i], rectSum.val[0]);
		rect.x += sec->grads_step;
	}
	
	cvReleaseImageHeader(&sub_img);
	#endif
}
void  cal_one_pen_head(IplImage* src,Section_t*  sec)
{

	IplImage *sub_img = cvCreateImageHeader(
		cvSize(sec->rect.width,sec->rect.height),
		src->depth,
		src->nChannels);
	sub_img->origin = src->origin;
	sub_img->widthStep = src->widthStep;
	sub_img->imageData = src->imageData + sec->rect.y*src->widthStep +sec->rect.x;

	

	//cal hist
	int hsizes = HIST_BINS;
	float hranges[2] = {0,255};
	float *ranges = hranges;
	CvHistogram* hist = cvCreateHist(1,&hsizes,CV_HIST_ARRAY,&ranges);
	
	cvCalcHist(&sub_img,hist);
	cvNormalizeHist(hist,50);
	sec->hist = hist;

	
	//cal mean and sdv
	CvScalar mean =	cvAvg(sub_img);
	sec->mean = mean.val[0];

	//cal grads
	assert(sec->grads);


	cal_horiz_grads(sub_img,sec);
	
	//finally, release the subimg
	cvReleaseImageHeader(&sub_img);


}

void cal_mather_header(IplImage* src,Section_t* sec, int hdr_width,int hdr_height)
{
	CvRect rect = cvRect(0,0,hdr_width,hdr_height);


	for (int i=0;i<g_matherSecHdrCnt;i++)
	{	
		sec[i].rect = rect;
		sec[i].line_num = rect.y+rect.height/2;

		cal_one_pen_head(src,&sec[i]);

		rect.y += 1;
	}

}
Cmp_t cmp_head(Section_t* sample,Section_t* model,int slide_tolerance)
{
	//cal all aspects of the hdr
	Cmp_t cmp_rslt;
	memset(&cmp_rslt,0,sizeof(Cmp_t));
	cmp_rslt.line_num = model->line_num;

	cmp_rslt.meanDiff  = abs(sample->mean - model->mean);


	cmp_rslt.histDiff  = cvCompareHist(sample->hist,model->hist,CV_COMP_CHISQR);
	if(slide_tolerance>=0)
	{
		cmp_rslt.gradDiff = (double)cal_diff_sum_int(sample->grads,model->grads,sample->grads_len,slide_tolerance);
		cmp_rslt.gradDiff /= (sample->grads_step*sample->rect.height);
	}
	cmp_rslt.leicha = cmp_rslt.gradDiff + cmp_rslt.histDiff + cmp_rslt.meanDiff;
	return cmp_rslt;

}



//hdr_height must be 奇数.3,5,7....
int find_sample_hdr_pos(IplImage *srcSample,int target_line_num,int hdr_width,int hdr_height,int grads_slide_tolerance)
{
	assert(srcSample->nChannels == 1);
	assert(hdr_height%2!=0&&hdr_height>=3);

	Section_t sample_sec;
	sample_sec.grads = (int*)malloc(g_gradsLen*sizeof(int));
	sample_sec.grads_step = g_gradStep;
	sample_sec.grads_len = g_gradsLen;
	sample_sec.rect = cvRect(0,target_line_num - hdr_height/2,hdr_width,hdr_height);
	sample_sec.line_num = target_line_num;

	//cal sample hdr
	cal_one_pen_head(srcSample,&sample_sec);

	//compare sample and stored models of hdr
		Cmp_t tmp,min_cmp;
		for (int i=0;i<g_matherSecHdrCnt;i++)
		{
			tmp = cmp_head(&sample_sec,getMatherSecByIdx(i),grads_slide_tolerance);
			llf_printf("line:%d,leicha:%.2f,meanDiff:%.2f,histDiff:%.2f,gradsDiff:%.2f\n",tmp.line_num,tmp.leicha,tmp.meanDiff,tmp.histDiff,tmp.gradDiff);

			if(i==0)min_cmp = tmp;
			if (tmp.leicha < min_cmp.leicha)
			{
				min_cmp = tmp;
			}
		}

	free(sample_sec.grads);

	return min_cmp.line_num;
}

//hdr_height must be 奇数.3,5,7....
void initMatherHeaderSects(IplImage* matherImg,int headWidth,int headHeight,int gradStep)
{
	 g_logfile = fopen("log.txt","w");
	 parse_configfile("../cvtest/calangle.conf");
	//alloc Sec
	g_matherSecHdrCnt = matherImg->height-headHeight;
	g_pSect = (Section_t *)malloc(g_matherSecHdrCnt*sizeof(Section_t));
	if (!g_pSect)
	{
		llf_error("malloc header sect erro\n");
		return;
	}
	memset(g_pSect,0,g_matherSecHdrCnt*sizeof(Section_t));
	//alloc grades array for each Sec
	g_gradsLen = headWidth/gradStep;
	g_gradStep = gradStep;

	
	for (int i=0;i<g_matherSecHdrCnt;i++)
	{
		//Section_t *tmpSec = &g_pSect[i];
		//init sec member
		g_pSect[i].grads_len = g_gradsLen;// may change when need to wide the grades
		g_pSect[i].grads_step = g_gradStep;
		g_pSect[i].grads = (int *)malloc(g_pSect[i].grads_len*sizeof(int));
		if (!g_pSect[i].grads)
		{
			llf_error("failed to malloc grades for sec[%d]\n",i);
			return;
		}
	}
	
}

void freeMatherHeadSecs()
{
	fclose(g_logfile);
	for (int i=0;i < g_hdrSecLen; i++)
	{
		if(g_pSect)
		{
			if (g_pSect[i].grads!=NULL)
				free(g_pSect->grads);
			else
				llf_error("fail to free grads of sec [%d]\n",i);

			free(g_pSect);
			g_pSect = NULL;
		}
		else
			llf_error("failed tofreeMatherHeadSec[%d]",i);
	}
}
Section_t* getMatherSecByIdx(int idx)
{
	if (g_pSect)
	{
		return &g_pSect[idx];
	}
	else
		llf_error("failed to GetMatherSec\n");

}

#if 0
void SingleStep()
{
	IplImage *img = GrabPic();
	CmpHead();
	CmpTail();

}
void ssMain()
{
	InitHeaderModels();
	while(1)
	{
		SingleStep();
	}
}
#endif
