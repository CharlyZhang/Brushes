
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
	/// ���û�������
	void setTexture(CZTexture *tex_);
	/// ��ʼFBO��������������棩
	void begin();
	/// ����FBO
	void end();
	/// ��������Ƶ���Ļ
	void showTextureOnScreen(int x,int y,int width_ = 128,int height_ = 128);
private:
	/// ���״̬
	void checkFramebufferStatus();

	int width,height;
	GLuint id;
	GLint preFbo;
	GLuint depthId;				///< ��Ȼ���
	CZTexture *tex;				///< ֻ�����ã������𴴽�������
};

#endif