
///  \file CZBristleGenerator.cpp
///  \brief This is the file implement the Class CZBristleGenerator.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2015-07-24
///  \note

#include "CZBristleGenerator.h"
#include "../CZUtil.h"
#include "../basic/CZ2DPoint.h"
#include "../graphic/CZGLContext.h"
#include "../graphic/glDef.h"
#include <map>
#include <string>

using namespace std;

CZBristleGenerator::CZBristleGenerator(CZGLContext *ctx):CZStampGenerator(ctx)
{
	bristleDensity.title = "Bristle Density";
	bristleDensity.minimumValue = 0.01f;

	bristleSize.title = "Bristle Size";
	bristleSize.minimumValue = 0.01f;

	/// set by CharlyZhang
	bristleDensity.value = 0.02;
	bristleSize.value = 0.7;

}

/// ����һ�ݵ�ǰ������
CZBristleGenerator* CZBristleGenerator::copy()
{
	CZBristleGenerator *ret = new CZBristleGenerator(ptrGLContext);
	ret->seed = seed;
	ret->size = size;
	ret->scale = scale;
	ret->bristleDensity = bristleDensity;
	ret->bristleSize = bristleSize;
	return ret;
}

/// ����ͼ��
void CZBristleGenerator::renderStamp(CZRandom* randomizer)
{
	if (!randomizer)
	{
		LOG_ERROR("randomizer is null\n");
		return;
	}

	if(ptrGLContext == NULL)
	{
		LOG_ERROR("ptrGLContext is NULL!\n");
		return;
	}

	CZShader *shader = getShader("drawCircle");
	if (shader == NULL)
	{
		ptrGLContext->setAsCurrent();
		vector<string> attributes, uniforms;
		attributes.push_back("inPosition");
		uniforms.push_back("mvpMat");
		uniforms.push_back("center");
		uniforms.push_back("radius");
		uniforms.push_back("gray");
		shader = new CZShader("fillCircle","fillCircle",attributes,uniforms);

		shaders.insert(make_pair("drawCircle",shader));
	}

	float  width = baseDimension;
	float  height = baseDimension;
	CZRect  bounds = CZRect(0, 0, width, height);
	CZ2DPoint center = bounds.getCenter();

	int numBristles = bristleDensity.value * 980 + 20;
	int s = bristleSize.value * (size.width * 0.05) + 5;

	width = height = 512;
	float data[] = {0.0,		0.0,
					width,		0.0,
					0.0,		height,
					width,		height};

	GLuint mVertexBufferObject;
	// װ�ض���
	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(float)*2,0);

	shader->begin();

	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"),1,GL_FALSE,projMat);

	// make some bristles
	long maxRadius = (long)width / 2;

	for (int i = 0; i < numBristles; i++) {
		int radius = randomizer->nextInt() % s;

		float tempMax = (maxRadius - (radius + 1));

		float n = randomizer->nextFloat();

		float r = n * tempMax;
		float a = randomizer->nextFloat() * (M_PI * 2);
		float x = center.x + cos(a) * r;
		float y = center.y + sin(a) * r;
	
		glUniform2f(shader->getUniformLocation("center"),x,y);
		glUniform1f(shader->getUniformLocation("radius"),radius/(width/2.0f));
		glUniform1f(shader->getUniformLocation("gray"),randomizer->nextFloat());

		/// ����
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
		
	}

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	/// ����
	glDeleteBuffers(1, &mVertexBufferObject);

	shader->end();
	CZCheckGLError();
}
