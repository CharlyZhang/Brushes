
///  \file CZCanvas.cpp
///  \brief This is the file implements the class CZCanvas.
///
///	
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note

#include "CZCanvas.h"
#include "CZDefine.h"
#include "CZActiveState.h"
#include "basic/CZMat4.h"
#include "painting/CZPainting.h"
#include "tool/CZFreehandTool.h"
#include "graphic/glDef.h"

/// CZView, ƽ̨��ص���ͼ
class CZViewImpl : public CZView
{
public:
	CZViewImpl(const CZRect rect){}
	~CZViewImpl(){};
	void setPaiting(CZPainting* p){};
	void draw(){};
};

/// implemention of CZCanvas
CZCanvas::CZCanvas(const CZRect rect)
{
	ptrPainting = NULL;
	view = new CZViewImpl(rect);
	isDirty = false;
}

CZCanvas::~CZCanvas()
{
	delete view;
}

/// set painting
bool CZCanvas::setPaiting(CZPainting *p)
{
	if (!p) {
		LOG_ERROR("painting is NULL\n");
		return false;
	}

	ptrPainting = p;
	p->setCanvas(this);
	CZActiveState::getInstance()->setPainting(p);
	view->setPaiting(p);
	return true;
}

/// ������ͼ
void CZCanvas::drawView()
{
	//    if (!ptrPainting) {
	//        LOG_ERROR("painting is NULL\n");
	//        return;
	//    }
#if USE_OPENGL_ES
	view->draw();
#elif USE_OPENGL
	isDirty = true;
#endif
	//drawViewInRect(visibleRect);
}

void* CZCanvas::getView()
{
#if USE_OPENGL_ES
	CZViewImpl *thisView = (CZViewImpl*)view;
	return (void*)thisView->realView;
#endif
	return NULL;
}

/// ��һ�����������ͼ
void CZCanvas::drawViewInRect(CZRect &rect)
{

}
