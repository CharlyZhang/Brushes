
///  \file CZTexture.h
///  \brief This is the file declare the Class CZTexture.
///
///		This file includes CZTexture, and all are about 2D.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-19
///  \note

#ifndef _CZTEXTURE_H_
#define _CZTEXTURE_H_

#include "Macro.h"
#include "CZImage.h"

#if USE_OPENGL
	#include "GL/glew.h"
#endif

class CZTexture
{
public:
	enum TexType {BrushTex,RenderTex};
	CZTexture(int width_, int height_, TexType texType_ = RenderTex);
	~CZTexture();
	/// 获取其对应的图像数据
	CZImage *getImage();

protected:
	/// 初始化渲染纹理
	void initRenderTex();
	/// 初始化笔刷纹理
	void initBrushTex();
	/// 将纹理转换为相应的图形数据
	CZImage *convert2Image();

public:
#if USE_OPENGL	///< ？是否需要宏，待定
	GLuint id;
#endif

	int width,height;
	TexType texType;
	CZImage *img;
};
#endif