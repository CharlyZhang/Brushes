
///  \file CZRender.h
///  \brief This is the file declare the Class CZRender.
///
///		这里用来绘制，每个render都有单独的context,与绘制相关。
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

/// 定义CZPath最终绘制数据格式
typedef struct 
{
	GLfloat     x, y;
	GLfloat     s, t;
	GLfloat     a;
} vertexData;

class CZRender
{
public:
	enum BlendMode		///< 混合模式
	{
		kBlendModeNormal,
		kBlendModeMultiply,
		kBlendModeScreen,
		kBlendModeExclusion
	};

	CZRender();
	~CZRender();

	/// 设置当前上下文
	void setContext();

	/// 绘制轨迹数据
	void drawPathData(unsigned int n, vertexData *data);
public:
	int width, height;
	CZFbo fbo;
	int context;			///< !模拟一下
};


#endif