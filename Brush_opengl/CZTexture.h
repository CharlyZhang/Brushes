
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
	/// ��ȡ���Ӧ��ͼ������
	CZImage *getImage();

protected:
	/// ��ʼ����Ⱦ����
	void initRenderTex();
	/// ��ʼ����ˢ����
	void initBrushTex();
	/// ������ת��Ϊ��Ӧ��ͼ������
	CZImage *convert2Image();

public:
#if USE_OPENGL	///< ���Ƿ���Ҫ�꣬����
	GLuint id;
#endif

	int width,height;
	TexType texType;
	CZImage *img;
};
#endif