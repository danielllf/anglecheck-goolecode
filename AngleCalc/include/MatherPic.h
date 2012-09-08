
#ifndef _MATHER_PIC_H
#define _MATHER_PIC_H

#include "stdio.h"
#include "string.h"

class MatherPic
{
public:
	MatherPic(int row,int column);
	MatherPic(int row,int column,unsigned char *array);
	void PicPrint2File();
	void PicPrint2Screen();

	~MatherPic();
public:
	unsigned char **m_pData;
private:

	int m_column;
	int m_row;
	FILE* m_pOutFile;
	char *m_OutFileName;
};

#endif