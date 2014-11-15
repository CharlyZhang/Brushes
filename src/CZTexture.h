
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
#include "CZCommon.h"
#include "CZImage.h"
#include <iostream>

#if USE_OPENGL
	#include "GL/glew.h"
#endif

class CZTexture
{
public:
	enum TexType {BrushTex,RenderTex};
	CZTexture(int width_, int height_, TexType texType_ = RenderTex, PixDataType *data = NULL);
	~CZTexture();
	/// ��CZImage��������
	static CZTexture* produceFromImage(CZImage *img, bool deepColor = false);
	/// ��ȡ���Ӧ��ͼ������
	CZImage *toImage();
	/// �������Բ�ֵ
	void enableLinearInterprolation(float flag);

protected:
	/// ��ʼ����Ⱦ����
	void initRenderTex(PixDataType *data = NULL);
	/// ��ʼ����ˢ����
	void initBrushTex(PixDataType *data = NULL);

public:
	unsigned int texId;
	static bool supportColor;		///< �ж�Ӳ���Ƿ�֧�������ɫ
	int width,height;
	TexType texType;
};
#endif