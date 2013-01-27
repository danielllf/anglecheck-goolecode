#ifndef IMAGE_BASE
#define IMAGE_BASE
#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <highgui.h>

class ImageBase
{
	
public:
	ImageBase();
	~ImageBase();
	virtual IplImage *getImage()=0;
public:

	int saveImageToFile(const char* filename);

};
#endif