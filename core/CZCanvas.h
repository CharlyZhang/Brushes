
///  \file CZCanvas.h
///  \brief This is the file declare the class CZCanvas.
///
///	
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note

#ifndef _CZCANVAS_H_
#define _CZCANVAS_H_

#include "basic/CZ2DPoint.h"
#include "basic/CZRect.h"
#include "basic/CZSize.h"

class CZCanvas
{
public:
	/// ������ͼ
	void drawView();

private:
	/// ��һ�����������ͼ
	void drawViewInRect(CZRect &rect);

private:
	CZRect visibleRect;
};

#endif