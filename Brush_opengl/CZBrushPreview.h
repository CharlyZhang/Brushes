
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
	~CZBrushPreview(){ destroy(); }

	/// 初始化函数
	bool initial();
	/// 销毁函数
	bool destroy();
	/// 启动新预览图（生成绘制的轨迹）
	void setup(const CZSize &size_);
	/// 构建轨迹（绘制一条sin曲线）
	void buildPath();

public:
	/// 完成单例获取函数
	static CZBrushPreview * getInstance()
	{
		static CZBrushPreview instance;   //局部静态变量  
		return &instance; 
	}

	/// 成员变量
	CZPath *path;
	CZBrush *brush;						///< 仅引用，不负责建立和销毁
	float backingWidth, backingHeight;

	
	/// 展现指定尺寸大小预览图
	void previewWithSize(const CZSize &size_ = CZSize(0,0));
	/// 设置画刷
	void setBrush(CZBrush *brush_);
};