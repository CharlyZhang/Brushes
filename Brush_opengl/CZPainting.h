
///  \file CZPainting.h
///  \brief This is the file declare the class CZPainting.
///
///		
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-26
///  \note

#ifndef _CZPAINTING_H_
#define _CZPAINTING_H_

#include "CZGeometry.h"
#include "CZPath.h"
#include "CZRandom.h"
#include "CZMat4.h"
#include "CZLayer.h"
#include "CZCoding.h"
#include "CZPaintingRender.h"
#include "CZUndoManager.h"
#include <string>
#include <vector>

extern std::string CZStrokeAddedNotification; //  "StrokeAddedNotification";
extern std::string CZLayersReorderedNotification; //  "LayersReorderedNotification";
extern std::string CZLayerAddedNotification; //  "LayerAddedNotification";
extern std::string CZLayerDeletedNotification; //  "LayerDeletedNotification";
extern std::string CZActiveLayerChangedNotification; //  "ActiveLayerChangedNotification";

class CZPainting :public CZCoding
{
public:
	CZPainting(const CZSize &size);
	~CZPainting();
	/// 将图像绘制出来（没绑定FBO）
	void blit(CZMat4 &projection);
	/// 生成当前状态的图像
	CZImage *imageForCurrentState(CZColor *backgroundColor);
	/// 绘制一条轨迹（绘制到纹理）
	CZRect paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer = false);
	/// 是否抑制消息
	bool isSuppressingNotifications();
	/// 设置范围
	void setDimensions(const CZSize &size);
	/// 设置当前图层
	void setActiveLayer(CZLayer *layer);
	/// 通过UID获取图层
	CZLayer *layerWithUID(unsigned int uid_);
	/// 删除图层
	void removeLayer(CZLayer *layer);
	/// 插入图层
	void insertLayer(int idx, CZLayer *layer);
	/// 添加图层
	void addLayer(CZLayer *layer);

	/// 实现coding的接口
	void update(CZDecoder *decoder_, bool deep = false);
	void encode(CZCoder *coder_, bool deep = false);

private:
	/// 开始抑制消息发送
	void beginSuppressingNotifications();
	/// 结束抑制消息发送
	void endSuppressingNotifications();
	/// 获得图层在所有图层中的标号，不存在返回负值
	int indexOfLayers(CZLayer *layer);

public:
	CZSize dimensions;				///< 绘制范围
	CZPaintingRender *render;		///< 绘制器
	CZPath *ptrActivePath;			///< 激活的路径，此处仅为引用
	CZLayer *ptrActiveLayer;		///< 当前绘制层

private:
	static unsigned int paintingNum;///< 绘制的数目
	CZUndoManager *undoManager;		///< 回撤控制器

	bool flattenMode;
	std::vector<CZLayer*>	layers;	///< 绘制的画层

	std::vector<CZColor*>	colors;	
	std::vector<CZBrush*>	brushes;
	std::vector<CZBrush*>	undoBrushes;
	int						strokeCount;	///< 笔画数目
	unsigned int			uid;

	CZBrush					*ptrLastBrush;	///< 上一把画刷

	unsigned int			suppressNotifications;	///< 抑制的消息数
};
#endif