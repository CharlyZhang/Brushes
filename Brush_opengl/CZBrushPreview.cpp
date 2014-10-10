
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

using namespace std;

/// ��ʼ������
bool CZBrushPreview::initial()
{
	path = NULL;
	ptrBrush = NULL;
	brushShader = NULL;
	tex = NULL;
	backingWidth = backingHeight = 0.0f;
	mainScreenScale = 1.0f;

	render.init();

	return true;

}

/// ���ٺ���
bool CZBrushPreview::destroy()
{
	if(path)	{	delete path; path = NULL; }
	if(brushShader) { delete brushShader; brushShader = NULL;}
	if(tex)		{	delete tex;	tex = NULL; }
	return true;
}

/// ������Ԥ��ͼ������FBO���������ɻ��ƵĹ켣��
void CZBrushPreview::setup(const CZSize &size_)
{
	if (backingWidth == size_.width && backingHeight == size_.height) return;

	backingWidth = size_.width;
	backingHeight = size_.height;

	/// ��ʼ������ͻ�����
	if(tex != NULL) delete tex;
	tex = new CZTexture(backingWidth,backingHeight);
	
	render.fbo.setTexture(tex);

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

	float scale = mainScreenScale;

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
		vector<string> tmp1,tmp2;
		brushShader = new CZShader("brush.vert","brush.frag",tmp1, tmp2);
	}

	/// ������
	CZTexture *stampTex = ptrBrush->generator->getStamp();
	if(stampTex == NULL)
	{
		std::cerr << "CZBrushPreview::configureBrush - stampTex is NULL\n";
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,stampTex->id);
}

/// չ��ָ���ߴ��СԤ��ͼ
CZTexture* CZBrushPreview::previewWithSize(CZSize size_)
{
	if(ptrBrush == NULL)
	{
		std::cerr << "Error - CZBrushPreview::previewWithSize : ptrBrush is NULL\n";
		return NULL;
	}

	/// �����豸�ֱ��ʽ��е���
	size_ = size_ * mainScreenScale;
	/// �����¹켣
	setup(size_);
	/// ���ñ�ˢ
	configureBrush();

	/// ���ù켣����
	path->setBrush(ptrBrush);
	path->remainder = 0.0f;
	path->setClosed(false);
	path->ptrShader = brushShader;		///< !û�б�Ҫ

	CZImage *ret = new CZImage(backingWidth,backingHeight,CZImage::RGBA);

	render.begin();
	brushShader->begin();
	/// ���ƹ켣
	path->paint();
	brushShader->end();

	checkPixels(backingWidth,backingHeight);
	glReadPixels(0, 0, backingWidth, backingHeight, GL_RGBA, GL_FLOAT, ret->data);

	render.end();
	
	return tex;
	//return tex->img;
}

/// ���û�ˢ
void CZBrushPreview::setBrush(CZBrush *brush_)
{
	ptrBrush = brush_;
	
	ptrBrush->addGenChangeDelegate(this);
	ptrBrush->addGenReplaceDelegate(this);
}

/// �����豸��Ļ�ֱ��ʱ���
void CZBrushPreview::setMainScreenScale(float s)
{
	mainScreenScale = s;
}
