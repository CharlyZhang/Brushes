
///  \file CZBezierNode.h
///  \brief This is the file declare the Class CZBezierNode.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-15
///  \note

#ifndef _CZBEZIERNODE_H_
#define _CZBEZIERNODE_H_

#include "CZ3DPoint.h"

class CZBezierNode
{
public:
	/// ê������������Ƶ�
	CZ3DPoint inPoint;
	CZ3DPoint anchorPoint;
	CZ3DPoint outPoint;

	float inPressure;
	float outPressure;
	float anchorPressure;

	CZBezierNode(){}
	CZBezierNode(const CZ3DPoint & anchor_);
	CZBezierNode(CZ3DPoint in_, CZ3DPoint anchor_, CZ3DPoint out_):inPoint(in_),outPoint(out_),anchorPoint(anchor_){};
	~CZBezierNode();
};
#endif