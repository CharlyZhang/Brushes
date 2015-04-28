
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

/// ���û���ģʽ
void CZActiveState::setEraseMode(bool flag)
{
	eraseMode = false;
}

/// ��ȡ����ģʽ
bool CZActiveState::isEraseMode()
{
	return eraseMode;
}

/// ���û�����ɫ
void CZActiveState::setPaintColor(CZColor &pc)
{
	paintColor = pc;
}
void CZActiveState::setPaintColor(float r, float g, float b, float a /* = 1.0f */)
{
	paintColor = CZColor(r,g,b,a);
}

/// ��ȡ������ɫ
CZColor CZActiveState::getPaintColor()
{
	return paintColor;
}

/// ����»�ˢ
/// 
///		\ret  ����ӵĻ�ˢ���
///		\note ������ɻ�ˢ������ڵ�ǰ����Ļ�ˢ֮��
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

/// ���õ�ǰ���ˢ
///
///		\param idx - ���û�ˢ����ţ����Ƿ�ʱĬ��Ϊ0
///		\ret	   - ԭ���ļ��ˢ���
///		\note	��ǰ���õĻ�ˢ������ģʽ����
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

/// ��ȡ��ǰ��ˢ
/// 
///		\note ��ˢ�����ɵ�ǰ��״̬eraseMode����
CZBrush *CZActiveState::getActiveBrush()
{
	CZBrush * ret = eraseMode ? brushes[eraseBrushIdx] : brushes[paintBrushIdx];

	return ret;
}

/// ɾ����ǰ��ˢ
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

/// ��ȡ��ǰ����
/// 
///		\note ���������ɵ�ǰ��״̬eraseMode����
CZTool* CZActiveState::getActiveTool()
{
	return eraseMode ? tools[1] : tools[0];
}

/// ��ȡһ����ˢ������
///
///		\param idx - ��ӵĻ�ˢ������ţ���idxΪ����ʱ��������
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

/// ����������
int CZActiveState::setUpGenerators()
{
	generators["spiral"] = new CZSpiralGenerator(stampGLContext);
	generators["radialFade"] = new CZRoundGenerator(stampGLContext);

	return generators.size();
}

/// ��ʼ����ˢ
int CZActiveState::initBrushes()
{
	int brushNum = 2;

	eraseBrushIdx = paintBrushIdx = -1;
	for(int i=0; i<brushNum; i++)	addNewBrush(i);
	
	eraseBrushIdx = paintBrushIdx = 1;
	
	return brushNum;
}
