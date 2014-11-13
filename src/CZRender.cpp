
///  \file CZRender.cpp
///  \brief This is the file implements the Class CZRender.
///
///		这里用来绘制，每个render都有单独的context,与绘制相关。
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-22
///  \note

#include "CZRender.h"


CZRender::CZRender()
{
	/*
	self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

	if (!context || ![EAGLContext setCurrentContext:context]) {
	return nil;
	}
	*/
};
CZRender::~CZRender()
{
};
	
/// 设置当前上下文
void CZRender::setContext()
{
	//如果不存在，则生成，并设置为当前
};

/// 绘制轨迹数据
void CZRender::drawPathData(unsigned int n, vertexData *data)
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
		
	/*	// 对于opengl属性数组形式， 顶点位置必须通过以下方式导入
		glEnableClientState (GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT , sizeof(vertexData), &data[0].x);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].s);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].a);
		glEnableVertexAttribArray(2);

		/// 绘制
		glDrawArrays(GL_TRIANGLE_STRIP,0,n);
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
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(vertexData),0);
		// 绑定纹理
		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,2,GL_FLOAT, GL_TRUE, sizeof(vertexData),0);
		// 绑定属性
		glBindBuffer(GL_ARRAY_BUFFER, mAttributeBufferObject);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), NULL);

		/// 绘制
		glDrawArrays(GL_TRIANGLE_STRIP,0,n);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/// 消除
		glDeleteBuffers(1, &mVertexBufferObject);
		glDeleteBuffers(1, &mTexCoordBufferObject);
		glDeleteBuffers(1, &mAttributeBufferObject);
		
	#endif
		CZCheckGLError();

}