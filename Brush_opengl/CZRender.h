
///  \file CZRender.h
///  \brief This is the file declare the Class CZRender.
///
///		�����������ƣ�ÿ��render���е�����context,�������ء�
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-10
///  \note

#ifndef _CZRENDER_H_
#define _CZRENDER_H_

#include "CZFbo.h"
#include "CZShader.h"
#include "CZTexture.h"
#include "CZImage.h"
#include "GL/glut.h"
#include <map>
#include <string>



class CZRender
{
public:
	enum DrawType		///< ���Ƶ�����
	{
		//-- Common --
		DrawGeneral,		///< ͨ��
		DrawStamp,			///< ��ˢ����
		kDrawPath,			///< ���켣

		//-- CZPaintingRender-- 
		//	used in CZLayer
		kPaintingErase,				///< �����켣
		kPaintingMask,				///< ��ӹ켣
		kPaintingBlit,				///< չ��ͼ��
		kPaintingBlitTransform,		///< λ�ñ任��չ��ͼ��
		kPaintingBlitColorBalance,	///< ��ɫ������չ��ͼ��
		kPaintingBlitHueSaturation,	///< ���Ͷ�ɫ��������չ��ͼ��
		kPaingtingImageData,		///< ��ȡͼ������
		//	used in CZPainting
		kPaintingStroke,			///< ��һ�ʹ켣
		kPaintingImageForCurrent	///< ���Ƶ�ǰ״̬ͼ��
	};
	enum BlendMode		///< ���ģʽ
	{
		kBlendModeNormal,
		kBlendModeMultiply,
		kBlendModeScreen,
		kBlendModeExclusion
	};

	CZRender()
	{
		/*
		self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

		if (!context || ![EAGLContext setCurrentContext:context]) {
		return nil;
		}
		*/
	};
	~CZRender()
	{
	};

public:
	int width, height;
	CZFbo fbo;
	int context;			///< !ģ��һ��
};


#endif