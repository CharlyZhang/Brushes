
///  \file CZBrush.h
///  \brief This is the file declare the Class CZBrush.
///
///		The class only describe the params of brushes,
///		and the class includes the brush texture, which is different from the original design.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-12
///  \note

#ifndef _CZBRUSH_H_
#define _CZBRUSH_H_

#include "CZProperty.h"
#include "CZTexture.h"
#include "CZStampGenerator.h"
#include <iostream>		// for NULL

class CZBrush
{
public:
	CZBrush(CZStampGenerator *gen_)
	{
		weight.value = 30;//80;
		intensity.value = 0.3;
		angle.value = 0;
		spacing.value = 0.02;
		rotationalScatter.value =0;
		positionalScatter.value =0.5;
		angleDynamics.value = 1;
		weightDynamics.value = 0;
		intensityDynamics.value = 1;

		generator = gen_;
	}
	~CZBrush()
	{
		if (generator) { delete generator; generator = NULL;}
	}

	/// 获取图案纹理
	CZTexture *getTexture(bool isSmallOne = false);

	CZStampGenerator *generator;	///< 笔刷纹理生成器
	CZProperty weight;				///< 画笔的粗细 [1.0, 512.0] -- pixels	
	CZProperty intensity;			///< [0.0, 1.0]

	CZProperty angle;              // [0.0, 1.0];
	CZProperty spacing;            // [0.01, 2.0] -- percentage of brush width
	CZProperty rotationalScatter;  // [0.0, 1.0]
	CZProperty positionalScatter;  // [0.0, 1.0]

	CZProperty angleDynamics;      // [-1.0, 1.0]
	CZProperty weightDynamics;     ///< 目测是画笔滑动时的粗细的缩小值 [-1.0, 1.0]
	CZProperty intensityDynamics;  // [-1.0, 1.0]
};
#endif