
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
#define kBrushDimension 512		///< ��ˢͼ��Ĭ�ϴ�С

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

/// �������ԣ��麯����
void CZStampGenerator::buildProperties()
{
	std::cout<<" CZStampGenerator::buildProperties - Unimplemented method \n";
}

/// ����ͼ�����麯����
void CZStampGenerator::renderStamp()
{
	std::cout<<" CZStampGenerator::renderStamp - Unimplemented method \n";
}

/// ��ȡ��ˢͼ�� ~
CZImage *CZStampGenerator::getStamp(bool isSmall /* = false */)
{
	if(stamp == NULL) stamp = generateStamp();
	return stamp;
}

/// ��ȡ�������
CZRandom *CZStampGenerator::getRandomizer()
{
	if(randomizer == NULL) randomizer = new CZRandom(seed);
	return randomizer;
}
/// ���ñ�ˢ����
void CZStampGenerator::configureBrush(CZBrush *brush)
{

	/*
	/// Ĭ�ϲ���ֵ
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

/// ���ɱ�ˢͼ��~
CZImage *CZStampGenerator::generateStamp()
{
	CZStampRender *ptrRender = CZStampRender::getInstance();

	ptrRender->configure(size.width, size.height);
	ptrRender->ptrGenerator = this;
	CZImage *ret = ptrRender->drawStamp();

	return ret;
}