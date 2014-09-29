
#ifndef _CZACTIVESTATE_H_
#define _CZACTIVESTATE_H_

#include "CZSpiralGenerator.h"

class CZActiveState
{
public:
	/// 完成单例获取函数
	static CZActiveState * getInstance()
	{
		static CZActiveState instance;   //局部静态变量  
		return &instance; 
	}

private:
	CZActiveState(){ initial(); }   //构造函数是私有的
	CZActiveState(const CZActiveState &);
	CZActiveState & operator = (const CZActiveState &);
	~CZActiveState(){ destroy(); }

	/// 初始化函数
	bool initial(){ brush = new CZBrush(new CZSpiralGenerator); return true; };
	/// 销毁函数
	bool destroy() { if(brush != NULL) {delete brush; brush = NULL;} return true;};

public:
	CZBrush *brush;
};

#endif