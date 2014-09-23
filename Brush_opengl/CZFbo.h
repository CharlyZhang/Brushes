
///  \file CZFbo.h
///  \brief This is the file implement the Class CZFbo.
///
///		This file includes FBO. It should be used with CZTexuture.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-23
///  \note

#ifndef _CZFBO_H_
#define _CZFBO_H_

#include "CZTexture.h"
#include "GL/glew.h"
#include <iostream>

class CZFbo
{
public:
	CZFbo(int width_,int height_, CZTexture *tex_ = NULL);
	~CZFbo();
	/// 设置绘制纹理
	void setTexture(CZTexture *tex_);
	/// 开始FBO（不负责清除缓存）
	void begin();
	/// 结束FBO
	void end();
	/// 将纹理绘制到屏幕
	void showTextureOnScreen(int x,int y,int width_ = 128,int height_ = 128);
private:
	/// 检查状态
	void checkFramebufferStatus();

	int width,height;
	GLuint id;
	GLint preFbo;
	GLuint depthId;				///< 深度缓存
	CZTexture *tex;				///< 只是引用，不负责创建和销毁
};

#endif