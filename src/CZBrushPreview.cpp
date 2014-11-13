
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

/// 初始化函数
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

/// 销毁函数
bool CZBrushPreview::destroy()
{
	if(path)	{	delete path; path = NULL; }

	CZNotificationCenter::getInstance()->removeObserver(CZBrushGeneratorChanged,this);
	CZNotificationCenter::getInstance()->removeObserver(CZBrushGeneratorReplaced,this);
	return true;
}

/// 启动新预览图（配置绘制器，生成绘制的轨迹）
void CZBrushPreview::setup(const CZSize &size_)
{
	if (backingWidth == size_.width && backingHeight == size_.height) return;

	backingWidth = size_.width;
	backingHeight = size_.height;

	/// 配置绘制器
	render.configure(backingWidth,backingHeight);

	/// 创建路径
	if(path) delete path;
	buildPath();
}

/// 构建轨迹（绘制一条sin曲线）
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
		float amplitude  = 10.0f;			///< 振幅

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

/// 生成指定尺寸大小预览图
CZImage* CZBrushPreview::previewWithSize(CZSize size_)
{
	if(ptrBrush == NULL)
	{
		std::cerr << "Error - CZBrushPreview::previewWithSize : ptrBrush is NULL\n";
		return NULL;
	}

	/// 根据设备分辨率进行调整
	size_ = size_ * mainScreenScale;
	/// 生成新轨迹
	setup(size_);
	/// 配置笔刷
	render.configureBrush(getBrushImage());

	/// 设置轨迹参数
	path->setBrush(ptrBrush);
	path->remainder = 0.0f;
	path->setClosed(false);

	CZImage *ret = render.drawPath(path);	
	return ret;
}

/// 设置画刷
void CZBrushPreview::setBrush(CZBrush *brush_)
{
	/// 如果刷子生成器改变了，则笔刷图案改变
	if(ptrBrush && brush_ && ptrBrush->generator->isEqual(brush_->generator))
	{
		render.clearBrush();
	}

	ptrBrush = brush_;
}

/// 设置设备屏幕分辨率倍数
void CZBrushPreview::setMainScreenScale(float s)
{
	mainScreenScale = s;
}

/// 获取笔刷图像
CZImage* CZBrushPreview::getBrushImage()
{
	CZStampGenerator *gen = ptrBrush->generator;
	return gen->getStamp(true);		///< get the small stamp;
}

/// 实现Observer接口
void CZBrushPreview::updateObserver(std::string &notificationName, void* data /* = NULL */)
{
	if (notificationName == CZBrushGeneratorChanged)
	{
		render.clearBrush();
	}
}
