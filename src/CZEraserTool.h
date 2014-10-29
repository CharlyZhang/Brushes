
#ifndef _CZERASERTOOL_H_
#define _CZERASERTOOL_H_

#include "CZTool.h"

class CZEraserTool : public CZTool
{
public:
	/// 开始移动（如果支持压力，则参数为压力值）
	void moveBegin(CZ2DPoint &p_, float pressure_ = 0.0f){};

	/// 正在移动
	///		
	///		将连续5个点处理，每处理一次绘制一遍。
	///
	///		/param p_				  - 当前移动的点位置
	///		/param pressureOrSpeed	  - 当设备支持压力值时，为压力值；否则为移动速度值(pixels/millisecond)
	void moving(CZ2DPoint &p_, float pressureOrSpeed){};

	/// 移动结束
	void moveEnd(CZ2DPoint &p_){};
};

#endif