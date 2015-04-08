
///  \file CZStampGenerator.h
///  \brief This is the file declare the Class CZStampGenerator and the Interface CZGeneratorDelegate.
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
#include "CZImage.h"			
#include "CZCoding.h"
#include "CZProperty.h"
#include "CZRandom.h"
#include "CZStampRender.h"
#include <vector>

class CZGeneratorDelegate;
class CZBrush;

/// ��ˢ��������������
class CZStampGenerator : public CZCoding
{
public:
	CZStampGenerator();
	~CZStampGenerator();

	/// ��������
	void resetSeed();
	///
	void randomize(){};
	/// ����ͼ��
	virtual void renderStamp();//:(CGContextRef)ctx randomizer:(WDRandom *)randomizer;
	/// ��ȡ��ˢͼ��
	CZImage *getStamp(bool isSmall = false);
	/// ��ȡ�������
	CZRandom *getRandomizer();
	/// ���ñ�ˢ����
	void configureBrush(CZBrush *brush);
	/// ��������ֵ
	std::vector<CZProperty> & getProperties(){ static std::vector<CZProperty> temp; return temp;};
	/// �ж��������Ƿ����
	bool isEqual(const CZStampGenerator * gen){return true;}
	/// ʵ��coding�ӿ�
	void update(CZDecoder *decoder_, bool deep = false){};
	void encode(CZCoder *coder_, bool deep = false){};
/*
	- (CGImageRef) radialFadeWithHardness:(float)hardness;
	- (WDPath *) splatInRect:(CGRect)rect maxDeviation:(float)percentage randomizer:(WDRandom *)randomizer;
	- (CGRect) randomRect:(WDRandom *)randomizer minPercentage:(float)minP maxPercentage:(float)maxP;
	*/

protected:
	/// ���ɱ�ˢͼ��
	CZImage* generateStamp();

public:
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
	CZGeneratorDelegate *ptrDelegate;
	CZRect bounds;
	unsigned char blurRadius;
	bool canRandomize;

protected:
	CZImage *stamp;				///< ��ˢͼ��
	CZRandom *randomizer;		///< ��seed��ص��������
};

class CZGeneratorDelegate
{
public:
	virtual void generatorChanged(CZStampGenerator &gen_) = 0;
};

#endif