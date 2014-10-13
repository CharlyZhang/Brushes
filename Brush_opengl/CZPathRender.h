
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

class CZPathRender :public CZRender
{
public:
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
};

#endif