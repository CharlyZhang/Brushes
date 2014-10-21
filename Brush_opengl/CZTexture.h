
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
#include <iostream>

#if USE_OPENGL
	#include "GL/glew.h"
#endif

class CZTexture
{
public:
	enum TexType {BrushTex,RenderTex};
	CZTexture(int width_, int height_, TexType texType_ = RenderTex, float *data = NULL);
	~CZTexture();
	/// 由CZImage生成纹理
	static CZTexture* produceFromImage(CZImage *img, bool deepColor = false);
	/// 获取其对应的图像数据
	CZImage *toImage();
	/// 开关线性差值
	void enableLinearInterprolation(float flag);

protected:
	/// 初始化渲染纹理
	void initRenderTex(float *data = NULL);
	/// 初始化笔刷纹理
	void initBrushTex(float *data = NULL);

public:
#if USE_OPENGL	///< ？是否需要宏，待定
	GLuint id;
#endif
	static bool supportColor;		///< 判断硬件是否支持深度颜色
	int width,height;
	TexType texType;
};
#endif