
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

/// 画刷纹理生成器基类
class CZStampGenerator : public CZCoding
{
public:
	CZStampGenerator();
	~CZStampGenerator();

	/// 重置种子
	void resetSeed();
	///
	void randomize(){};
	/// 绘制图案
	virtual void renderStamp();//:(CGContextRef)ctx randomizer:(WDRandom *)randomizer;
	/// 获取笔刷图案
	CZImage *getStamp(bool isSmall = false);
	/// 获取随机化器
	CZRandom *getRandomizer();
	/// 配置笔刷参数
	void configureBrush(CZBrush *brush);
	/// 返回属性值
	std::vector<CZProperty> & getProperties(){ static std::vector<CZProperty> temp; return temp;};
	/// 判断生成器是否相等
	bool isEqual(const CZStampGenerator * gen){return true;}
	/// 实现coding接口
	void update(CZDecoder *decoder_, bool deep = false){};
	void encode(CZCoder *coder_, bool deep = false){};
/*
	- (CGImageRef) radialFadeWithHardness:(float)hardness;
	- (WDPath *) splatInRect:(CGRect)rect maxDeviation:(float)percentage randomizer:(WDRandom *)randomizer;
	- (CGRect) randomRect:(WDRandom *)randomizer minPercentage:(float)minP maxPercentage:(float)maxP;
	*/

protected:
	/// 生成笔刷图案
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
	CZImage *stamp;				///< 笔刷图案
	CZRandom *randomizer;		///< 与seed相关的随机化器
};

class CZGeneratorDelegate
{
public:
	virtual void generatorChanged(CZStampGenerator &gen_) = 0;
};

#endif