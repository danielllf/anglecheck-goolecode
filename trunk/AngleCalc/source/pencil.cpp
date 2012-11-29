#include "utility.h"
#include "pencil.h"

#define CACL_GRADES 1 //是否计算梯度
//static Section_t *g_pSect=NULL;//指向mather pic section区域首地址
//static Section_t * pMatherBodySect = NULL;//指向mather pen body sect 内存区域
//static Section_t * pSampleBodySect = NULL;//指向sample body sect 内存区域
//static int g_hdrSecLen = 0;//header 区域的长度
//static int g_bodySecLen = 0;//body 每个子区域的长度
//int g_gradsLen = 0;
//int g_gradStep = 0;
//int g_matherSecHdrCnt = 0;// 铅笔在模板中的个数(纵向)
//int g_subSecCnt = 0;//一个铅笔中包含多少子区域
//static CvSize bodySecSize;
FILE* g_logfile=NULL; 
PenParam penParam;


void initParms()
{ 
	g_logfile = fopen("log.txt","w");
	parse_configfile("../cvtest/calangle.conf");

}

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
void  cal_one_pen_sect(IplImage* src,Section_t*  sec)
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


	for (int i=0;i<penParam.matherSecHdrCnt;i++)
	{	
		sec[i].rect = rect;
		sec[i].line_num = rect.y+rect.height/2;

		cal_one_pen_sect(src,&sec[i]);

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
	sample_sec.grads = (int*)malloc(penParam.gradsLen*sizeof(int));
	sample_sec.grads_step = penParam.gradStep;
	sample_sec.grads_len = penParam.gradsLen;
	sample_sec.rect = cvRect(0,target_line_num - hdr_height/2,hdr_width,hdr_height);
	sample_sec.line_num = target_line_num;

	//cal sample hdr
	cal_one_pen_sect(srcSample,&sample_sec);

	//compare sample and stored models of hdr
		Cmp_t tmp,min_cmp;
		for (int i=0;i<penParam.matherSecHdrCnt;i++)
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

	//alloc Sec
	penParam.matherSecHdrCnt = matherImg->height-headHeight;
	penParam.pSect = (Section_t *)malloc(penParam.matherSecHdrCnt*sizeof(Section_t));
	if (!penParam.pSect)
	{
		llf_error("malloc header sect erro\n");
		return;
	}
	memset(penParam.pSect,0,penParam.matherSecHdrCnt*sizeof(Section_t));
	//alloc grades array for each Sec
	penParam.gradsLen = headWidth/gradStep;
	penParam.gradStep = gradStep;

	
	for (int i=0;i<penParam.matherSecHdrCnt;i++)
	{
		//Section_t *tmpSec = &penParam.pSect[i];
		//init sec member
		penParam.pSect[i].grads_len = penParam.gradsLen;// may change when need to wide the grades
		penParam.pSect[i].grads_step = penParam.gradStep;
		penParam.pSect[i].grads = (int *)malloc(penParam.pSect[i].grads_len*sizeof(int));
		if (!penParam.pSect[i].grads)
		{
			llf_error("failed to malloc grades for sec[%d]\n",i);
			return;
		}
		memset(penParam.pSect[i].grads,0,penParam.pSect[i].grads_len*sizeof(int));
	}
	
}

int freeMatherHeadSecs()
{
	fclose(g_logfile);
	for (int i=0;i < penParam.hdrSecLen; i++)
	{
		if(penParam.pSect)
		{
			if (penParam.pSect[i].grads!=NULL)
				free(penParam.pSect->grads);
			else
				llf_error("fail to free grads of hdr sec [%d]\n",i);

			free(penParam.pSect);
			penParam.pSect = NULL;
		}
		else
		{
			llf_error("failed tofreeMatherHeadSec[%d]",i);
			return -1;
		}

	}
	return 0;
}
Section_t* getMatherSecByIdx(int idx)
{
	if (penParam.pSect)
	{
		return &penParam.pSect[idx];
	}
	else
	{
		llf_error("failed to GetMatherSec\n");
		return NULL;
	}

}

//以下匹配尾部
//将铅笔尾部分成多个子区域，分别与模板中对应的子块进行匹配
CvSize getSectSize()
{
	if (penParam.bodySecSize.height!=0&&penParam.bodySecSize.width!=0)
	{
		return penParam.bodySecSize;
	}
	else
		llf_error("penParam.bodySecSize is 0\n");
}
//rect.height must be 奇数.3,5,7....
void initMatherBodySects(IplImage* matherImg,int subSecHeight,int subSecLen,int gradStep)
{
	
	assert(subSecHeight%2!=0);
	//init body sec size
	penParam.bodySecSize.height = subSecHeight;
	penParam.bodySecSize.width = subSecLen;

	int bodySecCnt = matherImg->width/subSecLen;

	penParam.pMatherBodySect =(Section_t*) malloc(bodySecCnt*sizeof(Section_t));
	if (!penParam.pMatherBodySect)
	{
		llf_error("malloc body sect erro\n");
		return;
	}
	memset(penParam.pMatherBodySect,0,bodySecCnt*sizeof(Section_t));
	//alloc grades array for each Sec
	for (int i=0;i<bodySecCnt;i++)
	{
		//init sec member
		penParam.pMatherBodySect[i].grads_len = subSecLen;// may change when need to wide the grades
		penParam.pMatherBodySect[i].grads_step = penParam.gradStep;
		penParam.pMatherBodySect[i].grads = (int *)malloc(penParam.pMatherBodySect[i].grads_len*sizeof(int));
		memset(penParam.pMatherBodySect[i].grads,0,penParam.pMatherBodySect[i].grads_len*sizeof(int));
		if (!penParam.pMatherBodySect[i].grads)
		{
			llf_error("failed to malloc grades for body sec[%d]\n",i);
			return;
		}
	}

}

void initSampleBodySects(IplImage* sampleImg,int subSecHeight,int subSecLen,int gradStep)
{

	assert(subSecHeight%2!=0);
	int bodySecCnt = sampleImg->width/subSecLen;

	penParam.pSampleBodySect =(Section_t*) malloc(bodySecCnt*sizeof(Section_t));
	if (!penParam.pSampleBodySect)
	{
		llf_error("malloc body sect erro\n");
		return;
	}
	memset(penParam.pSampleBodySect,0,bodySecCnt*sizeof(Section_t));
	//alloc grades array for each Sec
	for (int i=0;i<bodySecCnt;i++)
	{
		//init sec member
		penParam.pSampleBodySect[i].grads_len = subSecLen;// may change when need to wide the grades
		penParam.pSampleBodySect[i].grads_step = penParam.gradStep;
		penParam.pSampleBodySect[i].grads = (int *)malloc(penParam.pSampleBodySect[i].grads_len*sizeof(int));
		memset(penParam.pSampleBodySect[i].grads,0,penParam.pSampleBodySect[i].grads_len*sizeof(int));
		if (!penParam.pSampleBodySect[i].grads)
		{
			llf_error("failed to malloc grades for sample body sec[%d]\n",i);
			return;
		}
	}

}
Section_t* getMatherRowSecByIdx(int idx)
{
	if (penParam.pMatherBodySect)
	{
		return &penParam.pMatherBodySect[idx];
	}
	else
		llf_error("failed to GetMatherSec\n");

}
Section_t* getSampleRowSecByIdx(int idx)
{
	if (penParam.pSampleBodySect)
	{
		return &penParam.pSampleBodySect[idx];
	}
	else
		llf_error("failed to GetMatherSec\n");
	return NULL;

}

void fillMotherBodySects(IplImage *img,int targetLine,int sectHeight,int sectLen)
{
	assert(sectHeight%2!=0);
	int secCnt = img->width/sectLen;//舍弃最后一个长度小于sectLen的sec
	int radius = sectHeight/2;
	CvRect rect = cvRect(0,targetLine-radius,sectLen,sectHeight);

	for (int i=0;i<secCnt;i++)
	{
		penParam.pMatherBodySect[i].rect = rect;
		penParam.pMatherBodySect[i].line_num = targetLine;
		cal_one_pen_sect(img,getMatherRowSecByIdx(i));
		rect.x += sectLen;
	}

}
void cmpThePen(IplImage* sampleImg)
{

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
