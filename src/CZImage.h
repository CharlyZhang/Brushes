
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

#include "Macro.h"

#if USE_OPENGL
	#define GL_PIXEL_TYPE GL_FLOAT
	typedef float PixDataType;
#elif USE_OPENGL_ES
	#define GL_PIXEL_TYPE GL_UNSIGNED_BYTE
	typedef unsigned char PixDataType;
#endif

class CZImage
{
public:
	enum ImageMode {RGB, RGBA};			///< 声明枚举类型
	CZImage(int w_=0, int h_=0, ImageMode mode_ = RGBA);
	~CZImage();

	int width,height;

    PixDataType *data;
    
	ImageMode mode;
};

#endif