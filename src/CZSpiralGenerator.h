
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

	/// ��������
	void buildProperties();
	/// ����ͼ��
	void renderStamp();
private:
	/// ����������
	void drawSpiral(const CZ2DPoint &center_, float radius_);

public:
	CZProperty density;
};

#endif
