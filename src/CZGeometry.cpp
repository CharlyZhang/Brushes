
///  \file CZGeometry.cpp
///  \brief This is the file implement a series of geometry Class.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-11
///  \note

#include "CZGeometry.h"
#include "CZUtil.h"
#include <cmath>

//////////////////////////////////////////////////////////////////////////

/// 重载运算符
bool CZ2DPoint::operator <( const  CZ2DPoint& p_) const
{
	if ( this->x < p_.x )
	{
		return true;
	} 
	else if ( this->x==p_.x && this->y < p_.y )
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool CZ2DPoint::operator ==( const  CZ2DPoint& p_) const
{
	if ( this->x==p_.x && this->y==p_.y)
	{
		return true;
	} 
	else
	{
		return false;
	}
}
void CZ2DPoint::operator =( const  CZ2DPoint& p_) 
{
	this->x = p_.x; 
	this->y = p_.y;
}

/// 求到另外一点距离
float CZ2DPoint::distanceTo2DPoint(CZ2DPoint & p_)
{ 
	return sqrt((x-p_.x)*(x-p_.x) + (y-p_.y)*(y-p_.y)); 
}

//////////////////////////////////////////////////////////////////////////

/// 判断是否为零矩形
bool CZRect::isZeroRect() const
{
	if(origin.x==0.0f && origin.y==0.0f
		&& size.width ==0.0f && size.height ==0.0f)
		return true;
	else
		return false;
}

/// 合并其他矩形
CZRect CZRect::unionWith(const CZRect & rect_)
{
	if(isZeroRect())			return rect_;
	else if(rect_.isZeroRect()) return *this;

	float minX = Min(this->getMinX(), rect_.getMinX());
	float minY = Min(this->getMinY(), rect_.getMinY());
	float maxX = Max(this->getMaxX(), rect_.getMaxX());
	float maxY = Max(this->getMaxY(), rect_.getMaxY());

	return CZRect(minX, minY, maxX-minX, maxY-minY);
}

/// 扩展的整数矩形
CZRect CZRect::expandToIntergral()
{
	float minX = floor(this->getMinX());
	float minY = floor(this->getMinY());
	float maxX = ceil(this->getMaxX());
	float maxY = ceil(this->getMinY());

	return CZRect(minX, minY, maxX-minX, maxY-minY);
}