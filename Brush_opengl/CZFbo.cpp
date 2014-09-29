
///  \file CZFbo.cpp
///  \brief This is the file implement the Class CZFbo.
///
///		This file includes FBO. It should be used with CZTexuture.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-23
///  \note

#include "CZFbo.h"

CZFbo::CZFbo(CZTexture *tex_ /* = NULL */)
{
	width = 0;
	height = 0;
	renderId = 0;
	tex = tex_;
	isReady = false;

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);	
	
	if(tex)
	{
		width = tex->width;
		height = tex->height;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0);
		//check status
		if (GL_FRAMEBUFFER_COMPLETE == checkFramebufferStatus()) isReady = true;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CZFbo::CZFbo(int width_,int height_)
{
	width = width_;
	height = height_;
	renderId = 0;
	tex = NULL;
	isReady = false;

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);	

	//申请绘制缓冲区
	glGenRenderbuffers(1,&renderId);
	glBindRenderbuffer(GL_RENDERBUFFER,renderId);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
	//将颜色绘制缓冲与FBO绑定
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,renderId);
	//check status
	if (GL_FRAMEBUFFER_COMPLETE == checkFramebufferStatus()) isReady = true;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CZFbo::~CZFbo()
{
	glDeleteFramebuffers(1,&id);
	if(renderId) glDeleteRenderbuffers(1,&renderId);
}

/// 设置绘制纹理
void CZFbo::setTexture(CZTexture *tex_)
{
	if(tex_)
	{
		tex = tex_;
		width = tex->width;
		height = tex->height;
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0);
		//check status
		if (GL_FRAMEBUFFER_COMPLETE == checkFramebufferStatus()) isReady = true;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
		std::cerr << "CZFbo::setTexture - Tex is null \n";
}

/// 设置绘制缓冲区
void CZFbo::setColorRenderBuffer(int w_, int h_)
{
	if (renderId)	glDeleteRenderbuffers(1,&renderId);
	width = w_;
	height = h_;

	glBindFramebuffer(GL_FRAMEBUFFER, id);
	//申请绘制缓冲区
	glGenRenderbuffers(1,&renderId);
	glBindRenderbuffer(GL_RENDERBUFFER,renderId);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,width,height);
	//将颜色绘制缓冲与FBO绑定
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,renderId);
	//check status
	if (GL_FRAMEBUFFER_COMPLETE == checkFramebufferStatus()) isReady = true;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// 开始FBO
void CZFbo::begin()
{
	if(!isReady) 
	{
		std::cerr << "CZFbo::begin - The FBO is not ready \n";
		return;
	}

	glGetIntegerv(GL_FRAMEBUFFER_BINDING,&preFbo);//获取当前的FBO，用于还原
	glBindFramebuffer(GL_FRAMEBUFFER, id); 
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glPushAttrib(/*GL_ALL_ATTRIB_BITS*/GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,width,height);
}

/// 结束FBO
void CZFbo::end()
{
	if(!isReady) return;

	glPopAttrib();
	glBindFramebuffer(GL_FRAMEBUFFER, preFbo);
}

/// 将纹理绘制到屏幕
void CZFbo::showTextureOnScreen( int x,int y,int width_ /*= 128*/,int height_ /*= 128*/)
{
	if(tex == NULL || !isReady) 
	{
		std::cerr << "CZFbo::showTextureOnScreen - Tex is NULL \n";
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(x,y,width_,height_);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D,tex->id);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  0.0f);
	glEnd();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

/// 检查状态
int CZFbo::checkFramebufferStatus()
{
	int status;
	status = (GLenum) glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status) 
	{
	case GL_FRAMEBUFFER_COMPLETE:
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		std::cout << "Unsupported framebuffer format\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cout << "Framebuffer incomplete, missing attachment\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cout << "Framebuffer incomplete, incomplete attachment\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		std::cout << "Framebuffer incomplete, attached images must have same dimensions\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		std::cout << "Framebuffer incomplete, attached images must have same format\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cout << "Framebuffer incomplete, missing draw buffer\n";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		std::cout << "Framebuffer incomplete, missing read buffer\n";
		break;
	default:
		std::cout << "Framebuffer incomplete, unknown error\n";
		break;
	}

	return status;
}