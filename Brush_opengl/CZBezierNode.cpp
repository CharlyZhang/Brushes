
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

CZBezierNode::~CZBezierNode(){}
CZBezierNode::CZBezierNode(const CZ3DPoint & anchor_)
{
	inPoint = anchor_;
	anchorPoint = anchor_;
	outPoint = anchor_;
}
