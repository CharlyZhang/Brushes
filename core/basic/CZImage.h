
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


class CZImage
{
public:
	enum ImageMode {RGB, RGBA};			///< ����ö������
	CZImage(int w_=0, int h_=0, ImageMode mode_ = RGBA);
	~CZImage();

	/// ��ȿ�������
	CZImage* copy();

	int width,height;

    PixDataType *data;
    
	ImageMode mode;
};

#endif