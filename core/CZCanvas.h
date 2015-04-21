
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

#include "basic/CZRect.h"

class CZPainting;

/// protocol CZView
class CZView
{
public:
    virtual ~CZView(){};
    virtual void setPaiting(CZPainting* p) = 0;
    virtual void draw() = 0;
};

class CZCanvas
{
public:
	CZCanvas(const CZRect rect);
	~CZCanvas();

	/// ���û���
	bool setPaiting(CZPainting* p);
	/// ������ͼ
	void drawView();
    ///
    void* getView();

private:
	/// ��һ�����������ͼ
	void drawViewInRect(CZRect &rect);
public:
	CZPainting *ptrPainting;

private:
	CZRect visibleRect;
	CZView *view;
};

#endif