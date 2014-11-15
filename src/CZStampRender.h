
///  \file CZStampRender.h
///  \brief This is the file declares the Class CZStampRender.
///
///		这里用来绘制笔刷图形的绘制器，是个单例。
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note		只用到离线绘制

#ifndef _CZSTAMPRENDER_H_
#define _CZSTAMPRENDER_H_

#include "CZRender.h"
#include "CZGeometry.h"
#include "CZ3DPoint.h"
#include "CZMat4.h"
#include <vector>

class CZStampGenerator;

class CZStampRender :public CZRender
{
public:
	/// 完成单例获取函数
	static CZStampRender * getInstance()
	{
		static CZStampRender instance;   //局部静态变量  
		return &instance; 
	}
	/// 配置大小
	void configure(int w, int h);
	/// 生成stamp图像
	CZImage *drawStamp();
	
	/// 绘制螺旋纹
	void drawSpiralData(std::vector<CZ2DPoint> &points);
	void drawSpiralData(std::vector<CZ3DPoint> &points);

private:
	CZStampRender();  //构造函数是私有的
	CZStampRender(const CZStampRender &);
	CZStampRender & operator = (const CZStampRender &);
	~CZStampRender();

public:
	CZStampGenerator *ptrGenerator;		///< 当前绘制的生成器
private:
	CZMat4 projMat;						///< 投影矩阵
	CZShader *shader;					///< 着色器
};

#endif