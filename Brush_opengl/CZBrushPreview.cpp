
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
#include "CZMat4.h"
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
	backingWidth = backingHeight = 0.0f;
	mainScreenScale = 1.0f;

	CZNotificationCenter::getInstance()->addObserver(CZBrushGeneratorChanged,this,NULL);
	CZNotificationCenter::getInstance()->addObserver(CZBrushGeneratorReplaced,this,NULL);

	return true;

}

/// ���ٺ���
bool CZBrushPreview::destroy()
{
	if(path)	{	delete path; path = NULL; }

	CZNotificationCenter::getInstance()->removeObserver(CZBrushGeneratorChanged,this);
	CZNotificationCenter::getInstance()->removeObserver(CZBrushGeneratorReplaced,this);
	return true;
}

/// ������Ԥ��ͼ�����û����������ɻ��ƵĹ켣��
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
	/// ���ñ�ˢ
	render.configureBrush(getBrushImage());

	/// ���ù켣����
	path->setBrush(ptrBrush);
	path->remainder = 0.0f;
	path->setClosed(false);

	CZImage *ret = render.drawPath(path);	
	return ret;
}

/// ���û�ˢ
void CZBrushPreview::setBrush(CZBrush *brush_)
{
	/// ���ˢ���������ı��ˣ����ˢͼ���ı�
	if(ptrBrush && brush_ && ptrBrush->generator->isEqual(brush_->generator))
	{
		render.clearBrush();
	}

	ptrBrush = brush_;
}

/// �����豸��Ļ�ֱ��ʱ���
void CZBrushPreview::setMainScreenScale(float s)
{
	mainScreenScale = s;
}

/// ��ȡ��ˢͼ��
CZImage* CZBrushPreview::getBrushImage()
{
	CZStampGenerator *gen = ptrBrush->generator;
	return gen->getStamp(true);		///< get the small stamp;
}

/// ʵ��Observer�ӿ�
void CZBrushPreview::updateObserver(std::string &notificationName, void* data /* = NULL */)
{
	if (notificationName == CZBrushGeneratorChanged)
	{
		render.clearBrush();
	}
}
