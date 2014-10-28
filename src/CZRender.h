
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
#include "GL/glut.h"
#include <map>
#include <string>

/// ����CZPath���ջ������ݸ�ʽ
typedef struct 
{
	GLfloat     x, y;
	GLfloat     s, t;
	GLfloat     a;
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

	/// ���ƹ켣����
	void drawPathData(unsigned int n, vertexData *data);
public:
	int width, height;
	CZFbo fbo;
	int context;			///< !ģ��һ��
};


#endif