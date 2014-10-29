
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

/// ��ȡ������ɫ
CZColor CZActiveState::getPaintColor()
{
	return paintColor;
}

/// ���ÿ��û�ˢ
void CZActiveState::setBrushes(std::vector<CZBrush*> &bs)
{
	for(vector<CZBrush*>::iterator itr = bs.begin(); itr != bs.end(); itr++)
		brushes.push_back(*itr);
}

/// ��ӻ�ˢ
/// 
///		\note ����ڵ�ǰ����Ļ�ˢ֮��
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

/// ��ÿ��û�ˢ
vector<CZBrush*>& CZActiveState::getBrushes()
{
	return brushes;
}

/// ���õ�ǰ���ˢ
///
///		\param idx - ���û�ˢ����ţ����Ƿ�ʱĬ��Ϊ0
///		\note	��ǰ���õĻ�ˢ������ģʽ����
void CZActiveState::setActiveBrush(int idx)
{
	if (idx < 0 || idx >= brushes.size()) return;
	
	if (eraseMode)
		ptrEraseBrush = brushes[idx];
	else
		ptrPaintBrush = brushes[idx];
}

/// ��ȡ��ǰ��ˢ
/// 
///		\note ��ˢ�����ɵ�ǰ��״̬eraseMode����
CZBrush *CZActiveState::getActiveBrush()
{
	CZBrush * ret = eraseMode ? ptrEraseBrush : ptrPaintBrush;

	return ret;
}

/// ɾ����ǰ��ˢ
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

/// ��ȡ��ǰ����
/// 
///		\note ���������ɵ�ǰ��״̬eraseMode����
CZTool* CZActiveState::getActiveTool()
{
	return eraseMode ? tools[1] : tools[0];
}

/// ��ȡ���һ����ˢ������
CZStampGenerator * CZActiveState::getRandomGenerator()
{
	return new CZSpiralGenerator;
}

