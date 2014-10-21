
///  \file CZPaintingRender.h
///  \brief This is the file declare the Class CZPaintingRender.
///
///		负责画图区域的绘制。
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

	/// 配置绘制信息（改变内部变量）
	void configure(std::map<std::string,void*> &conf);
	/// 开始绘制（执行具体的绘制调用）
	void begin(DrawType type);
	/// 调用接口绘制
	void draw(DrawType type, void* data = NULL, unsigned int num = 0);
	/// 生成图像数据并返回
	CZImage* imageForLastDraw();
	/// 结束绘制
	void end(DrawType type);

	/// 清除图层的色调/浓度/亮度纹理
	void clearLayerHCLTexture(CZLayer *layer);
	/// 清除图层纹理
	void clearLayerTexture(CZLayer* layer);
	/// 设置成绘制到纹理
	void setRenderToTexture();
	/// 设置是否清楚缓存
	void setClearBuffer(bool flag);
	/// 更换笔刷纹理
	void changeBrushTex(CZBrush *brush);
	/// 调整绘制器大小（调整width,height以及activePaintTexture）
	void resize(const CZSize &size);

private:
	/// 载入着色器
	void loadShaders();
	/// 配置混合模式
	void configureBlendMode(BlendMode mode);
	/// 返回quadVAO
	GLuint getQuadVAO();
	/// 获取绘制用纹理
	CZTexture* getPaintTexture();
	/// 获取图层纹理
	CZTexture* getLayerTexture(CZLayer* layer);	
	/// 获取图层的色调/浓度/亮度纹理
	CZTexture* getLayerHCLTexture(CZLayer *layer);
	
	/// 绘制矩形
	void drawRect(const CZRect &rect, const CZAffineTransform &transform);

private:
	std::map<std::string,CZShader*>	shaders;///< 着色器

	CZLayer *ptrRenderingLayer;				///< 当前绘制的图层
	GLuint quadVAO,quadVBO;					///< 绘制矩形的VAO、VBO
	CZTexture *activePaintTexture;			///< 绘制用的纹理
	std::map<CZLayer*,CZTexture*> layerTextures;			///< 各层的纹理（避免反复生成纹理）
	std::map<CZLayer*,CZTexture*> layerHueChromaLumaTex;	///< 各层的色调/浓度/亮度纹理（避免反复生成纹理）

	bool clearBuffer;						///< 是否在绘制前清除缓存
	CZTexture *brushTex;					///< 笔刷纹理
	CZColor *ptrClearColor;					///< 背景颜色
};
#endif
