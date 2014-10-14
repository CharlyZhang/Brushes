
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
#include "CZBezierNode.h"
#include "Macro.h"
//#include "gl/glew.h"
#include <vector>

void CZCheckGLError_(const char *file, int line);
#if CZ_DEBUG
	#define CZCheckGLError()	CZCheckGLError_(__FILE__, __LINE__);
#else
	#define CZCheckGLError()
#endif

#define LOG_STRING 

#define NULL		0
#define M_E        2.71828182845904523536
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
	

/******************************
 * static inline functions
 *****************************/
/// ȡ���ֵ
static inline float Max(float a, float b)
{
	return a>b ? a:b;
}
/// ȡ��Сֵ
static inline float Min(float a, float b)
{
	return a<b ? a:b;
}

/// �淶��
static inline float CZClamp(float min, float max, float value) 
{
	return (value < min) ? min : (value > max) ? max : value;
}

/******************************
 * other functions
 *****************************/
/// ȡ�����[0,1]
float CZRandomFloat();

/// ��ͬ��ɫģʽ��ת��
void HSVtoRGB(float h, float s, float v, float &r, float &g, float &b);
void RGBtoHSV(float r, float g, float b, float &h, float &s, float &v);

/// ��Ӽ�������Ԥ�� -CZBrushPreview������
void addObserver2Preview();

/// ��һ��������ɢ�����ڽ�������α�������������
/// 
///		������㣨nodes���γ�һ�����α��������ߣ��ٽ����ߴ�ɢ�����ɸ����Ƶ㣨points��
/// 
///		/param nodes		- ��������ά���
///		/param closed		- �Ƿ��γɱջ�
///		/param points		- ��ɢ��õ��Ļ��Ƶ�����
///		/return				- ��ɢ��õ��Ļ��Ƶ���Ŀ
unsigned int flattenNodes2Points(const std::vector<CZBezierNode> &nodes, bool closed,std::vector<CZ3DPoint> &points);

/// ���Һ���,��[0,1]��[0,1] -CZFreehandTool������
float sineCurve(float input);

/// �����ã������Ƶ�����
void checkPixels(int w_, int h_);



#endif

