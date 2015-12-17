
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
#include "basic/CZMat4.h"
#include "CZActiveState.h"
#include "CZUtil.h"

/// implemention of CZCanvas
CZCanvas::CZCanvas(CZView *view_)
{
    ptrPainting = NULL;
    if (!view_) {
        LOG_ERROR("view is NULL\n");
    }
	view = view_;
    view->setCanvas(this);
    
    canvasTransform = CZAffineTransform::makeIdentity();
    canvasScale = 1.f;
    
    CZRect rect = view->getFrame();
    deviceSpacePivot = CZ2DPoint(rect.size.width / 2.f, rect.size.height / 2.f);
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
    
    CZSize size = ptrPainting->getDimensions();
    userSpacePivot = CZ2DPoint(size.width / 2.0f, size.height / 2.0f);
    
    rebuildViewTransformAndRedraw(false);
    
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


/// Methods for CanvasView
void CZCanvas::setScale(float s)
{
    canvasScale = s;
}

float CZCanvas::getScale()
{
    return canvasScale;
}

void CZCanvas::rebuildViewTransformAndRedraw(bool flag)
{
    float screenScale = CZActiveState::getInstance()->mainScreenScale;
    
    canvasTransform = CZAffineTransform::makeIdentity();
    
    canvasTransform.translate(deviceSpacePivot.x, deviceSpacePivot.y);
    canvasTransform.scale(canvasScale / screenScale, -canvasScale / screenScale);
    canvasTransform.translate(-userSpacePivot.x, -userSpacePivot.y);
    
    canvasTransform.tx = roundf(canvasTransform.tx);
    canvasTransform.ty = roundf(canvasTransform.ty);
    
    if(flag)    view->draw();
}

CZ2DPoint CZCanvas::transformToPainting(const CZ2DPoint &pt)
{
//    CZAffineTransform iTx = canvasTransform.getInvert();

    CZMat4 mat;
    mat.LoadFromAffineTransform(canvasTransform);
    mat.Invert();
    VECTOR3D p = mat * VECTOR3D(pt.x,pt.y,0.f);
    VECTOR3D po = mat * p;
    CZ2DPoint ret(p.x,p.y);
    return ret;
}

void CZCanvas::pinchBegin(CZ2DPoint &pt)
{
    deviceSpacePivot = pt;
    userSpacePivot = transformToPainting(pt);
    
    correction = CZ2DPoint(0.f,0.f);
}

void CZCanvas::pinchChanged(CZ2DPoint &pt,  bool touchCountChanged /* = false */)
{
    if (touchCountChanged)
    {
        correction = CZ2DPoint(deviceSpacePivot.x - pt.x, deviceSpacePivot.y - pt.y);
    }
    
    deviceSpacePivot = CZ2DPoint(pt.x + correction.x, pt.y + correction.y);
}

CZMat4 CZCanvas::getTransformMatrix()
{
    CZMat4 effectiveProj;
    effectiveProj.LoadFromAffineTransform(canvasTransform);
    return effectiveProj;
}
/////////////////////////////////////////////////////////////

/// 在一定区域绘制视图
void CZCanvas::drawViewInRect(CZRect &rect)
{
    
}


