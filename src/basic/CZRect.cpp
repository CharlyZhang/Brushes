
#include "CZRect.h"
#include "../CZUtil.h"

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