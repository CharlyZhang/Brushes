
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

CZImage::CZImage(int w_/* =0 */, int h_/* =0 */, StorageMode mode_ /* = DEFAULT_STORAGEMODE */)
{
	width = w_;
	height = h_;
	mode = mode_;

	int n, type = 0;
	switch (mode)
	{
	case RGB_BYTE:		n = 3;	type = 0;	break;
	case RGB_FLOAT:		n = 3;	type = 1;	break;
	case RGBA_BYTE:		n = 4;	type = 0;	break;
	case RGBA_FLOAT:	n = 4;	type = 1;	break;
	default:
		LOG_ERROR("ImageMode is illegal!\n");
		n = 0;
	}

	if(type == 0)
		data = (void*) new unsigned char[n*width*height];
	else if(type ==1)
		data = (void*) new float[n*width*height];
	else
		data = NULL;
}

CZImage::~CZImage()
{
	if(data != NULL)
	{
		delete [] data;
		data = NULL;
	}
}