#include "CZBrush.h"
#include <iostream>

/// ��ȡͼ������
CZTexture *CZBrush::getTexture(bool isSmallOne /* = false */)
{
	if (generator == NULL)
	{
		std::cerr << "CZBrush::getTexture - generator is NULL\n";
		return NULL;
	}

	return generator->getStamp();
}
