
///  \file CZLayer.h
///  \brief This is the file declare the class CZLayer.
///
///		
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note

#ifndef _CZLAYER_H_
#define _CZLAYER_H_

#include "CZPaintingRender.h"
#include "CZImage.h"
#include "CZColor.h"
#include "CZColorBalance.h"
#include "CZHueSaturation.h"
#include "CZAffineTransform.h"
#include "CZCoder.h"
#include "CZDataProvider.h"
#include <string>

extern std::string CZColorBalanceChanged;   // ColorBalanceChanged";
extern std::string CZHueSaturationChanged;   // HueSaturationChanged";
extern std::string CZLayerVisibilityChanged;   // LayerVisibilityChanged";
extern std::string CZLayerLockedStatusChanged;   // LayerLockedStatusChanged";
extern std::string CZLayerAlphaLockedStatusChanged;   // LayerAlphaLockedStatusChanged";
extern std::string CZLayerOpacityChanged;   // LayerOpacityChanged";
extern std::string CZLayerBlendModeChanged;   // LayerBlendModeChanged";
extern std::string CZLayerContentsChangedNotification;   // LayerContentsChangedNotification";
extern std::string CZLayerThumbnailChangedNotification;   // LayerThumbnailChangedNotification";
extern std::string CZLayerTransformChangedNotification;   // LayerTransformChangedNotification";

class CZPainting;

class CZLayer : public CZCoding
{
public:
	CZLayer();
	~CZLayer();
	/// ͼ���ͼ������
	CZImage *imageData();
	/// �����ض����������ͼ������
	CZImage *imageDataInRect(const CZRect &rect);
	/// ��ͼ�����ݰ����ͻ���
	void blit(CZPaintingRender* ptrRender, CZMat4 &projection);
	/// ���Ӳ�������
	void blitWithEraseMask(CZPaintingRender* ptrRender, CZMat4 &projection);
	/// ���ӻ�������
	void blitWithMask(CZPaintingRender* ptrRender, CZMat4 &projection,CZColor *bgColor);

	/// ����ת������
	void setTransform(CZAffineTransform &trans);
	/// ���û��ģʽ
	void setBlendMode(CZRender::BlendMode &bm);
	/// ���õ�����ɫ
	void setColorBalance(CZColorBalance *cb);
	/// ������ɫ
	void commitColorAdjustments();

	/// �л��ɼ���
	void toggleVisibility();
	/// �л�alpha����
	void toggleAlphaLocked();
	/// �л�ͼ������
	void toggleLocked();

	/// �Ƿ�������Ϣ
	bool isSuppressingNotifications();

	/// ʵ��coding�Ľӿ�
	void update(CZDecoder *decoder_, bool deep = false);
	void encode(CZCoder *coder_, bool deep = false);

public:
	bool visible;								///< �Ƿ�ɼ�
	bool alphaLocked;							///< alpha�Ƿ�����
	bool locked;								///< ͼ������
	bool clipWhenTransformed;					///< ͼ��任ʱ�Ƿ����
	CZColorBalance *colorBalance;				///< ������ɫ
	CZHueSaturation *hueSaturation;				///< ����ɫ�����Ͷ�
	CZPainting *ptrPainting;					///< ĳ�λ���
	CZRender::BlendMode blendMode;				///< ���ģʽ
	float opacity;								///< ��͸����[0.0, 1.0]
	CZImage *image;								///< ��ͼ��ͼ��
	CZAffineTransform transform;				///< �任����
	CZSaveStatus isSaved;						///< ͼ��洢״̬

	unsigned int uid;							///< ���
};

#endif