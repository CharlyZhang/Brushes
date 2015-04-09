
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

#include "../basic/CZImage.h"		// for PixDataType definition

class CZTexture
{
public:
	enum TexType {BrushTex,RenderTex};
	CZTexture(int width_, int height_, TexType texType_ = RenderTex, PixDataType *data = 0);
	~CZTexture();
	/// ��CZImage��������
	static CZTexture* produceFromImage(CZImage *img, bool deepColor = false);
	/// ��ȡ���Ӧ��ͼ������
	CZImage *toImage();
	/// �������Բ�ֵ
	void enableLinearInterprolation(float flag);

protected:
	/// ��ʼ����Ⱦ����
	void initRenderTex(PixDataType *data = 0);
	/// ��ʼ����ˢ����
	void initBrushTex(PixDataType *data = 0);

public:
	unsigned int texId;
	static bool supportColor;		///< �ж�Ӳ���Ƿ�֧�������ɫ
	int width,height;
	TexType texType;
};
#endif