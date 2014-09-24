
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
//#include "gl/glew.h"
#include <vector>

/// �������ջ������ݸ�ʽ
typedef struct 
{
	GLfloat     x, y;
	GLfloat     s, t;
	GLfloat     a;
} vertexData;

/// ȡ���ֵ
float Max(float a, float b);
/// ȡ��Сֵ
float Min(float a, float);


/// ���ƹ켣���ݣ�����ͼ�νӿڣ�-CZPath������
void drawPathData(vertexData *data, unsigned int n, CZShader *shader);
/// ֱ�ӻ��ƹ켣���ݣ���������-CZPath������
void drawPathDataDirectly(std::vector<CZ2DPoint> &points);

/// ��Ӽ�������Ԥ�� -CZBrushPreview������
void addObserver2Preview();
#endif

