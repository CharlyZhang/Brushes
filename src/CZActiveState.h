
///  \file CZActiveState.h
///  \brief This is the file declare the CZActiveState Class.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-28
///  \note		It is a Singleton

#ifndef _CZACTIVESTATE_H_
#define _CZACTIVESTATE_H_

#include "CZSpiralGenerator.h"
#include "CZStampGenerator.h"
#include "CZNotificationCenter.h"
#include "CZColor.h"
#include <string>

/// 激活状态类
class CZActiveState :public CZObserver
{
public:
	/// 获取单例
	static CZActiveState * getInstance()
	{
		static CZActiveState instance;   //局部静态变量  
		return &instance; 
	}
	/// 获取随机一个笔刷生成器
	CZStampGenerator * getRandomGenerator(){return NULL;};
	/// 实现Observer接口
	void updateObserver(std::string &notificationName, void* data = NULL){};

private:
	CZActiveState(){ initial(); }   //构造函数是私有的
	CZActiveState(const CZActiveState &);
	CZActiveState & operator = (const CZActiveState &);
	~CZActiveState(){ destroy(); }

	/// 初始化函数
	bool initial()
	{ 
		brush = new CZBrush(new CZSpiralGenerator);
		paintColor = CZColor::greenColor();
		return true; 
	};
	/// 销毁函数
	bool destroy() { if(brush != NULL) {delete brush; brush = NULL;} return true;};

public:
	CZBrush *brush;
	CZColor paintColor;
};

#endif