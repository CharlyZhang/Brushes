
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
#include "CZDefine.h"
#include "brush/CZBrush.h"
#include "stamp/CZSpiralGenerator.h"
#include "stamp/CZRoundGenerator.h"
#include "tool/CZFreehandTool.h"
#include "tool/CZEraserTool.h"
#include "CZUtil.h"
#include "graphic/CZGLContext.h"

using namespace std;

CZActiveState *CZActiveState::ptrInstance = NULL;

CZActiveState::CZActiveState()
{
	eraseMode = false;
	paintColor = CZColor::blackColor();
	stampGLContext = new CZGLContext;
	setUpGenerators();
	initBrushes();

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

	for(map<string,CZStampGenerator*>::iterator itr = generators.begin(); itr != generators.end(); itr++)
		delete itr->second;
	generators.clear();

	delete stampGLContext;
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
void CZActiveState::setPaintColor(float r, float g, float b, float a /* = 1.0f */)
{
	paintColor = CZColor(r,g,b,a);
}

/// 获取绘制颜色
CZColor CZActiveState::getPaintColor()
{
	return paintColor;
}

/// 添加新画刷
/// 
///		\ret  新添加的画刷序号
///		\note 随机生成画刷，添加在当前激活的画刷之后
int CZActiveState::addNewBrush(int idx /* = -1*/)
{
	CZStampGenerator *gen= getGenerator(idx)->copy();
	gen->randomize();

	CZBrush* random = new CZBrush(gen);
	random->weight.value = CZUtil::RandomFloat() * 56 + 44;
	random->intensity.value = 0.15f;
	random->spacing.value = 0.02;

	int activeBrushIdx = eraseMode ? eraseBrushIdx : paintBrushIdx;

	brushes.insert(brushes.begin()+(activeBrushIdx+1),random);
	
	/// update eraseBrushIdx & paintBrushIdx and keep activeBrushIdx unchanged 
	if(paintBrushIdx>activeBrushIdx)	paintBrushIdx++;
	if(eraseBrushIdx>activeBrushIdx)	eraseBrushIdx++;

	return activeBrushIdx+1;
}

/// 设置当前激活画刷
///
///		\param idx - 可用画刷的序号，当非法时默认为0
///		\ret	   - 原来的激活画刷序号
///		\note	当前设置的画刷种类由模式决定
int CZActiveState::setActiveBrush(int idx)
{
	int oldIdx = eraseMode ? eraseBrushIdx : paintBrushIdx;

	if (idx < 0 || idx >= brushes.size()) 
	{
		LOG_WARN("idx is out of range!\n");
		return oldIdx;
	}

	if (eraseMode)	eraseBrushIdx = idx;
	else			paintBrushIdx = idx;

	return oldIdx;
}

/// 获取当前画刷
/// 
///		\note 画刷种类由当前的状态eraseMode决定
CZBrush *CZActiveState::getActiveBrush()
{
	CZBrush * ret = eraseMode ? brushes[eraseBrushIdx] : brushes[paintBrushIdx];

	return ret;
}

/// 删除当前画刷
bool CZActiveState::deleteActiveBrush()
{
	if(brushes.size() == 1) 
	{
		LOG_WARN("there must be at least one brush!\n");
		return false;
	}
	
	int activeBrushIdx = eraseMode ? eraseBrushIdx : paintBrushIdx;
	vector<CZBrush*>::iterator itr = brushes.begin() + activeBrushIdx;

	/// in case that the last brush has been deleted
	if(activeBrushIdx == brushes.size())	activeBrushIdx --;
	/// update eraseBrushIdx & paintBrushIdx 
	if(paintBrushIdx>=activeBrushIdx)	paintBrushIdx--;
	if(eraseBrushIdx>=activeBrushIdx)	eraseBrushIdx--;

	delete *itr;
	brushes.erase(itr);

	return true;
}

/// 获取当前工具
/// 
///		\note 工具种类由当前的状态eraseMode决定
CZTool* CZActiveState::getActiveTool()
{
	return eraseMode ? tools[1] : tools[0];
}

/// 获取一个笔刷生成器
///
///		\param idx - 添加的画刷类型序号，当idx为负数时随机化序号
CZStampGenerator * CZActiveState::getGenerator(int idx /* = -1*/)
{
	int genNum = generators.size();
	if (genNum > 0 && idx < genNum)
	{
		if(idx < 0)	idx = rand() % genNum;
		for(map<string,CZStampGenerator*>::iterator itr = generators.begin(); itr != generators.end(); itr++)
			if(idx-- == 0) return itr->second;
	}

	LOG_ERROR("generator candidates is none or idx is larger than genNum!\n");
	return NULL;
}

/// 建立生成器
int CZActiveState::setUpGenerators()
{
	generators["spiral"] = new CZSpiralGenerator(stampGLContext);
	generators["radialFade"] = new CZRoundGenerator(stampGLContext);

	return generators.size();
}

/// 初始化画刷
int CZActiveState::initBrushes()
{
	int brushNum = 2;

	eraseBrushIdx = paintBrushIdx = -1;
	for(int i=0; i<brushNum; i++)	addNewBrush(i);
	
	eraseBrushIdx = paintBrushIdx = 1;
	
	return brushNum;
}
