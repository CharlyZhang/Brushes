
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
	float hue;				///< 色调
	float saturation;		///< 饱和度
	float brightness;		///< 亮度
	float alpha;		
	float red;
	float green;
	float blue;
};

#endif