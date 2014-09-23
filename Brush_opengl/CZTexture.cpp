
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
	if(data != NULL) delete [] data;
}

//////////////////////////////////////////////////////////////////////////

CZTexture::CZTexture(int width_, int height_, TexType texType_ /* = RenderTex */)
{
	width = width_;
	height = height_;

	texType = texType_;

	// 根据纹理作用不同，分别初始化
	switch(texType)
	{
	case RenderTex:
		initRenderTex();
		break;
		
	case BrushTex:
		initBrushTex();
		break;
		
	default:
		std::cerr << "CZTexture::CZTexture - Wrong Textype \n";
	}

	img = NULL;
}

CZTexture::~CZTexture()
{
#if USE_OPENGL
	glDeleteTextures(1, &id);
#endif

	if(img !=NULL) delete img;
}

/// 初始化渲染纹理
void CZTexture::initRenderTex()
{
	glGenTextures (1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
		GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/// 初始化笔刷纹理
void CZTexture::initBrushTex()
{
	glGenTextures (1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0,
		GL_LUMINANCE_ALPHA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}