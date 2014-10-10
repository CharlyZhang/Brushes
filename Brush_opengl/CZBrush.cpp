
///  \file CZBrush.cpp
///  \brief This is the file implements the Class CZBrush.
///
///		The class only describe the params of brushes.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-8
///  \note

#include "CZBrush.h"
#include "CZActiveState.h"
#include "CZUtil.h"
#include "CZBrushPreview.h"
#include <iostream>

using namespace std;

static char *CZGeneratorKey = "generator";
static char *CZWeightKey = "weight";
static char *CZIntensityKey = "intensity";
static char *CZAngleKey = "angle";
static char *CZSpacingKey = "spacing";
static char *CZRotationalScatterKey = "rotationalScatter";
static char *CZPositionalScatterKey = "positionalScatter";
static char *CZAngleDynamicsKey = "angleDynamics";
static char *CZWeightDynamicsKey = "weightDynamics";
static char *CZIntensityDynamicsKey = "intensityDynamics";
static char *CZUIDKey = "uid";

int CZBrush::brushNum = 0;

CZBrush::CZBrush(CZStampGenerator *gen_)
{
	buildProperties();
	
	uid = 1000 + brushNum ++;
	generator = gen_;
	generator->ptrDelegate = this;
	generator->configureBrush(this);

	proChangeDelegate.clear();
	genChangeDelegate.clear();
	genReplaceDelegate.clear();
	strokePreview = NULL;
	suppressNum = 0;
}
CZBrush::~CZBrush()
{
	if (generator) { delete generator; generator = NULL;}
	if (strokePreview) { delete strokePreview; strokePreview = NULL;}
	changedProperties.clear();
	proChangeDelegate.clear();
	genChangeDelegate.clear();
	genReplaceDelegate.clear();

	brushNum--;
}

/// 随机生成笔刷（静态函数）
CZBrush * randomBrush()
{
	CZStampGenerator *generator= CZActiveState::getInstance()->getRandomGenerator();
	generator->randomize();

	CZBrush* random = new CZBrush(generator);

	random->weight.value = CZRandomFloat() * 56 + 44;
	random->intensity.value = 0.15f;
	random->spacing.value = 0.02;

	return random;
}

/// 获取相应大小的笔刷图
CZImage *CZBrush::previewImageWithSize(const CZSize &size)
{
	if (strokePreview && size.width==strokePreview->width && size.height==strokePreview->height) 
		return strokePreview;

	CZBrushPreview *preview = CZBrushPreview::getInstance();

	preview->setBrush(this);
	strokePreview = preview->previewWithSize(size);

	return strokePreview;
}

/// 是否抑制通知
void CZBrush::suppressNotifications(bool flag)
{
	suppressNum += flag ? 1 : (-1);
}

/// 恢复默认值
void CZBrush::restoreDefaults()
{
	changedProperties.clear();
	
	suppressNotifications(true);
	generator->configureBrush(this);
	suppressNotifications(false);

	if (changedProperties.size() && proChangeDelegate.size()) 
	{
		if (strokePreview) { delete strokePreview; strokePreview = NULL;}	
		int n = proChangeDelegate.size();
		for(int i=0; i<n; i++)
			proChangeDelegate[i]->brushPropertyChanged(changedProperties);
	}

	changedProperties.clear();
}

/// 更改生成器
void CZBrush::setGenerator(CZStampGenerator *gen_)
{
	delete generator;
	generator = gen_;

	generator->ptrDelegate = this;
	generator->configureBrush(this);

	if (strokePreview) { delete strokePreview; strokePreview = NULL;}

	if(genReplaceDelegate.size())
	{
		int n = genReplaceDelegate.size();
		for(int i=0; i<n; i++)
			genReplaceDelegate[i]->brushGeneratorChanged(*gen_);
	}
	else
	{
		cerr << "CZBrush::setGenerator - genReplaceDelegate is empty \n";
	}
}

/// 笔刷属性有多少组（生成器不存在属性时，只有2组属性）
int CZBrush::numberOfPropertyGroups()
{
	if(generator->getProperties().size())
		return 3;
	else 
		return 2;
}
/// 获取某组属性
vector<CZProperty> & CZBrush::propertiesGroupAt(int i)
{
	static vector<CZProperty> ret;

	vector<CZProperty> &temp = generator->getProperties();
	
	if(temp.size() == 0)  i++;

	ret.clear();
	if (i == 0) 
	{
		// shape group
		for(vector<CZProperty>::iterator itr = temp.begin(); itr != temp.end(); itr++)
			ret.push_back(*itr);
	} 
	else if (i == 1) 
	{
		// spacing group
		ret.push_back(intensity);
		ret.push_back(angle);
		ret.push_back(spacing);
		ret.push_back(rotationalScatter);
		ret.push_back(positionalScatter);
	} 
	else if (i == 2) 
	{
		// dynamic group
		ret.push_back(angleDynamics);
		ret.push_back(weightDynamics);
		ret.push_back(intensityDynamics);
	}
	
	return ret;
}

/// 处理属性变化（实现属性委托接口）
void CZBrush::propertyChanged(CZProperty &property_)
{
	if (suppressNum == 0 && proChangeDelegate.size())
	{
		if (strokePreview) { delete strokePreview; strokePreview = NULL;}

		changedProperties.clear();
		changedProperties.push_back(property_);
		
		int n = proChangeDelegate.size();
		for(int i=0; i<n; i++)
			proChangeDelegate[i]->brushPropertyChanged(changedProperties);
	} 
	else 
	{
		changedProperties.push_back(property_);	///< 发生在restoreProperty中gen配置画刷的过程中
	}
}

/// 处理生成器变化（实现生成器委托接口）
void CZBrush::generatorChanged(CZStampGenerator &gen_)
{
	if (strokePreview) { delete strokePreview; strokePreview = NULL;}

	if (genChangeDelegate.size())
	{
		int n = genChangeDelegate.size();
		for(int i=0; i<n; i++)
			genChangeDelegate[i]->brushGeneratorChanged(gen_);
	} 
	else 
	{
		cerr << "CZBrush::generatorChanged - genChangeDelegate is empty \n";
	}
}

/// 实现coding接口
void CZBrush::update(CZDecoder *decoder, bool deep /*= false*/)
{
	if (deep) 
	{
		if(generator) delete generator;
		generator = (CZStampGenerator *) decoder->decodeObject(CZGeneratorKey);
		generator->ptrDelegate = this;
		buildProperties();
	}

	weight.value	= decodeValue(CZWeightKey, decoder, weight.value);
	intensity.value = decodeValue(CZIntensityKey, decoder, intensity.value);
	angle.value		= decodeValue(CZAngleKey, decoder, angle.value);
	spacing.value	= decodeValue(CZSpacingKey, decoder, spacing.value);
	rotationalScatter.value = decodeValue(CZRotationalScatterKey, decoder, rotationalScatter.value);
	positionalScatter.value = decodeValue(CZPositionalScatterKey, decoder, positionalScatter.value);
	angleDynamics.value		= decodeValue(CZAngleDynamicsKey, decoder, angleDynamics.value);
	weightDynamics.value	= decodeValue(CZWeightDynamicsKey, decoder, weightDynamics.value);
	intensityDynamics.value = decodeValue(CZIntensityDynamicsKey, decoder, intensityDynamics.value);
	uid = decoder->decodeUint(CZUIDKey);
}
void CZBrush::encode(CZCoder *coder, bool deep/* = false*/)
{
	if (deep) 
	{
		coder->encodeObject(generator, CZGeneratorKey, deep);
	}

	coder->encodeUint(uid,CZUIDKey);
	coder->encodeFloat(weight.value, CZWeightKey);
	coder->encodeFloat(intensity.value, CZIntensityKey);
	coder->encodeFloat(angle.value, CZAngleKey);
	coder->encodeFloat(spacing.value, CZSpacingKey);
	coder->encodeFloat(rotationalScatter.value, CZRotationalScatterKey);
	coder->encodeFloat(positionalScatter.value, CZPositionalScatterKey);
	coder->encodeFloat(angleDynamics.value, CZAngleDynamicsKey);
	coder->encodeFloat(weightDynamics.value, CZWeightDynamicsKey);
	coder->encodeFloat(intensityDynamics.value, CZIntensityDynamicsKey);
}

/// 添加三种事件的委托
void CZBrush::addProChangeDelegate(CZBrushDelegate* ptr)
{
	proChangeDelegate.push_back(ptr);
}
void CZBrush::addGenChangeDelegate(CZBrushDelegate* ptr)
{
	genChangeDelegate.push_back(ptr);
}
void CZBrush::addGenReplaceDelegate(CZBrushDelegate* ptr)
{
	genReplaceDelegate.push_back(ptr);
}

/// 创建属性(不包括值)
void CZBrush::buildProperties()
{
	weight.title = "Weight";
	weight.conversionFactor = 1;
	weight.minimumValue = 1;
	weight.maximumValue = 512;
	weight.ptrDelegate = this;

	intensity.title = "Intensity";
	intensity.ptrDelegate = this;

	angle.title = "Angle";
	angle.maximumValue = 360;
	angle.conversionFactor = 1;
	angle.ptrDelegate = this;

	spacing.title = "Spacing";
	spacing.minimumValue = 0.004f;
	spacing.maximumValue = 2.0f;
	spacing.percentage = true;
	spacing.ptrDelegate = this;

	rotationalScatter.title = "Jitter";
	rotationalScatter.ptrDelegate = this;
	positionalScatter.title = "Scatter";
	positionalScatter.ptrDelegate = this;

	angleDynamics.title = "Dynamic Angle";
	angleDynamics.minimumValue = -1.0f;
	angleDynamics.ptrDelegate = this;

	weightDynamics.title = "Dynamic Weight";
	weightDynamics.minimumValue = -1.0f;
	weightDynamics.ptrDelegate = this;

	intensityDynamics.title = "Dynamic Intensity";
	intensityDynamics.minimumValue = -1.0f;
	intensityDynamics.ptrDelegate = this;
}

/// 解压缩值
float CZBrush::decodeValue(const char *key, CZDecoder *decoder, float deft)
{
	float value = decoder->decodeFloat(key,123456789);
	if (value == 123456789) 
	{
		// for legacy files
		CZProperty *old = (CZProperty *) decoder->decodeObject(strcmp(key,CZWeightKey)==0 ? "noise": key);
		return old ? old->value : deft;
	} 
	else 
	{
		return value;
	}
}
