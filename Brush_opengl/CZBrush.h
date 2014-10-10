
///  \file CZBrush.h
///  \brief This is the file declare the Class CZBrush and the Interface CZBrushDelegate.
///
///		The class only describe the params of brushes.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-12
///  \note		uid������ȿ�������ȱȽϵ�û����

#ifndef _CZBRUSH_H_
#define _CZBRUSH_H_

#include "CZProperty.h"
#include "CZTexture.h"
#include "CZImage.h"
#include "CZStampGenerator.h"
#include "CZCoding.h"
#include <iostream>		// for NULL
#include <vector>

class CZBrushDelegate;
class CZStampGenerator;

class CZBrush :public CZCoding, public CZPropertyDelegate, public CZGeneratorDelegate
{
public:
	CZBrush(CZStampGenerator *gen_);
	~CZBrush();

	/// ������ɱ�ˢ����̬������
	static CZBrush * randomBrush(); 
	/// ��ȡ��Ӧ��С�ı�ˢͼ
	CZImage *previewImageWithSize(const CZSize &size);
	/// �Ƿ�����֪ͨ
	void suppressNotifications(bool flag);
	/// �ָ�Ĭ��ֵ
	void restoreDefaults();
	/// ����������
	void setGenerator(CZStampGenerator *gen_);
	/// ��ˢ�����ж����飨����������������ʱ��ֻ��2�����ԣ�
	int numberOfPropertyGroups();
	/// ��ȡĳ������
	std::vector<CZProperty> & propertiesGroupAt(int i); 
	/// �������Ա仯��ʵ������ί�нӿڣ�
	void propertyChanged(CZProperty &property_);
	/// �����������仯��ʵ��������ί�нӿڣ�
	void generatorChanged(CZStampGenerator &gen_);
	/// ʵ��coding�ӿ�
	void update(CZDecoder *decoder, bool deep = false);
	void encode(CZCoder *coder, bool deep = false);

	/// ��������¼���ί��
	void addProChangeDelegate(CZBrushDelegate* ptr);
	void addGenChangeDelegate(CZBrushDelegate* ptr);
	void addGenReplaceDelegate(CZBrushDelegate* ptr);

private:
	/// �������ԣ�������ֵ��
	void buildProperties();
	/// ��ѹ��ֵ
	float decodeValue(const char *key, CZDecoder *decoder, float deft);

public:
	unsigned int uid;				///< ��ԭ����У�uid�����Ժ��������ı���ı�
	CZStampGenerator *generator;	///< ��ˢ���������������ฺ������
	CZProperty weight;				///< ���ʵĴ�ϸ [1.0, 512.0] -- pixels	

	CZProperty intensity;			///< [0.0, 1.0]
	CZProperty angle;              // [0.0, 1.0];
	CZProperty spacing;            // [0.01, 2.0] -- percentage of brush width
	CZProperty rotationalScatter;  // [0.0, 1.0]
	CZProperty positionalScatter;  // [0.0, 1.0]

	CZProperty angleDynamics;      // [-1.0, 1.0]
	CZProperty weightDynamics;     ///< Ŀ���ǻ��ʻ���ʱ�Ĵ�ϸ����Сֵ [-1.0, 1.0]
	CZProperty intensityDynamics;  // [-1.0, 1.0]

private:
	int suppressNum;								///< ����֪ͨ����Ŀ
	std::vector<CZProperty> changedProperties;			///< �ı��˵����ԣ���vector������set������дCZProperty�ıȽϺ�����
	std::vector<CZBrushDelegate*> proChangeDelegate;			///< ��ί��ֱ��ģ����Ϣ���ģ������ˢ���Ըı�
	std::vector<CZBrushDelegate*> genChangeDelegate;			///< �����ˢ�������ı�
	std::vector<CZBrushDelegate*> genReplaceDelegate;			///< �����ˢ�������滻

	static int brushNum;							///< ��¼��ˢ��Ŀ
	CZImage *strokePreview;							///< ��ˢ��Ԥ��ͼ
};

/// ����һ����ˢί���࣬���������Ϣ����
class CZBrushDelegate
{
public:
	virtual void brushPropertyChanged(std::vector<CZProperty> &properties) = 0;	///< �ı�����ʱʵ�ָýӿ�
	virtual void brushGeneratorChanged(CZStampGenerator &gen) = 0;				///< �ı���滻������ʱ��ʵ�ָýӿ�
};
#endif