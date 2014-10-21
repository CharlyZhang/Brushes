
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
#include "CZUtil.h"
#include <string>
#include <vector>

class CZPreviewRender :public CZRender
{
public:
	CZPreviewRender();
	~CZPreviewRender();
	/// 配置绘制信息（改变内部变量）
	void configure(std::map<std::string,void*> &conf);
	/// 开始绘制（执行具体的绘制调用）
	void begin(DrawType type);
	/// 调用接口绘制
	void draw(DrawType type, void* data = NULL, unsigned int num = 0);
	/// 生成图像数据并返回
	CZImage* imageForLastDraw();
	/// 结束绘制
	void end(DrawType type);

	/// 设置笔刷纹理
	void configureBrush(CZImage *img);
	/// 清除笔刷纹理
	void clearBrush();

private:
	/// 绘制轨迹数据（利用图形接口）
	void drawPathData(vertexData *data, unsigned int n);

private:
	CZShader *brushShader;
	CZTexture *brushTexture;

};

#endif