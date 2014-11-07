
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

const int iMaxLayerNumber = 10;		///< ֧�ֵ����ͼ����Ŀ

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

/// ��ͼ����Ƴ�����û��FBO��
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

/// ��������ͼ���ͼ�񣨲�������ǰ���Ƶıʻ���
CZImage *CZPainting::imageWithSize(CZSize &size, CZColor *backgroundColor)
{
	return render->drawPaintingImage(size,backgroundColor);
}

/// ���ɵ�ǰ״̬��ͼ��
CZImage *CZPainting::imageForCurrentState(CZColor *backgroundColor)
{
	return render->drawPaintingCurrentState(backgroundColor);
}

/// ����һ���켣�����Ƶ�����
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

/// ���÷�Χ
void CZPainting::setDimensions(const CZSize &size)
{
	if(dimensions == size) return;

	dimensions = size;
	render->resize(size);
}

/// ���õ�ǰ����ͼ��
///
///		\param idx - ��ǰ��Ҫ�����ͼ�����
///		\ret	   - ԭ�������ͼ�����
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

/// ͨ��UID��ȡͼ��
CZLayer *CZPainting::layerWithUID(unsigned int uid_)
{
	int num = layers.size();
	for(int i = 0; i < num; i ++)
		if(uid_ == layers[i]->getUID()) return layers[i];

	return NULL;
}

/// ɾ��ͼ��
/// 
///		\param - ��Ҫɾ����ͼ��
///		\ret   - ԭͼ�����ڵ����
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
			/// ɾ��ͼ���������Դ
			render->clearLayerTexture(layer);
			render->clearLayerHCLTexture(layer);

			layers.erase(itr);
			break;
		}

	return oldIdx;
}

/// ����ͼ��
void CZPainting::insertLayer(int idx, CZLayer *layer)
{
	if(idx < 0 || idx > layers.size())
	{
		cerr << "CZPainting::insertLayer - idx is out of range\n";
		return;
	}

	layers.insert(layers.begin()+idx,layer);
}

/// ���ͼ��
/// 
///		\param layer - ��ӵ�ͼ��
///		\ret		 - ������ͼ���е����
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

/// ���ºϲ���ǰͼ��
/// 
///		\ret - �Ƿ�ϲ��ɹ�
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

/// �ƶ�ͼ��
/// 
///		\param layer - ��Ҫ�ƶ���ͼ��
///		\param idx	 - �ƶ�����λ��
bool CZPainting::moveLayer(CZLayer* layer, int idx)
{ 
	return false;
}

/// ��ȡ����ͼ��
std::vector<CZLayer*> & CZPainting::getAllLayers()
{
	return layers;
}


/// ���ͼ��������ͼ���еı�ţ������ڷ��ظ�ֵ
int CZPainting::indexOfLayers(CZLayer *layer)
{
	int ret;
	int num = layers.size();
	for(ret = 0; ret < num; ret ++)
		if(layer == layers[ret]) return ret;

	return -1;
}


/// ���ü���켣
void CZPainting::setActivePath(CZPath *path)
{
	ptrActivePath = path;
}

/// ��ȡ����켣
CZPath* CZPainting::getActivePath()
{
	return ptrActivePath;
}

/// ���ü���ͼ��
void CZPainting::setActiveLayer(CZLayer *layer)
{
	ptrActiveLayer = layer;
}

/// ��ȡ����ͼ��
CZLayer* CZPainting::getActiveLayer()
{
	return ptrActiveLayer;
}

/// ��ȡ��Ⱦ��
CZPaintingRender* CZPainting::getRender()
{
	return render;
}

/// ��ȡ��Χ
CZSize& CZPainting::getDimensions()
{
	return dimensions;
}

/// ��ȡ���ƾ���
CZRect& CZPainting::getBounds()
{
	return CZRect(0,0,dimensions.width,dimensions.height);
}

/// ʵ��CZCoding�ӿ�
void CZPainting::update(CZDecoder *decoder_, bool deep /*= false*/){};
void CZPainting::encode(CZCoder *coder_, bool deep /*= false*/){};
