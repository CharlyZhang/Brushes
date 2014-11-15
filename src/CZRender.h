
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
#include "CZUtil.h"

#if USE_OPENGL
#include "GL/glut.h"
#endif

#include <map>
#include <string>

/// ����CZPath���ջ������ݸ�ʽ
typedef struct 
{
	float     x, y;
	float     s, t;
	float     a;
} vertexData;

class CZRender
{
public:
	enum BlendMode		///< ���ģʽ
	{
		kBlendModeNormal,
		kBlendModeMultiply,
		kBlendModeScreen,
		kBlendModeExclusion
	};

	CZRender();
	~CZRender();

	/// ���õ�ǰ������
	void setContext();
    
    void changeContext(void *_ctx);
    
    void* getContext();

	/// ���ƹ켣����
	void drawPathData(unsigned int n, vertexData *data);
public:
	int width, height;
	CZFbo *fbo;             ///< ! should be as a pointer, for it has to be created after context creation
    
private:
#if USE_OPENGL
	int context;			///< !ģ��һ��
#endif
    struct  Impl;
    Impl *imp;
};


#endif