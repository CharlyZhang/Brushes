
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
#include "CZFbo.h"
#include "CZCanvasRender.h"
#include "CZTexture.h"
#include <map>
#include <string>

class CZPainting
{
public:
	CZPainting(const CZSize &size);
	~CZPainting();
	/// 绘制一条轨迹
	CZRect paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer = false);
private:
	/// 配置笔刷
	///	
	///		配置好绘制用笔刷的纹理以及Shader。由于CZPainting会涉及到多种笔刷，所以在作画前需要将当前用的笔刷配置好。
	/// 
	///		/param brush_	- 配置的画刷
	///		/return			- 该画刷对应的Shader
	CZShader* configureBrush(CZBrush *brush_);
	/// 获取相应的Shader（同时设定当前GL上下文）
	CZShader* getShader(std::string shaderName);
	/// 获取绘制用纹理
	CZTexture* getPaintTexture();
	/// 载入Shader
	void loadShaders();

private:
	CZFbo *fbo;						///< 绘制用的FBO
	CZFbo reusableFBo;				///< 重复使用的FBO，绘制到纹理
	CZPath *ptrActivePath;			///< 激活的路径，此处仅为引用
	CZTexture *activePaintTexture;	///< 绘制用的纹理
	CZSize dimensions;				///< 绘制范围

	std::vector<CZColor*>	colors;	
	std::vector<CZBrush*>	brushes;
	std::vector<CZBrush*>	undoBrushes;
	int						strokeCount;	///< 笔画数目
	unsigned int			uid;
	

	std::map<std::string,CZShader *> shaders;	///< 绘制会用到的各种shader
	CZCanvasRender			render;

};
#endif