
///  \file CZStampRender.h
///  \brief This is the file declare the Class CZStampRender.
///
///		这里用来绘制笔刷图形的绘制器，是个单例。
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note

#ifndef _CZSTAMPRENDER_H_
#define _CZSTAMPRENDER_H_

#include "CZRender.h"
#include "CZGeometry.h"
#include "CZ3DPoint.h"
#include <vector>

class CZStampRender :public CZRender
{
public:
	/// 完成单例获取函数
	static CZStampRender * getInstance()
	{
		static CZStampRender instance;   //局部静态变量  
		return &instance; 
	}

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

	/// 绘制螺旋纹
	void drawSpiralData(std::vector<CZ2DPoint> &points);
	void drawSpiralData(std::vector<CZ3DPoint> &points);
private:
	CZStampRender();   
	CZStampRender(const CZStampRender &);
	CZStampRender & operator = (const CZStampRender &);
	~CZStampRender(){ }

};

#endif