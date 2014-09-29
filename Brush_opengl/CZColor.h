
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
	CZColor(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f){};
	/// 返回特定颜色的静态函数
	static CZColor black()	{ return CZColor(0.0f,0.0f,0.0f,1.0f);}

	void update(CZDecoder *decoder_, bool deep = false){};
	void encode(CZCoder *coder_){};

	float hue;				///< 色调
	float saturation;		///< 饱和度
	float brightness;		///< 亮度
	float alpha;		
	float red;
	float green;
	float blue;
};

#endif