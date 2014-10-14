
#ifndef _CZSTAMPRENDER_H_
#define _CZSTAMPRENDER_H_

#include "CZRender.h"
#include "CZGeometry.h"
#include "CZ3DPoint.h"
#include <vector>

class CZStampRender :public CZRender
{
public:
	/// ��ɵ�����ȡ����
	static CZStampRender * getInstance()
	{
		static CZStampRender instance;   //�ֲ���̬����  
		return &instance; 
	}

	/// ��ʼ��״̬
	void init()
	{
		// configure some default GL state
		glDisable(GL_DITHER);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}
	/// ���û�������
	void configure(int w, int h)
	{
		width  = w;
		height = h;
		
		//~ ����context

		fbo.setColorRenderBuffer(width,height);

		/// ����ͶӰ����
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, (GLfloat) width, 0.0f, 
			(GLfloat) height, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity(); 
	}
	/// ��ʼ
	void begin()
	{
		fbo.begin();

		glClearColor(.0f, .0f, .0f, .0f);	
		glClear(GL_COLOR_BUFFER_BIT);

		{	/// �Է������ط��ƻ���������״̬
			glEnable(GL_BLEND);							
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		}

		glColor4f(1.0,1.0,1.0,0.5);
		
	};

	/// ����
	void end()
	{
		fbo.end();
	};

	/// ����������
	void drawSpiralData(std::vector<CZ2DPoint> &points)
	{
#if USE_OPENGL
		//glEnable(GL_LINE_SMOOTH);		///< ���˸о����ǲ����ÿ�������ú�
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		GLfloat w = rand()*9/RAND_MAX +1;			///< �ߴ�Сԭ����10����
		glLineWidth(w);
		glPointSize(w*0.7);

		GLfloat c = rand()*1.0/RAND_MAX;
		glColor4f(c,c,c,c);
		int n = points.size();

		GLuint mVertexBufferObject;
		// װ�ض���
		glGenBuffers(1, &mVertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, n * sizeof(CZ2DPoint), &points[0].x, GL_STREAM_DRAW);


		// �󶨶���
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,0,0);

		/// ����
		glDrawArrays(GL_LINE_STRIP,0,n);
		//glDrawArrays(GL_POINTS,0,n);

		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/// ����
		glDeleteBuffers(1, &mVertexBufferObject);

		glDisable(GL_LINE_SMOOTH);
#endif
	}
	void drawSpiralData(std::vector<CZ3DPoint> &points)
	{
#if USE_OPENGL
		//glEnable(GL_LINE_SMOOTH);		///< ���˸о����ǲ����ÿ�������ú�
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		GLfloat w = rand()*9/RAND_MAX +1;			///< �ߴ�Сԭ����10����
		glLineWidth(w);
		glPointSize(w*0.7);

		GLfloat c = rand()*1.0/RAND_MAX;
		glColor4f(c,c,c,c);
		int n = points.size();

		GLuint mVertexBufferObject;
		// װ�ض���
		glGenBuffers(1, &mVertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, n * sizeof(CZ3DPoint), &points[0].x, GL_STREAM_DRAW);


		// �󶨶���
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,sizeof(CZ3DPoint),0);

		/// ����
		glDrawArrays(GL_LINE_STRIP,0,n);
		//glDrawArrays(GL_POINTS,0,n);

		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/// ����
		glDeleteBuffers(1, &mVertexBufferObject);

		glDisable(GL_LINE_SMOOTH);
#endif
	}
};

#endif