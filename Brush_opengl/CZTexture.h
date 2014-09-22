
///  \file CZTexture.h
///  \brief This is the file declare the Class CZTexture and other related classes.
///
///		This file includes CZImage, CZTexture.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-19
///  \note

#ifndef _CZTEXTURE_H_
#define _CZTEXTURE_H_

#include "Macro.h"

#if USE_OPENGL
	#include "GL/glew.h"
#endif

/// CZImage class
class CZImage
{
public:
	enum ImageMode{RGB, RGBA};			///< 声明枚举类型
	CZImage(int w_=0, int h_=0, ImageMode mode_ = RGB);
	~CZImage();

	int width,height;
	float *data;
	ImageMode mode;
};

/// CZTexture class
class CZTexture
{
public:
	CZTexture();
	~CZTexture();


#if USE_OPENGL
	GLuint texId;
#endif
	CZImage *img;
};
#endif