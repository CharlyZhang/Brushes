
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
#include "CZUtil.h"
#include <iostream>

using namespace std;

bool CZTexture::supportColor = CZcanUseHDTexture();

CZTexture::CZTexture(int width_, int height_, TexType texType_ /* = RenderTex */, float *data /* = NULL */)
{
	width = width_;
	height = height_;

	texType = texType_;

	// �����������ò�ͬ���ֱ��ʼ��
	switch(texType)
	{
	case RenderTex:
		initRenderTex(data);
		break;
		
	case BrushTex:
		initBrushTex(data);
		break;
		
	default:
		std::cerr << "CZTexture::CZTexture - Wrong Textype \n";
	}
}

CZTexture::~CZTexture()
{
#if USE_OPENGL
	glDeleteTextures(1, &id);
#endif
}

CZTexture* CZTexture::produceFromImage(CZImage *img, bool deepColor /*= false*/)
{
	if(img == NULL)
	{
		cerr << "CZTexture::produceFromImage - img is NULL\n";
		return NULL;
	}

	/// ���Ҫ�������ɫ��������Ӳ���Ƿ�֧�־���
	if(deepColor) deepColor = supportColor;

	CZTexture * ret = new CZTexture(img->width,img->height,RenderTex,img->data);
	return ret;
}

/// ��ȡ���Ӧ��ͼ������
CZImage *CZTexture::toImage()
{
	return NULL;
}

/// �������Բ�ֵ
void CZTexture::enableLinearInterprolation(float flag)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flag ? GL_LINEAR : GL_NEAREST);
}

/// ��ʼ����Ⱦ����
void CZTexture::initRenderTex(float *data /* = NULL */)
{
	glGenTextures (1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);  // �����Զ����ɵ�������
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
		GL_RGBA, GL_FLOAT,(void*)data);
	glBindTexture(GL_TEXTURE_2D, 0);

#if 0 /// painting �����������
	// Set up filter and wrap modes for this texture object
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLenum      type = deepColor ? GL_HALF_FLOAT_OES : GL_UNSIGNED_BYTE;
	NSUInteger  bytesPerPixel = deepColor ? 8 : 4;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, type, pixels);

	WDCheckGLError();
#endif

}

/// ��ʼ����ˢ������Ŀǰ��Ȼδ��ʵ��FBOֱ����Ⱦ�Ҷ�ͼ��
void CZTexture::initBrushTex(float *data /* = NULL */)
{
	glGenTextures (1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA32F_ARB, width, height, 0,
		GL_LUMINANCE_ALPHA, GL_FLOAT, (void*)data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

