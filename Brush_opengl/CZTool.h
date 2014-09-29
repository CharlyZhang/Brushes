
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
	/// 开始移动
	void moveBegin(CZ2DPoint &p_){ moved = false;};
	/// 正在移动
	void moving(CZ2DPoint &p_){ moved = true;};
	/// 移动结束
	void moveEnd(CZ2DPoint &p_){};

protected:
	bool moved;
};


#endif
