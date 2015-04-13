
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
#include "CZFreehandTool.h"
#include "CZEraserTool.h"
#include "graphic/CZGLContext.h"

using namespace std;

CZActiveState::CZActiveState()
{
	eraseMode = false;
	paintColor = CZColor::blackColor();
	stampGLContext = new CZGLContext;
	//setUpGenerators()
	generators["test"] = new CZSpiralGenerator(stampGLContext);
	brushes.push_back(new CZBrush(getRandomGenerator()->copy()));  /// if invoke CZBrush::randomBrush, the CZActiveState constructor will be called again
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
///		\ret	   - ԭ���ļ��ˢ���
///		\note	��ǰ���õĻ�ˢ������ģʽ����
int CZActiveState::setActiveBrush(int idx)
{
	int oldIdx = eraseMode ? indexOfBrushes(ptrEraseBrush) : indexOfBrushes(ptrPaintBrush);

	if (idx < 0 || idx >= brushes.size()) return oldIdx;
	

	if (eraseMode)
		ptrEraseBrush = brushes[idx];
	else
		ptrPaintBrush = brushes[idx];

	return oldIdx;
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

/// ��ȡ���һ����ˢ������~
CZStampGenerator * CZActiveState::getRandomGenerator()
{
	if (generators.size())
	{
		return generators["test"];
	}

	return NULL;
}


/// ��û�ˢ�����л�ˢ�еı�ţ������ڷ��ظ�ֵ
int CZActiveState::indexOfBrushes(CZBrush *brush)
{
	int ret;
	int num = brushes.size();
	for(ret = 0; ret < num; ret ++)
		if(brush == brushes[ret]) return ret;

	return -1;
}

