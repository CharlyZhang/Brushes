
///  \file CZBrushPreview.cpp
///  \brief This is the file mlass CZBrushPreview Preview.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-16
///  \note

#include "CZBrushPreview.h"
#include "CZBezierNode.h"
#include "CZGeometry.h"
#include "Macro.h"

#include <vector>
#include <cmath>

#define M_PI       3.14159265358979323846

/// ��ʼ������
bool CZBrushPreview::initial()
{
	path = NULL;
	brush = NULL;

#if USE_OPENGL_ES
	context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

	if (!context || ![EAGLContext setCurrentContext:context]) {
		return nil;
	}

	// create system framebuffer object. The backing will be allocated in -reshapeFramebuffer
	glGenFramebuffers(1, &defaultFramebuffer);
	glGenRenderbuffers(1, &colorRenderbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

	// configure some default GL state
	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
/*
	[[NSNotificationCenter defaultCenter] addObserver:self
selector:@selector(brushGeneratorChanged:)
name:WDBrushGeneratorChanged
object:nil];

	[[NSNotificationCenter defaultCenter] addObserver:self
selector:@selector(brushGeneratorChanged:)
name:WDBrushGeneratorReplaced
object:nil];
*/
#endif

	return true;

}

/// ���ٺ���
bool CZBrushPreview::destroy()
{
	if(path) delete path;
	return true;
}
/// ������Ԥ��ͼ�����ɻ��ƵĹ켣��
void CZBrushPreview::setup(const CZSize &size_)
{
	backingWidth = size_.width;
	backingHeight = size_.height;

	/// ����·��
	if(path) delete path;
	buildPath();
}

/// �����켣������һ��sin���ߣ�
void CZBrushPreview::buildPath()
{
	if (path) 	return;
	path = new CZPath;
	path->limitBrushSize = true;

	float scale = 1.0;//[UIScreen mainScreen].scale;

	// build a nice little sin curve
	{
		CZ2DPoint start( 30, backingHeight / (2.0f * scale) );
		float   width = (backingWidth / scale) - 2.0f * 30;
		float amplitude  = 10.0f;

		float kNumLineSegments = 100;
		for (int i = 0; i < kNumLineSegments; i++) 
		{
			float fraction = (float)i / (kNumLineSegments - 1);
			CZ2DPoint pt(start.x + width * fraction, start.y + sin(fraction * 2 * M_PI) * amplitude );
			CZBezierNode node(CZ3DPoint(pt.x, pt.y, fraction));
			path->nodes.push_back(node);
		}
	}
}

/// չ��ָ���ߴ��СԤ��ͼ
void CZBrushPreview::previewWithSize(const CZSize &size_)
{
	/// �����¹켣
	setup(size_);

	/// ���ù켣����
	path->brush = brush;
	path->remainder = 0.0f;
	path->setClosed(false);
	
	/// ���ƹ켣
	//path->paint();

}

/// ���û�ˢ
void CZBrushPreview::setBrush(CZBrush *brush_)
{
	brush = brush_;
}
