
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

#include "CZStampGenerator.h"
#include "CZColor.h"
#include "CZBrush.h"
#include "CZTool.h"
#include <string>
#include <vector>
#include <map>

/// 激活状态类
class CZActiveState
{
public:
	/// 获取单例
	static CZActiveState * getInstance()
	{
		static CZActiveState instance;   
		return &instance; 
	}

	/// 设置绘制模式
	void setEraseMode(bool flag);
	/// 获取绘制模式
	bool isEraseMode();
	/// 设置绘制颜色
	void setPaintColor(CZColor &pc);
	void setPaintColor(float r, float g, float b, float a = 1.0f);
	/// 获取绘制颜色
	CZColor getPaintColor();
	/// 设置可用画刷
	void setBrushes(std::vector<CZBrush*> &bs);
	/// 添加画刷
	/// 
	///		\note 添加在当前激活的画刷之后
	void addBrush(CZBrush *b);
	/// 获得可用画刷
	std::vector<CZBrush*> &getBrushes();
	/// 设置当前激活画刷
	///
	///		\param idx - 可用画刷的序号，当非法时默认为0
	///		\ret	   - 原来的激活画刷序号
	///		\note	当前设置的画刷种类由模式决定
	int setActiveBrush(int idx);
	/// 获取当前画刷
	/// 
	///		\note 画刷种类由当前的状态eraseMode决定
	CZBrush *getActiveBrush();
	/// 删除当前画刷
	void deleteActiveBrush();
	/// 获取当前工具
	/// 
	///		\note 工具种类由当前的状态eraseMode决定
	CZTool *getActiveTool();
	

	/// 获取随机一个笔刷生成器
	CZStampGenerator * getRandomGenerator();

private:
	CZActiveState();
	CZActiveState(const CZActiveState &);
	CZActiveState & operator = (const CZActiveState &);
	~CZActiveState();

	/// 获得画刷在所有画刷中的标号，不存在返回负值
	int indexOfBrushes(CZBrush *brush);

private:
	std::vector<CZBrush*> brushes;						///< 所有可用的画刷，负责销毁
	std::vector<CZTool*> tools;							///< 可用工具，目前为绘制画刷和擦除刷两种
	std::map<std::string,CZStampGenerator*>	generators;	///< 所有可用的画刷生成器，负责销毁
	bool eraseMode;										///< 标记是否处在擦除模式
	CZBrush *ptrPaintBrush;								///< 绘制用笔刷
	CZBrush *ptrEraseBrush;								///< 擦除用笔刷 

	CZColor paintColor;									///< 绘制颜色
};

#endif