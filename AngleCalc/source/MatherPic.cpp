#include "MatherPic.h"

#ifndef PRINT_TO_FILE
#define PRINT_TO_FILE
#endif



//�����ĺ�����ӡ��ά���鵽ָ���ļ�
void PicPrint2File(unsigned char** data,int row,int column,FILE *pFile,const char* filename)
{
  pFile=fopen(filename,"ab+");
  for(int i=0;i<row;i++)
	  for(int j=0;j<column;j++)
  {
	fprintf(pFile,"%d",data[i][j]);
	if(j==column-1)
		fprintf(pFile,"\r\n");
  }
  fclose(pFile);
}

void MatherPic::PicPrint2File()
{
  m_pOutFile=fopen(m_OutFileName,"ab+");
  for(int i=0;i<m_row;i++)
	  for(int j=0;j<m_column;j++)
  {
	fprintf(m_pOutFile,"%d",m_pData[i][j]);
	if(j==m_column-1)
		fprintf(m_pOutFile,"\r\n");
  }
  fclose(m_pOutFile);
}

void MatherPic::PicPrint2Screen()
{
  for(int i=0;i<m_row;i++)
	  for(int j=0;j<m_column;j++)
  {
	fprintf(stdout,"%d",m_pData[i][j]);
	if(j==m_column-1)
		fprintf(stdout,"\r\n");
  }
}

MatherPic::MatherPic(int row,int column)
{
	
#ifdef PRINT_TO_FILE
	//create file
	m_OutFileName="MatherPic.txt";
	m_pOutFile=fopen(m_OutFileName,"wb");
	fclose(m_pOutFile);
#endif
	/*�����������������һ��ģ�����������һ��ͼ*/

	//�����ڴ棬��ά����
	//alloc ptr array first
	m_pData = new unsigned char *[row];
	memset(m_pData,0,row);
	//then alloc data array
	m_pData[0] = new unsigned char[row*column];
	memset(m_pData[0],0,row*column);

	for(int k=1;k<row;k++)
	{
		m_pData[k] = m_pData[k-1]+column;//����ÿ��ָ����ָ�������
	}


	//fill pic by random data
	for(int i=0;i<row;i++)
		for(int j=0;j<column;j++)
		{
			m_pData[i][j] = j;
		}
	//save row,column
	m_column = column;
	m_row = row;

}
MatherPic::MatherPic(int row,int column,unsigned char *array)
{
	
#ifdef PRINT_TO_FILE
	//create file
	m_OutFileName="MatherPic.txt";
	m_pOutFile=fopen(m_OutFileName,"wb");
	fclose(m_pOutFile);
#endif

	/*�����������������һ��ģ�����������һ��ͼ*/
	
	//�����ڴ棬��ά����
	//alloc ptr array first
	m_pData = new unsigned char *[row];
	memset(m_pData,0,row);
	//then alloc data array
	m_pData[0] = new unsigned char[row*column];
	memset(m_pData[0],0,row*column);
	
	for(int k=1;k<row;k++)
	{
		m_pData[k] = m_pData[k-1]+column;//����ÿ��ָ����ָ�������
	}
	
	
	//fill pic by random data
	for(int i=0;i<row;i++)
		for(int j=0;j<column;j++)
		{
			m_pData[i][j] = array[i*column+j];
		}
		//save row,column
		m_column = column;
		m_row = row;
		
}

MatherPic::~MatherPic()
{
	delete[] m_pData[0];
	delete[] m_pData;
}