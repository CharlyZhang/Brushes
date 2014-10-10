
#ifndef _CZSTAMPRENDER_H_
#define _CZSTAMPRENDER_H_

#include "CZRender.h"

class CZStampRender :public CZRender
{
public:
	/// 初始化
	void init()
	{
		// configure some default GL state
		glDisable(GL_DITHER);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
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
};

#endif