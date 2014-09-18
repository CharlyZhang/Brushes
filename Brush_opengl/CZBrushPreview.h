
///  \file CZBrushPreview.h
///  \brief This is the file declare the Class CZBrushPreview.
///
///		这是一个单例，展现笔刷的预览图.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-16
///  \note

#include "CZPath.h"

class CZBrushPreview
{
private:
	CZBrushPreview(){ initial(); }   //构造函数是私有的
	CZBrushPreview(const CZBrushPreview &);
	CZBrushPreview & operator = (const CZBrushPreview &);

public:
	/// 完成单例获取函数
	static CZBrushPreview *pInstance;
	static CZBrushPreview * getInstance()
	{
		if(pInstance == NULL)  pInstance = new CZBrushPreview();  
		return pInstance;
	}

	/// 成员变量
	CZPath *path;
	CZBrush *brush;
	float backingWidth, backingHeight;

	/// 初始化函数
	bool initial();
	/// 构建路径（绘制一条sin曲线）
	void buildPath();
	/// 展现指定尺寸大小预览图
	void previewWithSize(const CZSize &size_);
};