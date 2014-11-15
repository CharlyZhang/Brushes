
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
#include "CZUtil.h"

#if USE_OPENGL
#include "GL/glut.h"
#endif

#include <map>
#include <string>

/// 定义CZPath最终绘制数据格式
typedef struct 
{
	float     x, y;
	float     s, t;
	float     a;
} vertexData;

class CZRender
{
public:
	enum BlendMode		///< 混合模式
	{
		kBlendModeNormal,
		kBlendModeMultiply,
		kBlendModeScreen,
		kBlendModeExclusion
	};

	CZRender();
	~CZRender();

	/// 设置当前上下文
	void setContext();
    
    void changeContext(void *_ctx);
    
    void* getContext();

	/// 绘制轨迹数据
	void drawPathData(unsigned int n, vertexData *data);
public:
	int width, height;
	CZFbo *fbo;             ///< ! should be as a pointer, for it has to be created after context creation
    
private:
#if USE_OPENGL
	int context;			///< !模拟一下
#endif
    struct  Impl;
    Impl *imp;
};


#endif