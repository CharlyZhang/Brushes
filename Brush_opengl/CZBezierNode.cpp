
///  \file CZBezierNode.cpp
///  \brief This is the file implement the Class CZBezierNode.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-15
///  \note

#include "CZBezierNode.h"

CZBezierNode::~CZBezierNode()
{
	if (this->inPoint)	{ delete this->inPoint; this->inPoint = NULL; }
	if (this->outPoint) { delete this->outPoint; this->outPoint = NULL; }
	if (this->anchorPoint)	{ delete this->anchorPoint; this->anchorPoint = NULL; }
}

/// 判断是否与另外一指针指向的对象相等
bool CZBezierNode::isEqual(CZBezierNode *ptrNode_)
{
	if(ptrNode_ == this) return true;
	if(!ptrNode_)		 return false;

	return (this->inPoint->isEqual(ptrNode_->inPoint) &&
			this->anchorPoint->isEqual(ptrNode_->anchorPoint) &&
			this->outPoint->isEqual(ptrNode_->outPoint));
}