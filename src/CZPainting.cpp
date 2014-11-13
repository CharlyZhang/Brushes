
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
#include <map>

using namespace  std;

const int iMaxLayerNumber = 10;		///< 支持的最大图层数目

unsigned int CZPainting::paintingNum = 0;

CZPainting::CZPainting(const CZSize &size)
{
	render = new CZPaintingRender(size);
	render->ptrPainting = this;

	setDimensions(size);

	flattenMode = false;
	
	colors.clear();
	brushes.clear();
	layers.clear();
	undoBrushes.clear();
	strokeCount = 0;

	ptrActivePath = NULL;
	ptrLastBrush = NULL;

	/// add the default blank layer
	ptrActiveLayer = new CZLayer;
	addLayer(ptrActiveLayer);

	uid = PAINTING_BASE_UID + (paintingNum++);
}
CZPainting::~CZPainting()
{
	if(render) { delete render; render = NULL;}

	colors.clear();
	brushes.clear();
	undoBrushes.clear();

	for(vector<CZLayer*>::iterator itr = layers.begin(); itr != layers.end(); itr++)
		delete *itr;
	layers.clear();

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
		if (!layer->isVisible()) continue;

		if (ptrActiveLayer == layer && ptrActivePath) 
		{
			if (ptrActivePath->action == CZPathActionErase)
				layer->blitWithEraseMask(projection);
			else 
				layer->blitWithMask(projection,&(ptrActivePath->color));
		} else 
			layer->blit(projection);
	}
}

/// 生成所有图层的图像（不包括当前绘制的笔画）
CZImage *CZPainting::imageWithSize(CZSize &size, CZColor *backgroundColor)
{
	return render->drawPaintingImage(size,backgroundColor);
}

/// 生成当前状态的图像
CZImage *CZPainting::imageForCurrentState(CZColor *backgroundColor)
{
	return render->drawPaintingCurrentState(backgroundColor);
}

/// 绘制一条轨迹（绘制到纹理）
CZRect CZPainting::paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer /* = false */)
{
	ptrActivePath = path_;

	if(path_->ptrBrush != ptrLastBrush)
	{
		render->changeBrushTex(path_->ptrBrush);
		ptrLastBrush = path_->ptrBrush;
	}


	CZRect pathBounds = render->drawPaintingStroke(path_,randomizer,clearBuffer);

	return pathBounds;
}

/// 设置范围
void CZPainting::setDimensions(const CZSize &size)
{
	if(dimensions == size) return;

	dimensions = size;
	render->resize(size);
}

/// 设置当前激活图层
///
///		\param idx - 当前需要激活的图层序号
///		\ret	   - 原来激活的图层序号
int CZPainting::setActiveLayer(int idx)
{
	int oldIndex = indexOfLayers(ptrActiveLayer);

	if (idx < 0 || idx >= layers.size()) 
	{
		cerr << "CZPainting::setActiveLayer - idx is out of range\n";
		return oldIndex;
	}

	ptrActiveLayer = layers[idx];

	return oldIndex;

}

/// 通过UID获取图层
CZLayer *CZPainting::layerWithUID(unsigned int uid_)
{
	int num = layers.size();
	for(int i = 0; i < num; i ++)
		if(uid_ == layers[i]->getUID()) return layers[i];

	return NULL;
}

/// 删除图层
/// 
///		\param - 需要删除的图层
///		\ret   - 原图层所在的序号
int CZPainting::removeLayer(CZLayer *layer)
{
	if(layer == NULL)
	{
		cerr << "CZPainting::removeLayer - layer is NULL\n";
		return -1;
	}

	int oldIdx = indexOfLayers(layer);

	if(layer->isLocked())
	{
		cerr << "CZPainting::removeLayer - layer is locked\n";
		return oldIdx;
	}

	if (layer == ptrActiveLayer && layers.size()> 1)
	{
		// choose another layer to be active before we remove it
		int index;
		if (oldIdx >= 1)
		{
			index = oldIdx-1;
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
			/// 删除图层的纹理资源
			render->clearLayerTexture(layer);
			render->clearLayerHCLTexture(layer);

			layers.erase(itr);
			break;
		}

	return oldIdx;
}

/// 插入图层
void CZPainting::insertLayer(int idx, CZLayer *layer)
{
	if(idx < 0 || idx > layers.size())
	{
		cerr << "CZPainting::insertLayer - idx is out of range\n";
		return;
	}

	layers.insert(layers.begin()+idx,layer);
}

/// 添加图层
/// 
///		\param layer - 添加的图层
///		\ret		 - 在所有图层中的序号
int CZPainting::addLayer(CZLayer *layer)
{
	if(layer == NULL)
	{
		cerr << "CZPainting::addLayer - layer is NULL\n";
		return -1;
	}

	if(layers.size() > iMaxLayerNumber)
	{
		cerr << "CZPainting::addLayer - has reached the max number of Layers\n";
		return -1;
	}

	int newIdx = indexOfLayers(ptrActiveLayer)+1;
	
	layer->setPainting(this);
	insertLayer(newIdx, layer);
	ptrActiveLayer = layer;

	return newIdx;
}

/// 向下合并当前图层
/// 
///		\ret - 是否合并成功
bool CZPainting::mergeActiveLayerDown()
{
	int activeIdx = indexOfLayers(ptrActiveLayer);
	
	/// in case the active layer is at bottom
	if(activeIdx <= 0)
	{
		cerr << "CZPainting::mergeActiveLayerDown - active layer is NULL or at bottom\n";
		return false;
	}

	/// in case there's only one layer
	if(layers.size() <= 1)
	{
		cerr << "CZPainting::mergeActiveLayerDown - layers number is no more than 1\n";
		return false;
	}

	CZLayer *bottomLayer = layers[activeIdx - 1];

	/// in case the layer to merged with is not editable
	if(!bottomLayer->isEditable())
	{
		cerr << "CZPainting::mergeActiveLayerDown - layer to be merged with is not editable\n";
		return false;
	}

	bool ret = bottomLayer->merge(ptrActiveLayer);
	removeLayer(ptrActiveLayer);
	ptrActiveLayer = bottomLayer;

	return ret;
}

/// 移动图层
/// 
///		\param layer - 需要移动的图层
///		\param idx	 - 移动到的位置
bool CZPainting::moveLayer(CZLayer* layer, int idx)
{ 
	return false;
}

/// 获取所有图层
std::vector<CZLayer*> & CZPainting::getAllLayers()
{
	return layers;
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


/// 设置激活轨迹
void CZPainting::setActivePath(CZPath *path)
{
	ptrActivePath = path;
}

/// 获取激活轨迹
CZPath* CZPainting::getActivePath()
{
	return ptrActivePath;
}

/// 设置激活图层
void CZPainting::setActiveLayer(CZLayer *layer)
{
	ptrActiveLayer = layer;
}

/// 获取激活图层
CZLayer* CZPainting::getActiveLayer()
{
	return ptrActiveLayer;
}

/// 获取渲染器
CZPaintingRender* CZPainting::getRender()
{
	return render;
}

/// 获取范围
CZSize& CZPainting::getDimensions()
{
	return dimensions;
}

/// 获取绘制矩形
CZRect& CZPainting::getBounds()
{
    bounds = CZRect(0,0,dimensions.width,dimensions.height);
	return bounds;
}

/// 实现CZCoding接口
void CZPainting::update(CZDecoder *decoder_, bool deep /*= false*/){};
void CZPainting::encode(CZCoder *coder_, bool deep /*= false*/){};
