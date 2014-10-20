
///  \file CZRender.h
///  \brief This is the file declare the Class CZRender.
///
///		这里用来绘制，每个render都有单独的context,与绘制相关。
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-10
///  \note

#ifndef _CZRENDER_H_
#define _CZRENDER_H_

#include "CZFbo.h"
#include "CZShader.h"
#include "CZTexture.h"
#include "CZImage.h"
#include "GL/glut.h"
#include <map>
#include <string>

/// 定义最终绘制数据格式
typedef struct 
{
	GLfloat     x, y;
	GLfloat     s, t;
	GLfloat     a;
} vertexData;

class CZRender
{
public:
	enum DrawType		///< 绘制的类型
	{
		//-- Common --
		DrawGeneral,		///< 通用
		DrawStamp,			///< 画刷纹理
		kDrawPath,			///< 画轨迹

		//-- CZPaintingRender-- 
		//	used in CZLayer
		kPaintingErase,				///< 擦除轨迹
		kPaintingMask,				///< 添加轨迹
		kPaintingBlit,				///< 展现图层
		kPaintingBlitTransform,		///< 位置变换后展现图层
		kPaintingBlitColorBalance,	///< 颜色调整后展现图层
		kPaintingBlitHueSaturation,	///< 饱和度色调调整后展现图层
		kPaingtingImageData,		///< 获取图像数据
		//	used in CZPainting
		kPaintingStroke,			///< 画一笔轨迹
		kPaintingImageForCurrent	///< 绘制当前状态图像
	};
	enum BlendMode		///< 混合模式
	{
		kBlendModeNormal,
		kBlendModeMultiply,
		kBlendModeScreen,
		kBlendModeExclusion
	};

	CZRender()
	{
		/*
		self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

		if (!context || ![EAGLContext setCurrentContext:context]) {
		return nil;
		}
		*/
	};
	~CZRender()
	{
	};
	/// 配置绘制信息（改变内部变量）
	virtual void configure(std::map<std::string,void*> &conf) = 0;
	/// 开始绘制（执行具体的绘制调用）
	virtual void begin(DrawType type) = 0;
	/// 调用接口绘制
	virtual void draw(DrawType type, void* data = NULL, unsigned int num = 0) = 0;
	/// 生成图像数据并返回
	virtual CZImage* imageForLastDraw() = 0;
	/// 结束绘制
	virtual void end(DrawType type) = 0;

public:
	int width, height;
	CZFbo fbo;
	int context;			///< !模拟一下
};


#endif