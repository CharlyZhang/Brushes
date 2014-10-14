
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
	brushTexture = NULL;
	backingWidth = backingHeight = 0.0f;
	mainScreenScale = 1.0f;

	render.init();

	CZNotificationCenter::getInstance()->addObserver(CZBrushGeneratorChanged,this,NULL);
	CZNotificationCenter::getInstance()->addObserver(CZBrushGeneratorReplaced,this,NULL);

	return true;

}

/// ���ٺ���
bool CZBrushPreview::destroy()
{
	if(path)	{	delete path; path = NULL; }
	if(brushShader) { delete brushShader; brushShader = NULL;}
	if(brushTexture) {delete brushTexture; brushTexture = NULL;}

	CZNotificationCenter::getInstance()->removeObserver(CZBrushGeneratorChanged,this);
	CZNotificationCenter::getInstance()->removeObserver(CZBrushGeneratorReplaced,this);
	return true;
}

/// ������Ԥ��ͼ������FBO���������ɻ��ƵĹ켣��
void CZBrushPreview::setup(const CZSize &size_)
{
	if (backingWidth == size_.width && backingHeight == size_.height) return;

	backingWidth = size_.width;
	backingHeight = size_.height;

	/// ���û�����
	render.configure(backingWidth,backingHeight);

	/// ����·��
	if(path) delete path;
	buildPath();

#if 0
	// handle viewing matrices
	GLfloat proj[16], scale[16];
	// setup projection matrix (orthographic)
	mat4f_LoadOrtho(0, backingWidth, 0, backingHeight, -1.0f, 1.0f, proj);

	float s = [UIScreen mainScreen].scale;
	CGAffineTransform tX = CGAffineTransformMakeScale(s, s);
	mat4f_LoadCGAffineTransform(scale, tX);

	mat4f_MultiplyMat4f(proj, scale, projection);
#endif
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
			path->addNode(node);
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

	render.ptrShader = brushShader;

	/// ������
	CZTexture *stampTex = getBrushTexture();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,stampTex->id);
}

/// ����ָ���ߴ��СԤ��ͼ
CZImage* CZBrushPreview::previewWithSize(CZSize size_)
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
	/// ���ص�ͼ��
	CZImage *ret = new CZImage(backingWidth,backingHeight,CZImage::RGBA);
	/// ���ñ�ˢ
	configureBrush();

	/// ���ù켣����
	path->setBrush(ptrBrush);
	path->remainder = 0.0f;
	path->setClosed(false);

	render.begin();
	/// ���ƹ켣
	path->paint(&render,path->getRandomizer());

	glReadPixels(0, 0, backingWidth, backingHeight, GL_RGBA, GL_FLOAT, ret->data);

	render.end();
	
	return ret;
}

/// ���û�ˢ
void CZBrushPreview::setBrush(CZBrush *brush_)
{
	/// ���ˢ���������ı��ˣ����ˢͼ���ı�
	if(ptrBrush && brush_ && ptrBrush->generator->isEqual(brush_->generator))
	{
		delete brushTexture;
		brushTexture = NULL;
	}

	ptrBrush = brush_;
}

/// �����豸��Ļ�ֱ��ʱ���
void CZBrushPreview::setMainScreenScale(float s)
{
	mainScreenScale = s;
}

/// ��ȡ��ˢ����
CZTexture* CZBrushPreview::getBrushTexture()
{
	if(brushTexture == NULL)
	{
		CZStampGenerator *gen = ptrBrush->generator;
		CZImage *smallStamp = gen->getStamp(true);		///< get the small stamp
		brushTexture = smallStamp->toTexture();
	}
	
	return brushTexture;
}

/// ʵ��Observer�ӿ�
void CZBrushPreview::updateObserver(std::string &notificationName, void* data /* = NULL */)
{
	if (notificationName == CZBrushGeneratorChanged)
	{
		if(brushTexture) {delete brushTexture; brushTexture = NULL;}
	}
}
