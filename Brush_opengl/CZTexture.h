
///  \file CZTexture.h
///  \brief This is the file declare the Class CZTexture and other related classes.
///
///		This file includes CZImage, CZTexture, and all are about 2D.
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
	enum ImageMode {RGB, RGBA};			///< ����ö������
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
	enum TexType {BrushTex,RenderTex};
	CZTexture(int width_, int height_, TexType texType_ = RenderTex);
	~CZTexture();

protected:
	/// ��ʼ����Ⱦ����
	void initRenderTex();
	/// ��ʼ����ˢ����
	void initBrushTex();

public:
#if USE_OPENGL	///< ���Ƿ���Ҫ�꣬����
	GLuint id;
#endif

	int width,height;
	TexType texType;
	CZImage *img;
};
#endif