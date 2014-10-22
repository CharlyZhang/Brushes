
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
	/// ��ͼ����Ƴ�����û��FBO��
	void blit(CZMat4 &projection);
	/// ���ɵ�ǰ״̬��ͼ��
	CZImage *imageForCurrentState(CZColor *backgroundColor);
	/// ����һ���켣�����Ƶ�����
	CZRect paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer = false);
	/// �Ƿ�������Ϣ
	bool isSuppressingNotifications();
	/// ���÷�Χ
	void setDimensions(const CZSize &size);
	/// ���õ�ǰͼ��
	void setActiveLayer(CZLayer *layer);
	/// ͨ��UID��ȡͼ��
	CZLayer *layerWithUID(unsigned int uid_);
	/// ɾ��ͼ��
	void removeLayer(CZLayer *layer);
	/// ����ͼ��
	void insertLayer(int idx, CZLayer *layer);
	/// ���ͼ��
	void addLayer(CZLayer *layer);

	/// ʵ��coding�Ľӿ�
	void update(CZDecoder *decoder_, bool deep = false);
	void encode(CZCoder *coder_, bool deep = false);

private:
	/// ��ʼ������Ϣ����
	void beginSuppressingNotifications();
	/// ����������Ϣ����
	void endSuppressingNotifications();
	/// ���ͼ��������ͼ���еı�ţ������ڷ��ظ�ֵ
	int indexOfLayers(CZLayer *layer);

public:
	CZSize dimensions;				///< ���Ʒ�Χ
	CZPaintingRender *render;		///< ������
	CZPath *ptrActivePath;			///< �����·�����˴���Ϊ����
	CZLayer *ptrActiveLayer;		///< ��ǰ���Ʋ�

private:
	static unsigned int paintingNum;///< ���Ƶ���Ŀ
	CZUndoManager *undoManager;		///< �س�������

	bool flattenMode;
	std::vector<CZLayer*>	layers;	///< ���ƵĻ���

	std::vector<CZColor*>	colors;	
	std::vector<CZBrush*>	brushes;
	std::vector<CZBrush*>	undoBrushes;
	int						strokeCount;	///< �ʻ���Ŀ
	unsigned int			uid;

	CZBrush					*ptrLastBrush;	///< ��һ�ѻ�ˢ

	unsigned int			suppressNotifications;	///< ���Ƶ���Ϣ��
};
#endif