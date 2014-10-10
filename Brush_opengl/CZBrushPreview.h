
///  \file CZBrushPreview.h
///  \brief This is the file declare the Class CZBrushPreview.
///
///		这是一个单例，展现笔刷的预览图。
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-16
///  \note

#include "CZPath.h"
#include "CZFbo.h"
#include "CZShader.h"
#include "CZTexture.h"
#include "CZImage.h"

class CZBrushPreview
{
public:
	/// 完成单例获取函数
	static CZBrushPreview * getInstance()
	{
		static CZBrushPreview instance;   //局部静态变量  
		return &instance; 
	}

	/// 展现指定尺寸大小预览图
	///		
	///		这里根据指定的大小，绘制出相应的纹理。主要包括内部资源的建立以及画笔轨迹的绘制。
	///		不处理缩，窗口缩放由外部控制，这里只以参数中的尺寸为准。
	///		主要采用OpenGL的FBO和Shader。
	/// 
	///		/param size - 绘制预览的尺寸大小
	///		/return		- 绘制得到的纹理
	///		/note		使用之前应该先调用setBrush设定笔刷。
	CZImage *previewWithSize(const CZSize &size_);
	/// 设置画刷
	void setBrush(CZBrush *brush_);

private:
	CZBrushPreview(){ initial(); }   //构造函数是私有的
	CZBrushPreview(const CZBrushPreview &);
	CZBrushPreview & operator = (const CZBrushPreview &);
	~CZBrushPreview(){ destroy(); }

	/// 初始化函数
	bool initial();
	/// 销毁函数
	bool destroy();
	/// 启动新预览图（生成FBO和纹理，生成绘制的轨迹）
	void setup(const CZSize &size_);
	/// 构建轨迹（绘制一条sin曲线）
	void buildPath();
	/// 配置画刷（配置shader，绑定纹理）
	void configureBrush();

public:
	/// 成员变量
	CZPath *path;							///< 绘制的轨迹
	CZShader *brushShader;					
	CZFbo *fbo;
	CZTexture *tex;							///< 呈现预览图的纹理		
	float backingWidth, backingHeight;

private:
	CZBrush *ptrBrush;						///< 仅引用，不负责建立和销毁
};