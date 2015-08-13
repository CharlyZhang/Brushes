
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
#include "painting/CZPainting.h"
#include "CZActiveState.h"


/// implemention of CZCanvas
CZCanvas::CZCanvas(CZView *view_)
{
    ptrPainting = NULL;
    if (!view_) {
        LOG_ERROR("view is NULL\n");
    }
	view = view_;
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

/// 绘制视图
void CZCanvas::drawView()
{
//    if (!ptrPainting) {
//        LOG_ERROR("painting is NULL\n");
//        return;
//    }

    view->draw();
	//drawViewInRect(visibleRect);
}

/// 在一定区域绘制视图
void CZCanvas::drawViewInRect(CZRect &rect)
{

}

