
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

#if USE_OPENGL_ES
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

struct CZRender::Impl
{
#if USE_OPENGL
	int context;			///< !模拟一下
#elif USE_OPENGL_ES
    EAGLContext *context;
    Impl(){ context = NULL;}
    ~Impl(){
        if(context) [context release ];
        context = NULL;
    }
#endif
};

CZRender::CZRender()
{
    imp = new Impl();
    
#if USE_OPENGL_ES
	imp->context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

	if (!imp->context || ![EAGLContext setCurrentContext:imp->context]) {
        NSLog(@"Cannot initialize render - context error");
	}
#endif
    fbo = new CZFbo;
};
CZRender::~CZRender()
{
    setContext();
    if(fbo) {   delete fbo; fbo = NULL; }
#if USE_OPENGL_ES
    delete imp;
    imp = NULL;
#endif
};

/// 设置当前上下文
void CZRender::setContext()
{
#if USE_OPENGL_ES
	//如果不存在，则生成，并设置为当前
    if (imp->context) {
        [EAGLContext setCurrentContext:imp->context];
    }
#endif
    std::cout << "context " << imp->context << std::endl;
};

/// change context
void CZRender::changeContext(void *_ctx)
{
#if USE_OPENGL_ES
    imp->context = (__bridge EAGLContext *) _ctx;
    setContext();
#endif
}

void* CZRender::getContext()
{
#if USE_OPENGL_ES
    if (imp) {
        return (void*) imp->context;
    }
#endif
    return NULL;
}

/// 绘制轨迹数据
void CZRender::drawPathData(unsigned int n, vertexData *data)
{
	#if USE_OPENGL_ES

    GLuint _vertexArray,_vertexBuffer;
    glGenVertexArraysOES(1, &_vertexArray);
    glBindVertexArrayOES(_vertexArray);
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData)*n, data, GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5*sizeof(GLfloat), (void*)8);
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, 5*sizeof(GLfloat), (void*)16);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, n);
    glBindVertexArrayOES(0);
    
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteVertexArraysOES(1, &_vertexArray);


//
//		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), &data[0].x);
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].s);
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].a);
//		glEnableVertexAttribArray(2);
	
    //	glDrawArrays(GL_TRIANGLE_STRIP, 0, n);
    CZCheckGLError();
    
    
	#endif

	#if USE_OPENGL
		
	/*	// 对于opengl 顶点位置必须通过以下方式导入
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