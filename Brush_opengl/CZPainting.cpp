
///  \file CZPainting.cpp
///  \brief This is the file implements the class CZPainting.
///
///		
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-26
///  \note

#include "CZPainting.h"
#include "Macro.h"
#include "CZUtil.h"
#include "CZLayer.h"
#include "CZPaintingRender.h"
#include <map>

using namespace  std;

// Notifications
string CZStrokeAddedNotification = "StrokeAddedNotification";
string CZLayersReorderedNotification = "LayersReorderedNotification";
string CZLayerAddedNotification = "LayerAddedNotification";
string CZLayerDeletedNotification = "LayerDeletedNotification";
string CZActiveLayerChangedNotification = "ActiveLayerChangedNotification";

unsigned int CZPainting::paintingNum = 0;

CZPainting::CZPainting(const CZSize &size)
{
	render = new CZPaintingRender(size);
	undoManager = new CZUndoManager;

	setDimensions(size);

	colors.clear();
	brushes.clear();
	undoBrushes.clear();
	strokeCount = 0;

	ptrActivePath = NULL;
	ptrActiveLayer = NULL;

	flattenMode = false;
	layers.clear();
	
	ptrLastBrush = NULL;

	uid = PAINTING_BASE_UID + (paintingNum++);
}
CZPainting::~CZPainting()
{
	if(render) { delete render; render = NULL;}

	colors.clear();
	brushes.clear();
	undoBrushes.clear();
	layers.clear();

	//registerForUndoNotifications()

	paintingNum --;
}

/// 将图像绘制出来（没绑定FBO）
void CZPainting::blit(CZMat4 &projection)
{
	if (flattenMode) 
	{
		//[self blitFlattenedTexture:projection];
		return;
	}

	for (vector<CZLayer*>::iterator itr = layers.begin(); itr != layers.end(); itr++) 
	{
		CZLayer *layer = *itr;
		if (!layer->visible) continue;

		/// 给绘制器传递数据
		map<string,void*> conf;
		conf.insert(make_pair("layer",layer));
		render->configure(conf);

		if (ptrActiveLayer == layer && ptrActivePath) 
		{
			if (ptrActivePath->action == CZPathActionErase) 
				layer->blit(projection,render,CZRender::kPaintingErase);
			else 
				layer->blit(projection,render,CZRender::kPaintingMask,&(ptrActivePath->color));
		} else 
			layer->blit(projection,render,CZRender::kPaintingBlit);
	}
}

/// 生成当前状态的图像
CZImage *CZPainting::imageForCurrentState(const CZColor *backgroundColor)
{
	/// 给绘制器传递数据
	map<string,void*> conf;
	conf.insert(make_pair("bgColor",(void*)backgroundColor));
	render->configure(conf);

	render->begin(CZRender::kPaingtingImageData);

	for (vector<CZLayer*>::iterator itr = layers.begin(); itr != layers.end(); itr++) 
	{
		CZLayer *layer = *itr;
		if (!layer->visible) continue;

		/// 给绘制器传递数据
		conf.clear();
		conf.insert(make_pair("layer",(void*)layer));
		render->configure(conf);

		if (ptrActiveLayer == layer && ptrActivePath) 
		{
			if (ptrActivePath->action == CZPathActionErase) 
				layer->blit(projectionMat,render,CZRender::kPaintingErase);
			else 
				layer->blit(projectionMat,render,CZRender::kPaintingMask,&(ptrActivePath->color));
		} else 
			layer->blit(projectionMat,render,CZRender::kPaintingBlit);
	}

	CZImage *ret = render->imageForLastDraw();

	render->end(CZRender::kPaintingImageForCurrent);

	return ret;
}

/// 绘制一条轨迹
CZRect CZPainting::paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer /* = false */)
{
	ptrActivePath = path_;

	CZRect pathBounds;

	/// 渲染器绘制到纹理
	render->setRenderToTexture();
	render->setClearBuffer(clearBuffer);
	if(path_->ptrBrush != ptrLastBrush)
	{
		render->changeBrushTex(path_->ptrBrush);
		ptrLastBrush = path_->ptrBrush;
	}

	/// 准备绘制
	render->begin(CZRender::kDrawPath);

	pathBounds = path_->paint(render,randomizer);
	
	render->end(CZRender::kDrawPath);

	//CZNotificationCenter::getInstance()->notify(CZStrokeAddedNotification,this,(void*)&pathBounds);
	
	return pathBounds;
}

/// 设置范围
void CZPainting::setDimensions(const CZSize &size)
{
	if(dimensions == size) return;

	dimensions = size;
	render->resize(size);

	projectionMat.SetOrtho(0,dimensions.width,0,dimensions.height,-1.0f,1.0f);
}

/// 设置当前图层
void CZPainting::setActiveLayer(CZLayer *layer)
{
	if (!layer) 
	{
		cout << "CZPainting::setActiveLayer - layer is NULL\n";
		return;
	}

	if (layer == ptrActiveLayer)
	{
		return;
	}

	int oldIndex = indexOfLayers(ptrActiveLayer);

	ptrActiveLayer = layer;

	if (!isSuppressingNotifications()) 
	{
		//CZNotificationCenter::getInstance()->notify(CZActiveLayerChangedNotification,this,(void*)&oldIndex);
	}
}

/// 是否抑制消息
bool CZPainting::isSuppressingNotifications()
{
	return (suppressNotifications > 0) ? true : false;
}

/// 通过UID获取图层
CZLayer *CZPainting::layerWithUID(unsigned int uid_)
{
	int num = layers.size();
	for(int i = 0; i < num; i ++)
		if(uid_ == layers[i]->uid) return layers[i];

	return NULL;
}

/// 删除图层
void CZPainting::removeLayer(CZLayer *layer)
{
//	undoManager->prepareForUndo()
	//[[undoManager_ prepareWithInvocationTarget:self] insertLayer:layer atIndex:[layers_ indexOfObject:layer]];    

	if (layer == ptrActiveLayer && layers.size()> 1)
	{
		// choose another layer to be active before we remove it
		int index = indexOfLayers(ptrActiveLayer);
		if (index >= 1)
		{
			index--;
		}
		else
		{
			index = 1;
		}
		ptrActiveLayer = layers[index];
	}

	// do this before decrementing index
	for(vector<CZLayer*>::iterator itr=layers.begin(); itr!=layers.end(); itr++)
		if(*itr == layer)
		{
			layers.erase(itr);
			break;
		}

	//[layer freeze];

	if (!isSuppressingNotifications())
	{
	//	NSValue *dirtyRect = [NSValue valueWithCGRect:self.bounds];
	//	NSDictionary *userInfo = @{@"index": @(index), @"rect": dirtyRect, @"layer": layer};
	//	[[NSNotificationCenter defaultCenter] postNotification:[NSNotification notificationWithName:WDLayerDeletedNotification object:self userInfo:userInfo]];
	}
}

/// 插入图层
void CZPainting::insertLayer(int idx, CZLayer *layer)
{
	//[[undoManager_ prepareWithInvocationTarget:self] removeLayer:layer];

//	layers.insert(idx,layer);
	if (!isSuppressingNotifications()) 
	{
		//NSDictionary *userInfo = @{@"layer": layer, @"rect": [NSValue valueWithCGRect:self.bounds]};
		//[[NSNotificationCenter defaultCenter] postNotification:[NSNotification notificationWithName:WDLayerAddedNotification object:self userInfo:userInfo]];
	}
}

/// 添加图层
void CZPainting::addLayer(CZLayer *layer)
{
	layer->ptrPainting = this;
	insertLayer(indexOfLayers(ptrActiveLayer)+1, layer);
	ptrActiveLayer = layer;
}

/// 开始抑制消息发送
void CZPainting::beginSuppressingNotifications()
{
	suppressNotifications ++;
}
/// 结束抑制消息发送
void CZPainting::endSuppressingNotifications()
{
	suppressNotifications --;
	
	if (suppressNotifications < 0) 
	{
		cout << "CZPainting::endSuppressingNotifications - Unbalanced notification suppression: " 
			<< suppressNotifications;
	}
}

/// 获得图层在所有图层中的标号，不存在返回负值
int CZPainting::indexOfLayers(CZLayer *layer)
{
	int ret;
	int num = layers.size();
	for(ret = 0; ret < num; ret ++)
		if(layer == layers[ret]) return ret;

	return -1;
}

/// 实现CZCoding接口
void CZPainting::update(CZDecoder *decoder_, bool deep /*= false*/){};
void CZPainting::encode(CZCoder *coder_, bool deep /*= false*/){};
