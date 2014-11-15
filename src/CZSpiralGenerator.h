
///  \file CZSpiralGenerator.h
///  \brief This is the file declare the Class CZSpiralGenerator.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-19
///  \note

#ifndef _CZSPIRALGENERATOR_H_
#define _CZSPIRALGENERATOR_H_

#include "CZStampGenerator.h"
#include "CZProperty.h"

class CZSpiralGenerator: public CZStampGenerator
{
public:
	CZSpiralGenerator();
	~CZSpiralGenerator(){};

	/// 创建属性
	void buildProperties();
	/// 绘制图案
	void renderStamp();
private:
	/// 绘制螺旋线
	void drawSpiral(const CZ2DPoint &center_, float radius_);

public:
	CZProperty density;
};

#endif
