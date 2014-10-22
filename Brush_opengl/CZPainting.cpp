
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
	render->ptrPainting = this;

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

	// register for undo notification

	uid = PAINTING_BASE_UID + (paintingNum++);
}
CZPainting::~CZPainting()
{
	if(render) { delete render; render = NULL;}
	if(undoManager) { delete undoManager; undoManager = NULL;}

	colors.clear();
	brushes.clear();
	undoBrushes.clear();
	layers.clear();

	//registerForUndoNotifications()

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
		if (!layer->visible) continue;

		render->ptrLayer = layer;
		if (ptrActiveLayer == layer && ptrActivePath) 
		{
			if (ptrActivePath->action == CZPathActionErase)
				layer->blitWithEraseMask(render,projection);
			else 
				layer->blitWithMask(render,projection,&(ptrActivePath->color));
		} else 
			layer->blit(render,projection);
	}
}

/// ���ɵ�ǰ״̬��ͼ��
CZImage *CZPainting::imageForCurrentState(CZColor *backgroundColor)
{
	return render->drawPaintingCurrentState(backgroundColor, layers);
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

/// ���õ�ǰͼ��
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

/// �Ƿ�������Ϣ
bool CZPainting::isSuppressingNotifications()
{
	return (suppressNotifications > 0) ? true : false;
}

/// ͨ��UID��ȡͼ��
CZLayer *CZPainting::layerWithUID(unsigned int uid_)
{
	int num = layers.size();
	for(int i = 0; i < num; i ++)
		if(uid_ == layers[i]->uid) return layers[i];

	return NULL;
}

/// ɾ��ͼ��
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

/// ����ͼ��
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

/// ���ͼ��
void CZPainting::addLayer(CZLayer *layer)
{
	layer->ptrPainting = this;
	insertLayer(indexOfLayers(ptrActiveLayer)+1, layer);
	ptrActiveLayer = layer;
}

/// ��ʼ������Ϣ����
void CZPainting::beginSuppressingNotifications()
{
	suppressNotifications ++;
}
/// ����������Ϣ����
void CZPainting::endSuppressingNotifications()
{
	suppressNotifications --;

	if (suppressNotifications < 0) 
	{
		cout << "CZPainting::endSuppressingNotifications - Unbalanced notification suppression: " 
			<< suppressNotifications;
	}
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

/// ʵ��CZCoding�ӿ�
void CZPainting::update(CZDecoder *decoder_, bool deep /*= false*/){};
void CZPainting::encode(CZCoder *coder_, bool deep /*= false*/){};
