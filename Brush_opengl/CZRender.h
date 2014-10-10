
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
	~CZRender(){};
	/// 初始化
	virtual void init() = 0;
	/// 开始
	virtual void begin() = 0;
	/// 结束
	virtual void end() = 0;

	CZFbo fbo;

};


#endif