
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
#include "../basic/CZImage.h"
#include "../CZUtil.h"
#include "../CZDefine.h"
#include "glDef.h"

bool CZTexture::supportColor = CZUtil::canUseHDTexture();	///

CZTexture::CZTexture(int width_, int height_, TexType texType_ /* = RenderTex */, PixDataType *data /* = NULL */)
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
		LOG_ERROR("Wrong Textype \n");
	}
}

CZTexture::~CZTexture()
{
	glDeleteTextures(1, &texId);
}

CZTexture* CZTexture::produceFromImage(CZImage *img, bool deepColor /*= false*/)
{
	if(img == NULL)
	{
		LOG_ERROR("CZTexture::produceFromImage - img is NULL\n");
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
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flag ? GL_LINEAR : GL_NEAREST);
}

/// ��ʼ����Ⱦ����
void CZTexture::initRenderTex(PixDataType *data /* = NULL */)
{
	glGenTextures (1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

#if USE_OPENGL
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);  // �����Զ����ɵ�������
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
		GL_RGBA, GL_FLOAT,(void*)data);
	glBindTexture(GL_TEXTURE_2D, 0);
#elif USE_OPENGL_ES
	/// painting �����������
	// Set up filter and wrap modes for this texture object
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	CZCheckGLError();
	GLenum      type = supportColor ? GL_HALF_FLOAT_OES : GL_UNSIGNED_BYTE;
	//NSUInteger  bytesPerPixel = supportColor ? 8 : 4;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)data);
	CZCheckGLError();

	if(width == height) /// in fact, both width and height should be power of two
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); ///! originally GL_LINEAR
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	CZCheckGLError();
	glBindTexture(GL_TEXTURE_2D, 0);
#endif
	CZCheckGLError();
}

/// ��ʼ����ˢ������Ŀǰ��Ȼδ��ʵ��FBOֱ����Ⱦ�Ҷ�ͼ��
void CZTexture::initBrushTex(PixDataType *data /* = NULL */)
{
	glGenTextures (1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
#if USE_OPENGL
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA32F_ARB, width, height, 0,
		GL_LUMINANCE_ALPHA, GL_FLOAT, (void*)data);
#endif
	glBindTexture(GL_TEXTURE_2D, 0);
}

