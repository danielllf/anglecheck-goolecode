#include "ImgeBase.h"

ImageBase::ImageBase()
{

}
ImageBase::~ImageBase()
{

}

int ImageBase::saveImageToFile(const char* filename)
{
	return cvSaveImage(filename, this->getImage());

}
