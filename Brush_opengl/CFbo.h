#pragma once

#ifndef __CFBO_H__
#define __CFBO_H__
#include<stdio.h>
#include<windows.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include "gl/glext.h"


#ifndef GLEE_EXTENTION
#define GLEE_EXTENTION
#define GL_COLOR_ATTACHMENT0_EXT                           0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT                           0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT                           0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT                           0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT                           0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT                           0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT                           0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT                           0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT                           0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT                           0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT                          0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT                          0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT                          0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT                          0x8CED
#define GL_COLOR_ATTACHMENT14_EXT                          0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT                          0x8CEF
#define GL_RGBA32F_ARB                                     0x8814
#define GL_FRAMEBUFFER_EXT                                 0x8D40

#endif

#if 0
#define GLEE_H_DEFINED_glDeleteFramebuffersEXT
  typedef void (APIENTRYP GLEEPFNGLDELETEFRAMEBUFFERSEXTPROC) (GLsizei n, const GLuint * framebuffers);
  GLEE_EXTERN GLEEPFNGLDELETEFRAMEBUFFERSEXTPROC GLeeFuncPtr_glDeleteFramebuffersEXT;
  #define glDeleteFramebuffersEXT GLeeFuncPtr_glDeleteFramebuffersEXT
#endif
static const GLenum COLOR_ATTACHMENT[] = {
	GL_COLOR_ATTACHMENT0_EXT,
	GL_COLOR_ATTACHMENT1_EXT,
	GL_COLOR_ATTACHMENT2_EXT,
	GL_COLOR_ATTACHMENT3_EXT,
	GL_COLOR_ATTACHMENT4_EXT,
	GL_COLOR_ATTACHMENT5_EXT,
	GL_COLOR_ATTACHMENT6_EXT,
	GL_COLOR_ATTACHMENT7_EXT,
	GL_COLOR_ATTACHMENT8_EXT,
	GL_COLOR_ATTACHMENT9_EXT,
	GL_COLOR_ATTACHMENT10_EXT,
	GL_COLOR_ATTACHMENT11_EXT,
	GL_COLOR_ATTACHMENT12_EXT,
	GL_COLOR_ATTACHMENT13_EXT,
	GL_COLOR_ATTACHMENT14_EXT,
	GL_COLOR_ATTACHMENT15_EXT
};

////最多支持十六张纹理
class CFbo
{
public:
	enum texType
	{
		TEXTURE,
		SHADOWMAP,
		TEXARRAY
	} fboType;
	CFbo(int _bufferWidth,int _bufferHeight,int _numBuffers=1);
	~CFbo();
	void init(texType tex = TEXTURE);
	void init_Shadow();
	void init_TexArray(int layerNum = 1);
	void init_ZBufferFree();
	void init_Layered_ZBFree(GLint internalFormat = /*GL_ALPHA32F_ARB*/GL_RGBA32F_ARB);
	void init_Stencil();
	GLuint getFboID();
	GLuint getTexID(int texIdx=0);
	GLuint getTexID_pingpong();
	int getWidth() { return bufferWidth; }
	int getHeight() { return bufferHeight; }
	void begin(int colorAttachment=0, int numDrawBuffers=1,int texArrayLayer = 0);
	void begin_NoClear(int colorAttachment=0, int numDrawBuffers=1);
	void begin_clearDepthWhite(int colorAttachment=0, int numDrawBuffers=1, int depthAttachment = 1);
	void begin_pingpong();
	void end();
	void end_pingpong();
	void bind();
	void unbind();
	void draw(int x, int y, int width, int height, int colorAttachment = 0, int numDrawBuffers = 1);
	void showDepthOnScreen(int x,int y,int width = 128,int height = 128,GLenum type = GL_TEXTURE_2D);//using for DEBUG
	void showTextureOnScreen(int x,int y,int width = 128,int height = 128,GLuint tID = 0,GLenum type = GL_TEXTURE_2D);//using for DEBUG
	void showTextureOnScreen_Replace(int x,int y,int width = 128,int height = 128,GLuint tID = 0,GLenum type = GL_TEXTURE_2D);//using for DEBUG
	void readPixel(int tIdx,int x, int y, int width, int height);
	GLuint depthID,stencilID;
	GLuint texArray;
private:
	void CheckFramebufferStatus();

	int bufferWidth,bufferHeight;
	GLuint fboID;
	GLint preFbo;
	GLuint *texID;

	int renderTexId;		///< 记录当前的绘制点，做pingpong 渲染时用
public:
	int numBuffers;
};
#endif