
///  \file CZTexture.cpp
///  \brief This is the file implement the Class CZTexture and other related classes.
///
///		This file includes CZImage, CZTexture.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-19
///  \note

#include "CZTexture.h"
#include <cstdlib>		//for NULL

CZImage::CZImage(int w_/* =0 */, int h_/* =0 */, ImageMode mode_ /* = RGB */)
{
	width = w_;
	height = h_;
	mode = mode_;

	int n;
	switch (mode)
	{
	case RGB:
		n = 3;
		break;
	case RGBA:
		n = 4;
		break;
	default:
		n = 0;
	}
	data  = new float[n*width*height];
}

CZImage::~CZImage()
{
	if(data != NULL) delete [] data;
}

CZTexture::CZTexture()
{
#if USE_OPENGL
	texId  = -1;
#endif

	img = NULL;
}

CZTexture::~CZTexture()
{
#if USE_OPENGL
	if(texId >= 0) glDeleteTextures(1, &texId);
#endif

	if(img !=NULL) delete img;
}