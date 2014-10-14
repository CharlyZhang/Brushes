
#ifndef _CZSTAMPRENDER_H_
#define _CZSTAMPRENDER_H_

#include "CZRender.h"
#include "CZGeometry.h"
#include "CZ3DPoint.h"
#include <vector>

class CZStampRender :public CZRender
{
public:
	/// 完成单例获取函数
	static CZStampRender * getInstance()
	{
		static CZStampRender instance;   //局部静态变量  
		return &instance; 
	}

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

		glClearColor(.0f, .0f, .0f, .0f);	
		glClear(GL_COLOR_BUFFER_BIT);

		{	/// 以防其他地方破坏了上下文状态
			glEnable(GL_BLEND);							
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		}

		glColor4f(1.0,1.0,1.0,0.5);
		
	};

	/// 结束
	void end()
	{
		fbo.end();
	};

	/// 绘制螺旋纹
	void drawSpiralData(std::vector<CZ2DPoint> &points)
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
	void drawSpiralData(std::vector<CZ3DPoint> &points)
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
};

#endif