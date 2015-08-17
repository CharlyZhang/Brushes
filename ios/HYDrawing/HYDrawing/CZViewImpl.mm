//
//  CZViewImpl.cpp
//  HYDrawing
//
//  Created by CharlyZhang on 15/8/12.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#include "CZViewImpl.h"


////////////////////CanvasView////////////////////
CZViewImpl::CZViewImpl(const CZRect rect)
{
    realView = [[CanvasView alloc]initWithFrame:CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height)];
}

CZViewImpl::~CZViewImpl() {}
void CZViewImpl::setPaiting(CZPainting* p) { [realView setPainting:(void*)p];}
void CZViewImpl::draw() { [realView drawView];}



