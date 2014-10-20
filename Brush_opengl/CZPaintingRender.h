
///  \file CZPaintingRender.h
///  \brief This is the file declare the Class CZPaintingRender.
///
///		����ͼ����Ļ��ơ�
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-14
///  \note


#ifndef _CZPAINTINGRENDER_H_
#define _CZPAINTINGRENDER_H_

#include "CZRender.h"
#include "CZGeometry.h"
#include "CZAffineTransform.h"
#include <map>
#include <string>

class CZLayer;
class CZColor;
class CZBrush;

class CZPaintingRender : public CZRender
{
public:
	CZPaintingRender(const CZSize &size);
	~CZPaintingRender();

	/// ���û�����Ϣ���ı��ڲ�������
	void configure(std::map<std::string,void*> &conf);
	/// ��ʼ���ƣ�ִ�о���Ļ��Ƶ��ã�
	void begin(DrawType type);
	/// ���ýӿڻ���
	void draw(DrawType type, void* data = NULL, unsigned int num = 0);
	/// ����ͼ�����ݲ�����
	CZImage* imageForLastDraw();
	/// ��������
	void end(DrawType type);

	/// ���ͼ���ɫ��/Ũ��/��������
	void clearLayerHCLTexture(CZLayer *layer);
	/// ���ͼ������
	void clearLayerTexture(CZLayer* layer);
	/// ���óɻ��Ƶ�����
	void setRenderToTexture();
	/// �����Ƿ��������
	void setClearBuffer(bool flag);
	/// ������ˢ����
	void changeBrushTex(CZBrush *brush);
	/// ������������С������width,height�Լ�activePaintTexture��
	void resize(const CZSize &size);

private:
	/// ������ɫ��
	void loadShaders();
	/// ���û��ģʽ
	void configureBlendMode(BlendMode mode);
	/// ����quadVAO
	GLuint getQuadVAO();
	/// ��ȡ����������
	CZTexture* getPaintTexture();
	/// ��ȡͼ������
	CZTexture* getLayerTexture(CZLayer* layer);	
	/// ��ȡͼ���ɫ��/Ũ��/��������
	CZTexture* getLayerHCLTexture(CZLayer *layer);
	
	/// ���ƾ���
	void drawRect(const CZRect &rect, const CZAffineTransform &transform);

private:
	std::map<std::string,CZShader*>	shaders;///< ��ɫ��

	CZLayer *ptrRenderingLayer;				///< ��ǰ���Ƶ�ͼ��
	GLuint quadVAO,quadVBO;					///< ���ƾ��ε�VAO��VBO
	CZTexture *activePaintTexture;			///< �����õ�����
	std::map<CZLayer*,CZTexture*> layerTextures;			///< ������������ⷴ����������
	std::map<CZLayer*,CZTexture*> layerHueChromaLumaTex;	///< �����ɫ��/Ũ��/�����������ⷴ����������

	bool clearBuffer;						///< �Ƿ��ڻ���ǰ�������
	CZTexture *brushTex;					///< ��ˢ����
	CZColor *ptrClearColor;					///< ������ɫ
};
#endif
