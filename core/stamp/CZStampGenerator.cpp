
///  \file CZStampGenerator.cpp
///  \brief This is the file implement the Class CZStampGenerator.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-19
///  \note

#include "CZStampGenerator.h"
#include "../brush/CZBrush.h"
#include "../graphic/CZFbo.h"
#include "../graphic/CZShader.h"
#include "../graphic/CZTexture.h"
#include "../basic/CZImage.h"
#include "../CZUtil.h"
#include "../basic/CZMat4.h"
#include "../basic/CZ3DPoint.h"
#include "../graphic/CZGLContext.h"
#include "../graphic/glDef.h"
#include <stdlib.h>				// for rand()

#define kSmallStampSize 64
#define kBrushDimension 512		///< 笔刷图案默认大小

using namespace std;

CZStampGenerator::CZStampGenerator(CZGLContext *ctx)
{
	ptrGLContext = ctx;

	seed = rand();

	size = CZSize(kBrushDimension, kBrushDimension);

	scale = 1.0f;

	baseDimension = kBrushDimension;

	stamp = NULL;
	smallStamp = NULL;

	randomizer = NULL;

	ptrDelegate = NULL;
}

CZStampGenerator::~CZStampGenerator()
{
	if(smallStamp != NULL)
	{
		delete smallStamp;
		smallStamp = NULL;
	}

	if(stamp != NULL)
	{
		delete stamp;
		stamp = NULL;
	}

	if(randomizer != NULL)
	{
		delete randomizer;
		randomizer = NULL;
	}
}

CZStampGenerator* CZStampGenerator::copy()
{
	CZStampGenerator *ret = new CZStampGenerator(ptrGLContext);
	ret->seed = seed;
	ret->size = size;
	ret->scale = scale;
	return ret;
}

/// 判断是否相同
bool CZStampGenerator::isEqual(CZStampGenerator *sg)
{
	if(!sg) return false;
	if(typeid(*sg) == typeid(*this))	return true;
	else								return false;
}

/// 绘制图案（虚函数）
void CZStampGenerator::renderStamp(CZRandom* randomizer)
{
	LOG_WARN("Unimplemented method\n");
}

/// 重置种子
void CZStampGenerator::resetSeed()
{
	seed = (unsigned int)rand();
	delete stamp;		stamp = NULL;
	delete smallStamp;	smallStamp = NULL;
	if (ptrDelegate)
	{
		ptrDelegate->generatorChanged(this);
	}
}

/// 随机化
void CZStampGenerator::randomize()
{
	for (map<string,CZProperty*>::iterator itr = rawProperties.begin(); 
		itr != rawProperties.end();	itr++)
	{
		if(itr->second)		itr->second->randomize();
	}
}

/// 获取笔刷图案 ~
CZImage *CZStampGenerator::getStamp(bool isSmall /* = false */)
{
	if (!isSmall || 1) ///! for develop
	{
		if(stamp == NULL) stamp = generateStamp();
		return stamp;
	}

	return NULL;
}

/// 获取随机化器
CZRandom *CZStampGenerator::getRandomizer()
{
	if(randomizer == NULL) randomizer = new CZRandom(seed);
	return randomizer;
}
/// 配置笔刷参数
void CZStampGenerator::configureBrush(CZBrush *brush)
{

	/*
	/// 默认参数值
	brush->intensity.value = 0.2f;
	brush->angle.value = 0;
	brush->spacing.value = 0.02;
	brush->rotationalScatter.value = 0.0f;
	brush->positionalScatter.value = 0.0f;
	brush->angleDynamics.value = 0.0f;
	brush->weightDynamics.value = 0.0f;
	brush->intensityDynamics.value = 0.0f;
	*/

	if(!brush)
	{
		LOG_ERROR("brush is null\n");
		return;
	}

	brush->weight.value = 30;//80;
	brush->intensity.value = 0.3;
	brush->angle.value = 0;
	brush->spacing.value = 0.02;
	brush->rotationalScatter.value =0;
	brush->positionalScatter.value =0.5;
	brush->angleDynamics.value = 1;
	brush->weightDynamics.value = 0;
	brush->intensityDynamics.value = 1;
}

/// 返回属性值
vector<CZProperty>& CZStampGenerator::getProperties()
{
	vector<CZProperty> temp;
	return temp;
}
/// 生成笔刷图案~
CZImage *CZStampGenerator::generateStamp()
{
	if (ptrGLContext == NULL)
	{
		LOG_ERROR("ptrGLContext is null");
		return NULL;
	}

	ptrGLContext->setAsCurrent();

	vector<string> attributes, uniforms;
	attributes.push_back("inPosition");
	uniforms.push_back("mvpMat");
	CZShader shader("stamp","stamp",attributes,uniforms);
	CZFbo fbo;
	fbo.setColorRenderBuffer(size.width,size.height);
	CZCheckGLError();

	CZMat4 projMat;	
	projMat.SetOrtho(0.0f ,size.width, 0.0f, size.height, -1.0f, 1.0f);

	fbo.begin();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);

	shader.begin();

	glUniformMatrix4fv(shader.getUniformLocation("mvpMat"),1,GL_FALSE,projMat);
	renderStamp(getRandomizer());

	shader.end();

	CZImage *ret = fbo.produceImageForCurrentState();

	fbo.end();

	return ret;
}