
///  \file CZStampRender.cpp
///  \brief This is the file implements the Class CZStampRender.
///
///		这里用来绘制笔刷图形的绘制器，是个单例。
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note

#include "CZStampRender.h"

/// 配置绘制信息（改变内部变量）
void CZStampRender::configure(std::map<std::string,void*> &conf)
{
	int *pW = (int*)conf["width"];
	int *pH = (int*)conf["height"];
	width  = *pW;
	height = *pH;

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
void CZStampRender::begin(DrawType type)
{
	fbo.begin();

	glClearColor(.0f, .0f, .0f, .0f);	
	glClear(GL_COLOR_BUFFER_BIT);

	{	/// 以防其他地方破坏了上下文状态
		glEnable(GL_BLEND);							
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	glColor4f(1.0,1.0,1.0,0.5);

};

/// 绘制
void CZStampRender::draw(DrawType type,void *data /*= NULL*/, unsigned int num /*= 0*/) 
{
}

/// 结束
void CZStampRender::end(DrawType type)
{
	fbo.end();
};

/// 生成图像数据并返回
CZImage* CZStampRender::imageForLastDraw()
{
	return NULL;
}

/// 
CZStampRender::CZStampRender()
{
	// configure some default GL state
	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}  

/// 绘制螺旋纹
void CZStampRender::drawSpiralData(std::vector<CZ2DPoint> &points)
{
#if USE_OPENGL
	//glEnable(GL_LINE_SMOOTH);		///< 个人感觉还是不启用抗锯齿来得好
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	GLfloat w = rand()*9/RAND_MAX +1;			///< 线大小原来是10以内
	glLineWidth(w);
	glPointSize(w*0.7);

	GLfloat c = rand()*1.0/RAND_MAX;
	glColor4f(c,c,c,c);
	int n = points.size();

	GLuint mVertexBufferObject;
	// 装载顶点
	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(CZ2DPoint), &points[0].x, GL_STREAM_DRAW);


	// 绑定顶点
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,0);

	/// 绘制
	glDrawArrays(GL_LINE_STRIP,0,n);
	//glDrawArrays(GL_POINTS,0,n);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// 消除
	glDeleteBuffers(1, &mVertexBufferObject);

	glDisable(GL_LINE_SMOOTH);
#endif
}
void CZStampRender::drawSpiralData(std::vector<CZ3DPoint> &points)
{
#if USE_OPENGL
	//glEnable(GL_LINE_SMOOTH);		///< 个人感觉还是不启用抗锯齿来得好
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	GLfloat w = rand()*9/RAND_MAX +1;			///< 线大小原来是10以内
	glLineWidth(w);
	glPointSize(w*0.7);

	GLfloat c = rand()*1.0/RAND_MAX;
	glColor4f(c,c,c,c);
	int n = points.size();

	GLuint mVertexBufferObject;
	// 装载顶点
	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(CZ3DPoint), &points[0].x, GL_STREAM_DRAW);


	// 绑定顶点
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,sizeof(CZ3DPoint),0);

	/// 绘制
	glDrawArrays(GL_LINE_STRIP,0,n);
	//glDrawArrays(GL_POINTS,0,n);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// 消除
	glDeleteBuffers(1, &mVertexBufferObject);

	glDisable(GL_LINE_SMOOTH);
#endif
}