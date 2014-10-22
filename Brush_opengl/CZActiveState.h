
#ifndef _CZACTIVESTATE_H_
#define _CZACTIVESTATE_H_

#include "CZSpiralGenerator.h"
#include "CZStampGenerator.h"
#include "CZNotificationCenter.h"
#include "CZColor.h"
#include <string>

class CZActiveState :public CZObserver
{
public:
	/// 完成单例获取函数
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
		paintColor = CZColor::blackColor();
		return true; 
	};
	/// 销毁函数
	bool destroy() { if(brush != NULL) {delete brush; brush = NULL;} return true;};

public:
	CZBrush *brush;
	CZColor paintColor;
};

#endif