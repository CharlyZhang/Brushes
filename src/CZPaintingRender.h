
///  \file CZPaintingRender.h
///  \brief This is the file declare the Class CZPaintingRender.
///
///		����ͼ����Ļ��ƣ�����CZPainting��CZLayer�����л��ơ�
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-14
///  \note		Ĭ�����ǻ��Ƶ�activePaintTexture������Ҫʱ���Ե��������߻��ƣ�����Ҫ�ָ���


#ifndef _CZPAINTINGRENDER_H_
#define _CZPAINTINGRENDER_H_

#include "CZRender.h"
#include "basic/CZ2DPoint.h"
#include "basic/CZRect.h"
#include "basic/CZSize.h"
#include "basic/CZAffineTransform.h"
#include "basic/CZMat4.h"
#include <map>
#include <string>

class CZLayer;
class CZPainting;
class CZColor;
class CZBrush;
class CZColorBalance;
class CZHueSaturation;
class CZPath;
class CZRandom;

class CZPaintingRender : public CZRender
{
public:
	CZPaintingRender(const CZSize &size);
	~CZPaintingRender();

	/// ����ĳ��������ͼ������Ļ��- for CZCanvas
	void drawViewInRect(/*const CZRect &rect*/);

	/// ���Ƶ�ǰ״̬��ͼ�񣨲��������ƹ켣��	- for CZPainting
	CZImage *drawPaintingImage(CZSize & size, CZColor *bgColor);
	/// ���ɵ�ǰ����״̬��ͼ��				- for CZPainting
	CZImage *drawPaintingCurrentState(CZColor *bgColor);
	/// ����һ�ʹ켣��������				- for CZPainting
	CZRect drawPaintingStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer);

	/// ���ɵ�ǰͼ��ͼ��				- for CZLayer
	CZImage *drawLayerInRect(const CZRect &rect);
	/// ����Layer�Ĳ����켣			- for CZLayer
	void drawLayerWithEraseMask(CZMat4 &projection);
	/// ����Layer�Ļ滭�켣			- for CZLayer
	void drawLayerWithMask(CZMat4 &projection,CZColor *bgColor);
	/// ��Layer���������				- for CZLayer
	void drawLayer(CZMat4 &projection);
	/// ��Layer������ת�������			- for CZLayer
	void drawLayerWithTransform(CZMat4 &projection, const CZAffineTransform &transform);
	/// ��Layer���������ɫ���������	- for CZLayer
	void drawLayerWithcolorBalance(CZMat4 &projection, CZColorBalance *colorBalance);
	/// ��Layer�������ɫ�ʵ��������	- for CZLayer
	void drawLayerWithhueSaturation(CZMat4 &projection, CZHueSaturation *hueSaturation);
	/// �����ƵĹ켣�ϲ�����ǰͼ��		- for CZLayer
	void composeActivePaintTexture(CZColor &color,bool erase);

	/// ���ͼ���ɫ��/Ũ��/��������
	void clearLayerHCLTexture(CZLayer *layer);
	/// ���ͼ������
	void clearLayerTexture(CZLayer* layer);
	/// ������������С������width,height�Լ�activePaintTexture��
	void resize(const CZSize &size);
	/// ������ˢ����
	void changeBrushTex(CZBrush *brush);
	/// ��ȡ����������
	CZTexture* getPaintTexture();

private:
	/// ������ɫ��
	void loadShaders();
	/// ���û��ģʽ
	void configureBlendMode(BlendMode mode);
	/// ����quadVAO
	unsigned int getQuadVAO();
	/// ��ȡͼ������
	CZTexture* getLayerTexture(CZLayer* layer);	
	/// ��ȡͼ���ɫ��/Ũ��/��������
	CZTexture* getLayerHCLTexture(CZLayer *layer);
	
	/// ���ƾ���
	void drawRect(const CZRect &rect, const CZAffineTransform &transform);
    
    /// draw quad
    void drawQuad();

public:
	CZPainting	*ptrPainting;				///< ��ǰ����
	CZLayer		*ptrLayer;					///< ��ǰ���Ƶ�ͼ�㣬������ί��ʱ�����ڴ�����
private:
	std::map<std::string,CZShader*>	shaders;///< ��ɫ��
	unsigned int quadVAO,quadVBO;					///< ���ƾ��ε�VAO��VBO
	CZTexture *activePaintTexture;			///< �����õ�����	- painting
	CZMat4					projectionMat;	///< ͶӰ����
	std::map<unsigned int,CZTexture*> layerTextures;			///< ������������ⷴ����������
	std::map<unsigned int,CZTexture*> layerHueChromaLumaTex;	///< �����ɫ��/Ũ��/�����������ⷴ����������
public: ///! for debug
	CZTexture *brushTex;					///< ��ˢ����
};
#endif
