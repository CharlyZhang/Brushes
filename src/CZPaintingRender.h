
///  \file CZPaintingRender.h
///  \brief This is the file declare the Class CZPaintingRender.
///
///		负责画图区域的绘制，包括CZPainting和CZLayer的所有绘制。
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-14
///  \note		默认认是绘制到activePaintTexture纹理，需要时可以调整到离线绘制，但需要恢复。


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

	/// 绘制某区域内视图（到屏幕）- for CZCanvas
	void drawViewInRect(/*const CZRect &rect*/);

	/// 绘制当前状态的图像（不包含绘制轨迹）	- for CZPainting
	CZImage *drawPaintingImage(CZSize & size, CZColor *bgColor);
	/// 生成当前绘制状态的图像				- for CZPainting
	CZImage *drawPaintingCurrentState(CZColor *bgColor);
	/// 绘制一笔轨迹（到纹理）				- for CZPainting
	CZRect drawPaintingStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer);

	/// 生成当前图层图像				- for CZLayer
	CZImage *drawLayerInRect(const CZRect &rect);
	/// 绘制Layer的擦除轨迹			- for CZLayer
	void drawLayerWithEraseMask(CZMat4 &projection);
	/// 绘制Layer的绘画轨迹			- for CZLayer
	void drawLayerWithMask(CZMat4 &projection,CZColor *bgColor);
	/// 将Layer的纹理绘制				- for CZLayer
	void drawLayer(CZMat4 &projection);
	/// 将Layer的纹理转换后绘制			- for CZLayer
	void drawLayerWithTransform(CZMat4 &projection, const CZAffineTransform &transform);
	/// 将Layer的纹理带颜色调整后绘制	- for CZLayer
	void drawLayerWithcolorBalance(CZMat4 &projection, CZColorBalance *colorBalance);
	/// 将Layer的纹理带色彩调整后绘制	- for CZLayer
	void drawLayerWithhueSaturation(CZMat4 &projection, CZHueSaturation *hueSaturation);
	/// 将绘制的轨迹合并到当前图层		- for CZLayer
	void composeActivePaintTexture(CZColor &color,bool erase);

	/// 清除图层的色调/浓度/亮度纹理
	void clearLayerHCLTexture(CZLayer *layer);
	/// 清除图层纹理
	void clearLayerTexture(CZLayer* layer);
	/// 调整绘制器大小（调整width,height以及activePaintTexture）
	void resize(const CZSize &size);
	/// 更换笔刷纹理
	void changeBrushTex(CZBrush *brush);
	/// 获取绘制用纹理
	CZTexture* getPaintTexture();

private:
	/// 载入着色器
	void loadShaders();
	/// 配置混合模式
	void configureBlendMode(BlendMode mode);
	/// 返回quadVAO
	unsigned int getQuadVAO();
	/// 获取图层纹理
	CZTexture* getLayerTexture(CZLayer* layer);	
	/// 获取图层的色调/浓度/亮度纹理
	CZTexture* getLayerHCLTexture(CZLayer *layer);
	
	/// 绘制矩形
	void drawRect(const CZRect &rect, const CZAffineTransform &transform);
    
    /// draw quad
    void drawQuad();

public:
	CZPainting	*ptrPainting;				///< 当前绘制
	CZLayer		*ptrLayer;					///< 当前绘制的图层，在受理委托时可用于传数据
private:
	std::map<std::string,CZShader*>	shaders;///< 着色器
	unsigned int quadVAO,quadVBO;					///< 绘制矩形的VAO、VBO
	CZTexture *activePaintTexture;			///< 绘制用的纹理	- painting
	CZMat4					projectionMat;	///< 投影矩阵
	std::map<unsigned int,CZTexture*> layerTextures;			///< 各层的纹理（避免反复生成纹理）
	std::map<unsigned int,CZTexture*> layerHueChromaLumaTex;	///< 各层的色调/浓度/亮度纹理（避免反复生成纹理）
public: ///! for debug
	CZTexture *brushTex;					///< 笔刷纹理
};
#endif
