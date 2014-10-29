
///  \file CZActiveState.cpp
///  \brief This is the file implements the CZActiveState Class.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-28
///  \note		It is a Singleton

#include "CZActiveState.h"
#include "CZSpiralGenerator.h"
#include "CZFreehandTool.h"
#include "CZEraserTool.h"

using namespace std;

CZActiveState::CZActiveState()
{
	eraseMode = false;
	paintColor = CZColor::blackColor();
	brushes.push_back(CZBrush::randomBrush());
	ptrEraseBrush = ptrPaintBrush = brushes[0];

	tools.push_back(new CZFreehandTool);
	tools.push_back(new CZEraserTool);

}
CZActiveState::~CZActiveState()
{
	for(vector<CZBrush*>::iterator itr = brushes.begin(); itr != brushes.end(); itr++)
		delete *itr;
	brushes.clear();

	for(vector<CZTool*>::iterator itr = tools.begin(); itr != tools.end(); itr++)
		delete *itr;
		tools.clear();
}

/// 设置绘制模式
void CZActiveState::setEraseMode(bool flag)
{
	eraseMode = false;
}

/// 获取绘制模式
bool CZActiveState::isEraseMode()
{
	return eraseMode;
}

/// 设置绘制颜色
void CZActiveState::setPaintColor(CZColor &pc)
{
	paintColor = pc;
}

/// 获取绘制颜色
CZColor CZActiveState::getPaintColor()
{
	return paintColor;
}

/// 设置可用画刷
void CZActiveState::setBrushes(std::vector<CZBrush*> &bs)
{
	for(vector<CZBrush*>::iterator itr = bs.begin(); itr != bs.end(); itr++)
		brushes.push_back(*itr);
}

/// 添加画刷
/// 
///		\note 添加在当前激活的画刷之后
void CZActiveState::addBrush(CZBrush *b)
{
	CZBrush *ptrActiveBrush = getActiveBrush();

	vector<CZBrush*>::iterator itr;

	for (itr = brushes.begin(); itr != brushes.end(); itr++)
	{
		if(ptrActiveBrush == *itr) break;
	}
	
	brushes.insert(itr+1,b);
}

/// 获得可用画刷
vector<CZBrush*>& CZActiveState::getBrushes()
{
	return brushes;
}

/// 设置当前激活画刷
///
///		\param idx - 可用画刷的序号，当非法时默认为0
///		\note	当前设置的画刷种类由模式决定
void CZActiveState::setActiveBrush(int idx)
{
	if (idx < 0 || idx >= brushes.size()) return;
	
	if (eraseMode)
		ptrEraseBrush = brushes[idx];
	else
		ptrPaintBrush = brushes[idx];
}

/// 获取当前画刷
/// 
///		\note 画刷种类由当前的状态eraseMode决定
CZBrush *CZActiveState::getActiveBrush()
{
	CZBrush * ret = eraseMode ? ptrEraseBrush : ptrPaintBrush;

	return ret;
}

/// 删除当前画刷
void CZActiveState::deleteActiveBrush()
{
	if(brushes.size() == 1) return;

	CZBrush *ptrBrush = getActiveBrush();

	vector<CZBrush*>::iterator itr;
	for(itr = brushes.begin(); itr != brushes.end(); itr++)
	{
		if (*itr == ptrBrush)
		{
			itr = brushes.erase(itr);
			break;
		}
	}

	delete ptrBrush;

	/// in case that the last brush has been deleted
	if(itr == brushes.end()) itr--;	

	if (ptrPaintBrush == ptrEraseBrush)
	{
		ptrEraseBrush = ptrPaintBrush = *itr;
	}
	else
	{
		if(eraseMode)	ptrEraseBrush = *itr;
		else			ptrPaintBrush = *itr;
	}
}

/// 获取当前工具
/// 
///		\note 工具种类由当前的状态eraseMode决定
CZTool* CZActiveState::getActiveTool()
{
	return eraseMode ? tools[1] : tools[0];
}

/// 获取随机一个笔刷生成器
CZStampGenerator * CZActiveState::getRandomGenerator()
{
	return new CZSpiralGenerator;
}

