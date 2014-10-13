
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

#define LOG

#define NULL		0
#define M_E        2.71828182845904523536
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
	
/// 定义最终绘制数据格式
typedef struct 
{
	GLfloat     x, y;
	GLfloat     s, t;
	GLfloat     a;
} vertexData;


/******************************
 * static inline functions
 *****************************/
/// 取最大值
static inline float Max(float a, float b)
{
	return a>b ? a:b;
}
/// 取最小值
static inline float Min(float a, float b)
{
	return a<b ? a:b;
}

/// 规范化
static inline float CZClamp(float min, float max, float value) 
{
	return (value < min) ? min : (value > max) ? max : value;
}

/******************************
 * other functions
 *****************************/
/// 取随机数[0,1]
float CZRandomFloat();

/// 不同颜色模式的转换
void HSVtoRGB(float h, float s, float v, float &r, float &g, float &b);
void RGBtoHSV(float r, float g, float b, float &h, float &s, float &v);

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

