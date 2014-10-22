
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
#include "CZGeometry.h"
#include "CZAffineTransform.h"
#include "CZMat4.h"
#include "CZCommon.h"
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


	/// ���ɵ�ǰ����״̬��ͼ��	- for CZPainting
	CZImage *drawPaintingCurrentState(CZColor *bgColor, std::vector<CZLayer*> &layers);
	/// ����һ�ʹ켣
	CZRect drawPaintingStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer);

	/// ���ɵ�ǰͼ��ͼ��		- for CZLayer
	CZImage *drawLayerInRect(const CZRect &rect);
	/// ����Layer�Ĳ����켣	- for CZLayer
	void drawLayerWithEraseMask(CZMat4 &projection);
	/// ����Layer�Ļ滭�켣	- for CZLayer
	void drawLayerWithMask(CZMat4 &projection,CZColor *bgColor);
	/// ��Layer���������	- for CZLayer
	void drawLayer(CZMat4 &projection);
	/// ��Layer������ת�������	- for CZLayer
	void drawLayerWithTransform(CZMat4 &projection, const CZAffineTransform &transform);
	/// ��Layer���������ɫ���������	- for CZLayer
	void drawLayerWithcolorBalance(CZMat4 &projection, CZColorBalance *colorBalance);
	/// ��Layer�������ɫ�ʵ��������	- for CZLayer
	void drawLayerWithhueSaturation(CZMat4 &projection, CZHueSaturation *hueSaturation);

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
	GLuint getQuadVAO();
	/// ��ȡͼ������
	CZTexture* getLayerTexture(CZLayer* layer);	
	/// ��ȡͼ���ɫ��/Ũ��/��������
	CZTexture* getLayerHCLTexture(CZLayer *layer);
	
	/// ���ƾ���
	void drawRect(const CZRect &rect, const CZAffineTransform &transform);

public:
	CZPainting	*ptrPainting;				///< ��ǰ����
	CZLayer		*ptrLayer;					///< ��ǰ���Ƶ�ͼ�㣬������ί��ʱ�����ڴ�����
private:
	std::map<std::string,CZShader*>	shaders;///< ��ɫ��
	GLuint quadVAO,quadVBO;					///< ���ƾ��ε�VAO��VBO
	CZTexture *activePaintTexture;			///< �����õ�����
	CZMat4					projectionMat;	///< ͶӰ����
	std::map<CZLayer*,CZTexture*> layerTextures;			///< ������������ⷴ����������
	std::map<CZLayer*,CZTexture*> layerHueChromaLumaTex;	///< �����ɫ��/Ũ��/�����������ⷴ����������

	CZTexture *brushTex;					///< ��ˢ����
};
#endif
