
///  \file CZColor.h
///  \brief This is the file declare the class CZColor.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-25
///  \note

#ifndef _CZCOLOR_H_
#define _CZCOLOR_H_

#include "CZCoding.h"

class CZColor: public CZCoding 
{
public:
	float hue;				///< ɫ��
	float saturation;		///< ���Ͷ�
	float brightness;		///< ����
	float alpha;		
	float red;
	float green;
	float blue;
};

#endif