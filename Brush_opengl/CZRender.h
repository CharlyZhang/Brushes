
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

class CZRender
{
public:
	CZRender(){
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
	/// 初始化绘制状态
	virtual void init() = 0;
	/// 配置绘制区域
	virtual void configure(int w, int h) = 0;
	/// 开始绘制
	virtual void begin() = 0;
	/// 结束绘制
	virtual void end() = 0;

protected:
	int width, height;
	CZFbo fbo;

};


#endif