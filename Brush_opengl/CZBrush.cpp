#include "CZBrush.h"
#include <iostream>

/// 获取图案纹理
CZTexture *CZBrush::getTexture(bool isSmallOne /* = false */)
{
	if (generator == NULL)
	{
		std::cerr << "CZBrush::getTexture - generator is NULL\n";
		return NULL;
	}

	return generator->getStamp();
}
