
///  \file CZTool.h
///  \brief This is the file declare the Class CZTool.
///
///		The CZTool is designed for input, it's the super class of all input classes. 
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-28
///  \note

#ifndef _CZTOOL_H_
#define _CZTOOL_H_

#include "CZGeometry.h"

class CZTool 
{
public:
	/// 开始移动（如果支持压力，则参数为压力值）
	virtual void moveBegin(CZ2DPoint &p_, float pressure_ = 0.0f) = 0;

	/// 正在移动
	///		
	///		将连续5个点处理，每处理一次绘制一遍。
	///
	///		/param p_				  - 当前移动的点位置
	///		/param pressureOrSpeed	  - 当设备支持压力值时，为压力值；否则为移动速度值(pixels/millisecond)
	virtual void moving(CZ2DPoint &p_, float pressureOrSpeed) = 0;

	/// 移动结束
	virtual void moveEnd(CZ2DPoint &p_) = 0;

protected:
	bool moved;
};


#endif
