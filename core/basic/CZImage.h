
///  \file CZImage.h
///  \brief This is the file declare the Class CZImage.
///
///		This file includes CZImage
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-09
///  \note

#ifndef _CZIMAGE_H_
#define _CZIMAGE_H_

#include "../CZDefine.h"

typedef enum StorageMode 
{
	NOT_SET,RGB_BYTE, RGBA_BYTE, RGB_FLOAT, RGBA_FLOAT
} StorageMode;			///< ����ͼ��洢ģʽ

class CZImage
{
public:
	CZImage(int w_=0, int h_=0, StorageMode mode_ = DEFAULT_STORAGEMODE, void* data_ = NULL);
	~CZImage();

	StorageMode getMode() { return mode;}

	/// ��ȿ�������
	CZImage* copy();

	int			width,height;
	void		*data;		///< ����������mode����

private:
	StorageMode	mode;
};

#endif