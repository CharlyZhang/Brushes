
///  \file CZStampGenerator.h
///  \brief This is the file declare the Class CZStampGenerator.
///
///		This is a virtual class.
///		It is the super class of all that generating the stamp image of brush stamp.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-19
///  \note

#ifndef _CZSTAMPGENERATOR_H_
#define _CZSTAMPGENERATOR_H_

#include "CZGeometry.h"
#include "CZTexture.h"			// for CZImage
#include "CFbo.h"

class CZStampGenerator
{
public:
	CZStampGenerator();
	~CZStampGenerator();

	/// ��������
	void resetSeed();
	///
	void randomize();
	/// ��������
	virtual void buildProperties();
	/// ����ͼ��
	virtual void renderStamp();//:(CGContextRef)ctx randomizer:(WDRandom *)randomizer;
	/// ��ȡ��ˢͼ��
	CFbo *getStamp();
/*
	- (void) configureBrush:(WDBrush *)brush;

	- (CGImageRef) radialFadeWithHardness:(float)hardness;
	- (WDPath *) splatInRect:(CGRect)rect maxDeviation:(float)percentage randomizer:(WDRandom *)randomizer;
	- (CGRect) randomRect:(WDRandom *)randomizer minPercentage:(float)minP maxPercentage:(float)maxP;
	*/

	unsigned int seed;
	CZSize size;
	float baseDimension;
	CZRect baseBounds;
	float scale;
	//@property (nonatomic) UIImage *smallStamp;
	//@property (weak, nonatomic, readonly) UIImage *preview;
	//@property (weak, nonatomic, readonly) UIImage *bigPreview;
	//NSArray *properties;
	//NSMutableDictionary *rawProperties;
	CZRect bounds;
	unsigned char blurRadius;
	bool canRandomize;

protected:
	///< ���ɱ�ˢͼ��
	CFbo* generateStamp();

	CFbo *stamp;				///< ��ˢͼ��
};

#endif