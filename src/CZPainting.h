
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
#include <string>
#include <vector>

/// ������
class CZPainting :public CZCoding
{
public:
	CZPainting(const CZSize &size);
	~CZPainting();
	
	/// ��ͼ����Ƴ�����û��FBO��
	void blit(CZMat4 &projection);
	
	/// ��������ͼ���ͼ�񣨲�������ǰ���Ƶıʻ���
	CZImage *imageWithSize(CZSize &size, CZColor *backgroundColor);

	/// ���ɵ�ǰ״̬��ͼ��
	CZImage *imageForCurrentState(CZColor *backgroundColor);
	
	/// ����һ���켣�����Ƶ�����
	CZRect paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer = false);
	
	/// ���÷�Χ����render�ķ�Χ���䱣��һ�£�
	void setDimensions(const CZSize &size);
	
	/// ���õ�ǰ����ͼ��
	///
	///		\param idx - ��ǰ��Ҫ�����ͼ�����
	///		\ret	   - ԭ�������ͼ�����
	int setActiveLayer(int idx);
	
	/// ͨ��UID��ȡͼ��
	/// 
	///		\note �����ڸ�UID��ͼ���򷵻�NULL
	CZLayer *layerWithUID(unsigned int uid_);
	
	/// ɾ��ͼ��
	/// 
	///		\param - ��Ҫɾ����ͼ��
	///		\ret   - ԭͼ�����ڵ����
	///		\note ��layer����ס��ʱ���ܱ�ɾ�� 
	int removeLayer(CZLayer *layer);
	
	/// ����ͼ��
	void insertLayer(int idx, CZLayer *layer);
	
	/// ���ͼ��
	/// 
	///		\param layer - ��ӵ�ͼ��
	///		\ret		 - ������ͼ���е����,ʧ�ܷ���-1
	int addLayer(CZLayer *layer);
	
	/// ���ºϲ���ǰͼ��
	/// 
	///		\ret - �Ƿ�ϲ��ɹ�
	bool mergeActiveLayerDown();

	/// �ƶ�ͼ��
	/// 
	///		\param layer - ��Ҫ�ƶ���ͼ��
	///		\param idx	 - �ƶ�����λ��
	bool moveLayer(CZLayer* layer, int idx);

	/// ��ȡ����ͼ��
	std::vector<CZLayer*> & getAllLayers();

	/// ���ͼ��������ͼ���еı�ţ������ڷ��ظ�ֵ
	int indexOfLayers(CZLayer *layer);

	/// ���ü���켣
	void setActivePath(CZPath *path);

	/// ��ȡ����켣
	CZPath* getActivePath();

	/// ���ü���ͼ��
	void setActiveLayer(CZLayer *layer);

	/// ��ȡ����ͼ��
	CZLayer* getActiveLayer();

	/// ��ȡ��Ⱦ��
	CZPaintingRender* getRender();
	
	/// ��ȡ��Χ
	CZSize& getDimensions();
	/// ��ȡ���ƾ���
	CZRect& getBounds();

	/// ʵ��coding�Ľӿ�
	void update(CZDecoder *decoder_, bool deep = false);
	void encode(CZCoder *coder_, bool deep = false);

private:
	static unsigned int paintingNum;///< ���Ƶ���Ŀ

	CZSize dimensions;				///< ���Ʒ�Χ
    CZRect bounds;
	CZPaintingRender *render;		///< ������
	CZPath *ptrActivePath;			///< �����·�����˴���Ϊ����
	CZLayer *ptrActiveLayer;		///< ��ǰ���Ʋ�

	bool flattenMode;
	std::vector<CZLayer*>	layers;	///< ���ƵĻ���

	std::vector<CZColor*>	colors;	
	std::vector<CZBrush*>	brushes;
	std::vector<CZBrush*>	undoBrushes;
	int						strokeCount;	///< �ʻ���Ŀ
	unsigned int			uid;

	CZBrush					*ptrLastBrush;	///< ��һ�ѻ�ˢ

};
#endif