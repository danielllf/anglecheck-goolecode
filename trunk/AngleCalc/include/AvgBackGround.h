#ifndef AVG_BACK_GROUND
#define AVG_BACK_GROUND


#include "cv.h"				// define all of the opencv classes etc.
#include "highgui.h"
#include "cxcore.h"
class AvgBackGround{

public:
 AvgBackGround(IplImage *avgback, IplImage *newsample, int thresHold=10);
 ~AvgBackGround();

// Create a binary: 0,255 mask where 255 means forground pixel
// Imask	mask image to be created, 1 channel 8u
void backgroundDiff() ; //Mask should be grayscale

void showImg();
private:
	IplImage *IavgBack,*IhiF, *IlowF, *Imask, *InewSample, *IthresHold;
	int m_thresHold;
};


#endif

