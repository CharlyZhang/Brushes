
///  \file CZPreviewRender.h
///  \brief This is the file declare the class CZPreviewRender.
///
///		负责笔刷预览图的绘制。
///
///  \version	1.0.1
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-14
///  \note


#ifndef _CZPREVIEWRENDER_H_
#define _CZPREVIEWRENDER_H_

#include "CZRender.h"
#include "CZImage.h"
#include "CZPath.h"
#include "CZMat4.h"
#include "CZCommon.h"

class CZBrushPreview;

class CZPreviewRender :public CZRender
{
public:
	CZPreviewRender();
	~CZPreviewRender();
	/// 配置绘制信息
	void configure(int w, int h);
	/// 绘制一条轨迹,并返回图像
	CZImage *drawPath(CZPath *path);
	/// 设置笔刷纹理
	void configureBrush(CZImage *img);
	/// 清除笔刷纹理
	void clearBrush();

private:
	CZShader		*brushShader;
	CZTexture		*brushTexture;
	CZMat4			projMat;		///< 绘制的投影矩阵
	CZBrushPreview	*ptrPreview;	///< 指向调用其绘制的preview
};

#endif