
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

CZFbo::CZFbo(int width_ /* = 0 */,int height_ /* = 0 */, CZTexture *tex_ /* = NULL */)
{
	width = width_;
	height = height_;
	depthId = 0;
	tex = tex_;

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);	

	if (width_ != 0 && height_ != 0)
	{
		//申请Z BUFFER
		glGenRenderbuffers(1,&depthId);
		glBindRenderbuffer(GL_RENDERBUFFER,depthId);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
		//将深度缓冲与FBO绑定
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthId);
	}
	
	if(tex)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0);
		//check status
		checkFramebufferStatus();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CZFbo::~CZFbo()
{
	glDeleteFramebuffers(1,&id);
	glDeleteRenderbuffers(1,&depthId);
}

/// 设置绘制纹理
void CZFbo::setTexture(CZTexture *tex_)
{
	if(tex_)
	{
		tex = tex_;
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0);
		//check status
		checkFramebufferStatus();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
		std::cerr << "CZFbo::setTexture - Tex is null \n";
}

/// 设置绘制缓冲区
void CZFbo::setRenderBuffer(int w_, int h_, GLenum internalFormat /* = GL_DEPTH_COMPONENT */)
{
	if (depthId)	glDeleteRenderbuffers(1,&depthId);
	
	if (w_ != 0 && h_ != 0)
	{
		//申请Z BUFFER
		glGenRenderbuffers(1,&depthId);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glBindRenderbuffer(GL_RENDERBUFFER,depthId);
		glRenderbufferStorage(GL_RENDERBUFFER,internalFormat,w_,h_);
		//将深度缓冲与FBO绑定
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthId);
		//check status
		checkFramebufferStatus();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
		std::cerr << "CZFbo::setRenderBuffer - width or height is 0 \n";
}

/// 开始FBO
void CZFbo::begin()
{
	if(tex == NULL) 
	{
		std::cerr << "CZFbo::begin - Tex is NULL \n";
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
	if(tex == NULL) return;

	glPopAttrib();
	glBindFramebuffer(GL_FRAMEBUFFER, preFbo);
}

/// 将纹理绘制到屏幕
void CZFbo::showTextureOnScreen( int x,int y,int width_ /*= 128*/,int height_ /*= 128*/)
{
	if(tex == NULL) 
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
void CZFbo::checkFramebufferStatus()
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
		exit(0);
	}
}