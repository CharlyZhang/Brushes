
#ifndef _CZCANVASRENDER_H_
#define _CZCANVASRENDER_H_

#include "CZRender.h"

class CZCanvasRender : public CZRender
{
public:
	/// ��ʼ������״̬
	void init()
	{
		// configure some default GL state
		glEnable(GL_BLEND);
		glDisable(GL_DITHER);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);
	};
	/// ���û�������
	void configure(int w, int h){};
	/// ��ʼ����
	void begin(){};
	/// ��������
	void end(){};
};
#endif
