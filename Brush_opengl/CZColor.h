
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
	/// �����ض���ɫ�ľ�̬����
	static CZColor black()	{ return CZColor(0.0f,0.0f,0.0f,1.0f);}

	void update(CZDecoder *decoder_, bool deep = false){};
	void encode(CZCoder *coder_){};

	float hue;				///< ɫ��
	float saturation;		///< ���Ͷ�
	float brightness;		///< ����
	float alpha;		
	float red;
	float green;
	float blue;
};

#endif