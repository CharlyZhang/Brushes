
///  \file CZBrush.h
///  \brief This is the file declare the Class CZBrush and the Interface CZBrushDelegate.
///
///		The class only describe the params of brushes.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-12
///  \note		uid处理、深度拷贝、相等比较等没处理

#ifndef _CZBRUSH_H_
#define _CZBRUSH_H_

#include "basic/CZProperty.h"
#include "stamp/CZStampGenerator.h"		// for CZGeneratorDelegate
#include "serialization/CZCoding.h"
#include <vector>
#include <string>

extern std::string CZBrushPropertyChanged;
extern std::string CZBrushGeneratorChanged;
extern std::string CZBrushGeneratorReplaced;

class CZImage;

class CZBrush :public CZCoding, public CZPropertyDelegate, public CZGeneratorDelegate
{
public:
	CZBrush(CZStampGenerator *gen_);
	~CZBrush();

	/// 随机生成笔刷（静态函数）
	static CZBrush * randomBrush(); 
	/// 获取相应大小的笔刷图
	CZImage *previewImageWithSize(const CZSize &size);
	/// 是否抑制通知
	void suppressNotifications(bool flag);
	/// 恢复默认值
	void restoreDefaults();
	/// 更改生成器
	void setGenerator(CZStampGenerator *gen_);
	/// 笔刷属性有多少组（生成器不存在属性时，只有2组属性）
	int numberOfPropertyGroups();
	/// 获取某组属性
	std::vector<CZProperty> & propertiesGroupAt(int i); 
	/// 处理属性变化（实现属性委托接口）
	void propertyChanged(CZProperty *property_);
	/// 处理生成器变化（实现生成器委托接口）
	void generatorChanged(CZStampGenerator *gen_);
	/// 实现coding接口
	void update(CZDecoder *decoder, bool deep = false);
	void encode(CZCoder *coder, bool deep = false);

private:
	/// 创建属性（不包括值）
	void buildProperties();
	/// 解压缩值
	float decodeValue(const char *key, CZDecoder *decoder, float deft);

public:
	unsigned int uid;				///< ！原设计中，uid随属性和生成器改变而改变
	CZStampGenerator *generator;	///< 笔刷纹理生成器，该类负责销毁
	CZProperty weight;				///< 画笔的粗细 [1.0, 512.0] -- pixels	

	CZProperty intensity;			///< [0.0, 1.0]	--笔刷浓度（纹理的数目）
	CZProperty angle;				// [0.0, 1.0];
	CZProperty spacing;				/// [0.01, 2.0] --笔刷的密度，percentage of brush width
	CZProperty rotationalScatter;  // [0.0, 1.0]
	CZProperty positionalScatter;  // [0.0, 1.0]

	CZProperty angleDynamics;      // [-1.0, 1.0]
	CZProperty weightDynamics;     ///< 目测是画笔滑动时的粗细的缩小值 [-1.0, 1.0]
	CZProperty intensityDynamics;  // [-1.0, 1.0]

private:
	int suppressNum;								///< 抑制通知的数目
	std::vector<CZProperty> changedProperties;		///< 改变了的属性（用vector代替了set，避免写CZProperty的比较函数）

	static int brushNum;							///< 记录笔刷数目
	CZImage *strokePreview;							///< 笔刷的预览图
};

#endif