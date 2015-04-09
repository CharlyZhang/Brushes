
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

#include "graphic/CZFbo.h"
#include "graphic/CZShader.h"
#include "graphic/CZTexture.h"
#include "basic/CZImage.h"
#include "CZUtil.h"

#include <map>
#include <string>

#if	USE_OPENGL_ES
	#define GL_GEN_VERTEXARRAY(n,arr)	glGenVertexArraysOES(n, arr)
	#define GL_BIND_VERTEXARRAY(id)		glBindVertexArrayOES(id)
	#define	GL_DEL_VERTEXARRAY(n,arr)	glDeleteVertexArraysOES(n,arr)
#elif	USE_OPENGL
	#define GL_GEN_VERTEXARRAY(n,arr)	glGenVertexArrays(n, arr)
	#define GL_BIND_VERTEXARRAY(id)		glBindVertexArray(id)
	#define	GL_DEL_VERTEXARRAY(n,arr)	glDeleteVertexArrays(n,arr)
#endif


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