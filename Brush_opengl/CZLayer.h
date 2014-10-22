
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
	/// 图层的图像数据
	CZImage *imageData();
	/// 生成特定矩形区域的图像数据
	CZImage *imageDataInRect(const CZRect &rect);
	/// 将图层内容按类型绘制
	void blit(CZPaintingRender* ptrRender, CZMat4 &projection);
	/// 叠加擦除纹理
	void blitWithEraseMask(CZPaintingRender* ptrRender, CZMat4 &projection);
	/// 叠加绘制纹理
	void blitWithMask(CZPaintingRender* ptrRender, CZMat4 &projection,CZColor *bgColor);

	/// 设置转换矩阵
	void setTransform(CZAffineTransform &trans);
	/// 设置混合模式
	void setBlendMode(CZRender::BlendMode &bm);
	/// 设置调整颜色
	void setColorBalance(CZColorBalance *cb);
	/// 调整颜色
	void commitColorAdjustments();

	/// 切换可见性
	void toggleVisibility();
	/// 切换alpha锁定
	void toggleAlphaLocked();
	/// 切换图层锁定
	void toggleLocked();

	/// 是否抑制消息
	bool isSuppressingNotifications();

	/// 实现coding的接口
	void update(CZDecoder *decoder_, bool deep = false);
	void encode(CZCoder *coder_, bool deep = false);

public:
	bool visible;								///< 是否可见
	bool alphaLocked;							///< alpha是否锁定
	bool locked;								///< 图层锁定
	bool clipWhenTransformed;					///< 图层变换时是否剪切
	CZColorBalance *colorBalance;				///< 调整颜色
	CZHueSaturation *hueSaturation;				///< 调整色调饱和度
	CZPainting *ptrPainting;					///< 某次绘制
	CZRender::BlendMode blendMode;				///< 混合模式
	float opacity;								///< 不透明度[0.0, 1.0]
	CZImage *image;								///< 该图层图像
	CZAffineTransform transform;				///< 变换矩阵
	CZSaveStatus isSaved;						///< 图层存储状态

	unsigned int uid;							///< 编号
};

#endif