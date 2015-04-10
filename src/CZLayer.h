
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
#include "basic/CZImage.h"
#include "basic/CZColor.h"
#include "CZColorBalance.h"
#include "CZHueSaturation.h"
#include "basic/CZAffineTransform.h"
#include "serialization/CZCoding.h"
#include "CZDataProvider.h"
#include <string>

class CZPainting;
class CZRect;

/// 图层类
class CZLayer : public CZCoding
{
public:
	CZLayer();
	~CZLayer();
	friend class CZPaintingRender;

	/// 图层的图像数据
	CZImage *imageData();
	/// 生成特定矩形区域的图像数据
	CZImage *imageDataInRect(const CZRect &rect);
	
	/// 绘制图层
	void basicBlit(CZMat4 &projection);
	/// 绘制图层（考虑移动转换、颜色调整等）
	void blit(CZMat4 &projection);
	/// 叠加擦除纹理
	void blitWithEraseMask(CZMat4 &projection);
	/// 叠加绘制纹理
	void blitWithMask(CZMat4 &projection,CZColor *bgColor);
	
	/// 将绘制的笔画合并到当前图层
	void commitStroke(CZRect &bounds, CZColor &color, bool erase, bool undoable);
	
	/// 调整颜色
	void commitColorAdjustments();
	
	/// 合并另以图层
	bool merge(CZLayer *layer);

	/// 设置绘制指针
	void setPainting(CZPainting *painting);
	/// 设置转换矩阵（没发生改变，则返回false）
	bool setTransform(CZAffineTransform &trans);
	/// 获取转换矩阵
	CZAffineTransform & getTransform();
	/// 设置混合模式（没发生改变，则返回false）
	bool setBlendMode(CZRender::BlendMode &bm);
	/// 获取混合模式
	CZRender::BlendMode getBlendMode();
	/// 设置不透明度（没发生改变，则返回false）
	bool setOpacity(float o);
	/// 获取不透明度
	float getOpacity();
	/// 设置调整颜色（没发生改变，则返回false）
	bool setColorBalance(CZColorBalance *cb);
	/// 设置色调调整
	bool setHueSaturation(CZHueSaturation *hs);
	/// 设置图层图像
	/// 
	///		\param img - 设置的图像
	///		\ret	   - 若img不为空，则设置陈宫
	///		\note 调用此函数将覆盖之前对该层的所有绘制
	bool setImage(CZImage *img);

	/// 切换可见性
	void toggleVisibility();
	/// 切换alpha锁定
	void toggleAlphaLocked();
	/// 切换图层锁定
	void toggleLocked();
	/// 设置可见性
	void setVisiblility(bool flag);
	/// 设置alpha锁定
	void setAlphaLocked(bool flag);
	/// 设置图层锁定
	void setLocked(bool flag);
	/// 是否被锁住图层
	bool isLocked();
	/// 是否被锁住alpha
	bool isAlphaLocked();
	/// 是否可见
	bool isVisible();
	/// 是否可编辑
	bool isEditable();

	/// 获取编号
	unsigned int getUID();

	/// 实现coding的接口
	void update(CZDecoder *decoder_, bool deep = false);
	void encode(CZCoder *coder_, bool deep = false);

private:
	static unsigned int layerNumber;			///< 图层的数目
	bool visible;								///< 是否可见
	bool alphaLocked;							///< alpha是否锁定
	bool locked;								///< 图层锁定
	bool clipWhenTransformed;					///< 图层变换时是否剪切
	CZColorBalance *colorBalance;				///< 调整颜色
	CZHueSaturation *hueSaturation;				///< 调整色调饱和度
	CZRender::BlendMode blendMode;				///< 混合模式
	CZAffineTransform transform;				///< 变换矩阵
	float opacity;								///< 不透明度[0.0, 1.0]

	CZPainting *ptrPainting;					///< 某次绘制
	CZImage *image;								///< 该图层图像（用于建立初始图层纹理）
	CZSaveStatus isSaved;						///< 图层存储状态

	unsigned int uid;							///< 编号
};

#endif