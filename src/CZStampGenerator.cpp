
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
#include <stdlib.h>				// for rand()
#include "CZBrush.h"
#include <iostream>

#define kSmallStampSize 64
#define kBrushDimension 512		///< 笔刷图案默认大小

CZStampGenerator::CZStampGenerator()
{
	seed = rand();

	size = CZSize(kBrushDimension, kBrushDimension);

	//rawProperties = [NSMutableDictionary dictionary];
	scale = 1.0f;

	baseDimension = kBrushDimension;

	stamp = NULL;

	randomizer = NULL;

	ptrDelegate = NULL;
}

CZStampGenerator::~CZStampGenerator()
{
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

/// 创建属性（虚函数）
void CZStampGenerator::buildProperties()
{
	std::cout<<" CZStampGenerator::buildProperties - Unimplemented method \n";
}

/// 绘制图案（虚函数）
void CZStampGenerator::renderStamp()
{
	std::cout<<" CZStampGenerator::renderStamp - Unimplemented method \n";
}

/// 获取笔刷图案 ~
CZImage *CZStampGenerator::getStamp(bool isSmall /* = false */)
{
	if(stamp == NULL) stamp = generateStamp();
	return stamp;
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

/// 生成笔刷图案~
CZImage *CZStampGenerator::generateStamp()
{
	CZStampRender *ptrRender = CZStampRender::getInstance();

	ptrRender->configure(size.width, size.height);
	ptrRender->ptrGenerator = this;
	CZImage *ret = ptrRender->drawStamp();

	return ret;
}