
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
	CZ3DPoint *inPoint;
	CZ3DPoint *anchorPoint;
	CZ3DPoint *outPoint;

	float inPressure;
	float outPressure;
	float anchorPressure;

	CZBezierNode(CZ3DPoint *in=NULL, CZ3DPoint *anchor=NULL, CZ3DPoint *out=NULL):inPoint(in),outPoint(out),anchorPoint(anchor){};
	~CZBezierNode();

	/// 判断是否与另外一指针指向的对象相等
	bool isEqual(CZBezierNode *ptrNode_);
};
#endif