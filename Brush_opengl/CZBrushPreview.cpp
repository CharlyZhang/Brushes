
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
#include "CZUtil.h"
#include "GL/glut.h"
#include "Macro.h"

#include <vector>
#include <cmath>

#define M_PI       3.14159265358979323846

/// ��ʼ������
bool CZBrushPreview::initial()
{
	path = NULL;
	ptrBrush = NULL;
	brushShader = NULL;
	fbo = NULL;
	tex = NULL;
	backingWidth = backingHeight = 0.0f;

	// configure some default GL state
	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	/// ��Ӽ�����
	addObserver2Preview();

	return true;

}

/// ���ٺ���
bool CZBrushPreview::destroy()
{
	if(path)	{	delete path; path = NULL; }
	if(brushShader) { delete brushShader; brushShader = NULL;}
	if(fbo)		{	delete fbo;	fbo = NULL; }
	if(tex)		{	delete tex;	tex = NULL; }
	return true;
}
/// ������Ԥ��ͼ������FBO���������ɻ��ƵĹ켣��
void CZBrushPreview::setup(const CZSize &size_)
{
	if (backingWidth == size_.width && backingHeight == size_.height) return;

	backingWidth = size_.width;
	backingHeight = size_.height;

	/// ��ʼ��FBO������
	if(tex != NULL) delete tex;
	tex = new CZTexture(backingWidth,backingHeight);
	if(fbo != NULL) delete fbo;
	fbo = new CZFbo(tex);

	/// ����·��
	if(path) delete path;
	buildPath();

	/// ����ͶӰ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) backingWidth, 0.0f, 
		(GLfloat) backingHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
		float amplitude  = 10.0f;			///< ���

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

/// ���û�ˢ������shader��������
void CZBrushPreview::configureBrush()
{
	if(brushShader == NULL)
	{
		brushShader = new CZShader;
		brushShader->readVertextShader("brush.vert");
		brushShader->readFragmentShader("brush.frag");
		brushShader->setShader();
	}

	/// ������
	CZTexture *stampTex = ptrBrush->getTexture(true);
	if(stampTex == NULL)
	{
		std::cerr << "CZBrushPreview::configureBrush - stampTex is NULL\n";
		return;
	}
	glBindTexture(GL_TEXTURE_2D,stampTex->id);
}

/// չ��ָ���ߴ��СԤ��ͼ
CZTexture* CZBrushPreview::previewWithSize(const CZSize &size_)
{
	if(ptrBrush == NULL)
	{
		std::cerr << "Error - CZBrushPreview::previewWithSize : ptrBrush is NULL\n";
		return NULL;
	}

	/// �����¹켣
	setup(size_);
	/// ���ñ�ˢ
	configureBrush();

	fbo->begin();

	glClearColor(.0f, .0f, .0f, .0f);	
	glClear(GL_COLOR_BUFFER_BIT);
	
	{	/// �Է������ط��ƻ���������״̬
		glEnable(GL_BLEND);							
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	/// ���ù켣����
	path->setBrush(ptrBrush);
	path->remainder = 0.0f;
	path->setClosed(false);
	path->ptrShader = brushShader;		///< !û�б�Ҫ

	glColor4f(1.0,1.0,1.0,0.5);
	brushShader->begin();
	/// ���ƹ켣
	path->paint();

	brushShader->end();

	fbo->end();
	
	return tex;
}

/// ���û�ˢ
void CZBrushPreview::setBrush(CZBrush *brush_)
{
	ptrBrush = brush_;
}
