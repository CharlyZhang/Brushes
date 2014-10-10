
///  \file CZTexture.cpp
///  \brief This is the file implement the Class CZImage.
///
///		This file includes CZImage.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-09
///  \note

#include "CZImage.h"
#include "CZTexture.h"
#include <iostream>

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
	if(data != NULL)
	{
		delete [] data;
		data = NULL;
	}
}

/// ×ª»»³ÉÎÆÀí
CZTexture * CZImage::toTexture()
{
	CZTexture * ret = new CZTexture(width,height,CZTexture::RenderTex,data);
	return ret;
}