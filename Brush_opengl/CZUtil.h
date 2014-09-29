
///  \file CZUtil.h
///  \brief This is the file declare a series of utilities.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-11
///  \note

#ifndef _CZUTIL_H_
#define _CZUTIL_H_

#include "CZGeometry.h"
#include "CZShader.h"
#include "Macro.h"
//#include "gl/glew.h"
#include <vector>

void CZCheckGLError_(const char *file, int line);
#if CZ_DEBUG
	#define CZCheckGLError()	CZCheckGLError_(__FILE__, __LINE__);
#else
	#define CZCheckGLError()
#endif

	
/// 定义最终绘制数据格式
typedef struct 
{
	GLfloat     x, y;
	GLfloat     s, t;
	GLfloat     a;
} vertexData;

/// 取最大值
float Max(float a, float b);
/// 取最小值
float Min(float a, float);


/// 绘制轨迹数据（利用图形接口）-CZPath类引用
void drawPathData(vertexData *data, unsigned int n, CZShader *shader);
/// 直接绘制轨迹数据（不带纹理）-CZPath类引用
void drawPathDataDirectly(::std::vector<CZ2DPoint> &points);

/// 添加监听器到预览 -CZBrushPreview类引用
void addObserver2Preview();

/// 正弦函数,由[0,1]到[0,1] -CZFreehandTool类引用
float sineCurve(float input);

/// 调试用，检测绘制的像素
void checkPixels(int w_, int h_);



#endif

