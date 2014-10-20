
///  \file CZPathRender.h
///  \brief This is the file declare the class CZPathRender.
///
///	
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-13
///  \note


#ifndef _CZPATHRENDER_H_
#define _CZPATHRENDER_H_

#include "CZRender.h"
#include "CZShader.h"
#include "CZUtil.h"

/// 定义最终绘制数据格式
typedef struct 
{
	GLfloat     x, y;
	GLfloat     s, t;
	GLfloat     a;
} vertexData;

class CZPathRender :public CZRender
{
public:
	/// 初始化状态
	void init()
	{
		// configure some default GL state
		glDisable(GL_DITHER);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}
	void configTexture(CZTexture *tex)
	{
		width = tex->width;
		height = tex->height;

		fbo.setTexture(tex);

		fbo.begin();

		glClearColor(.0f, .0f, .0f, .0f);	
		glClear(GL_COLOR_BUFFER_BIT);

		fbo.end();
	}
	/// 配置绘制区域
	void configure(int w, int h)
	{
		width  = w;
		height = h;

		//~ 设置context

		fbo.setColorRenderBuffer(width,height);

		/// 设置投影环境
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, (GLfloat) width, 0.0f, 
			(GLfloat) height, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	}
	/// 开始
	void begin()
	{
		fbo.begin();

		//glClearColor(.0f, .0f, .0f, .0f);	
		//glClear(GL_COLOR_BUFFER_BIT);

		{	/// 以防其他地方破坏了上下文状态
			glEnable(GL_BLEND);							
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		}

		glColor4f(1.0,1.0,1.0,0.5);

		ptrShader->begin();
	};

	/// 结束
	void end()
	{
		ptrShader->end();
		fbo.end();
	};

	/// 绘制轨迹数据（利用图形接口）
	void drawPathData(vertexData *data, unsigned int n)
	{
	#if USE_OPENGL_ES
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), &data[0].x);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].s);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].a);
		glEnableVertexAttribArray(2);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, n);
	#endif

	#if USE_OPENGL
		/*
		glEnableClientState (GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT , sizeof(vertexData), &data[0].x); 
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT, sizeof(vertexData), &data[0].s);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].a);
		*/
		GLuint mVertexBufferObject, mTexCoordBufferObject, mAttributeBufferObject;
		// 装载顶点
		glGenBuffers(1, &mVertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &data[0].x, GL_STREAM_DRAW);
		// 装载纹理
		glGenBuffers(1, &mTexCoordBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
		glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &data[0].s, GL_STREAM_DRAW);
		// 装载属性
		glGenBuffers(1, &mAttributeBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, mAttributeBufferObject);
		glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &data[0].a, GL_STREAM_DRAW);

		// 绑定顶点
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,sizeof(vertexData),0);
		// 绑定纹理
		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT,sizeof(vertexData),0);
		// 绑定属性
		glBindBuffer(GL_ARRAY_BUFFER, mAttributeBufferObject);
		GLuint alphaLoc = ptrShader->getAttributeLocation("alpha");
		glEnableVertexAttribArray(alphaLoc);
		glVertexAttribPointer(alphaLoc, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), NULL);

		/// 绘制
		glDrawArrays(GL_TRIANGLE_STRIP,0,n);

		glDisableVertexAttribArray(alphaLoc);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/// 消除
		glDeleteBuffers(1, &mVertexBufferObject);
		glDeleteBuffers(1, &mTexCoordBufferObject);
		glDeleteBuffers(1, &mAttributeBufferObject);

	#endif
		CZCheckGLError();
	}

	CZShader *ptrShader;

};

#endif