
#ifndef _CZCANVASRENDER_H_
#define _CZCANVASRENDER_H_

#include "CZRender.h"

class CZCanvasRender : public CZRender
{
public:
	/// 初始化绘制状态
	void init()
	{
		// configure some default GL state
		glEnable(GL_BLEND);
		glDisable(GL_DITHER);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);
	};
	/// 配置绘制区域
	void configure(int w, int h){};
	/// 开始绘制
	void begin(){};
	/// 结束绘制
	void end(){};
};
#endif
