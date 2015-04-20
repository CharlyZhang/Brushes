
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
#include "painting/CZPainting.h"

/// CZView, ƽ̨��ص���ͼ
struct CZCanvas::CZView
{
	CZView(){};
	~CZView(){};
};

CZCanvas::CZCanvas()
{
	view = new CZView;
}

CZCanvas::~CZCanvas()
{
	delete view;
}

/// ������ͼ
void CZCanvas::drawView()
{
	drawViewInRect(visibleRect);
}

/// ��һ�����������ͼ
void CZCanvas::drawViewInRect(CZRect &rect)
{

}
