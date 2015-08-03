
///  \file CZBristleGenerator.h
///  \brief This is the file declare the Class CZBristleGenerator.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2015-07-24
///  \note

#ifndef _CZBRISTLEGENERATOR_H_
#define _CZBRISTLEGENERATOR_H_

#include "CZStampGenerator.h"

typedef enum BristleGenerateType {
    kPencilBristle,
    kCrayonBristle
} BristleGenerateType;

class CZBristleGenerator: public CZStampGenerator
{
public:
	CZBristleGenerator(CZGLContext *ctx,BristleGenerateType t);
	~CZBristleGenerator(){};

	/// ����һ�ݵ�ǰ������
	CZBristleGenerator* copy();

	/// ����ͼ��
	void renderStamp(CZRandom* randomizer);

	void configureBrush(CZBrush *brush);
    
protected:
    void drawCircle(const CZ2DPoint &center_, float radius_);
    
public:
	CZProperty bristleDensity;
	CZProperty bristleSize;
    BristleGenerateType type;
};

#endif
