
///  \file CZTool.h
///  \brief This is the file declare the Class CZTool.
///
///		The CZTool is designed for input, it's the super class of all input classes. 
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-28
///  \note

#ifndef _CZTOOL_H_
#define _CZTOOL_H_

#include "CZGeometry.h"

class CZTool 
{
public:
	/// ��ʼ�ƶ������֧��ѹ���������Ϊѹ��ֵ��
	virtual void moveBegin(CZ2DPoint &p_, float pressure_ = 0.0f) = 0;

	/// �����ƶ�
	///		
	///		������5���㴦��ÿ����һ�λ���һ�顣
	///
	///		/param p_				  - ��ǰ�ƶ��ĵ�λ��
	///		/param pressureOrSpeed	  - ���豸֧��ѹ��ֵʱ��Ϊѹ��ֵ������Ϊ�ƶ��ٶ�ֵ(pixels/millisecond)
	virtual void moving(CZ2DPoint &p_, float pressureOrSpeed) = 0;

	/// �ƶ�����
	virtual void moveEnd(CZ2DPoint &p_) = 0;

protected:
	bool moved;
};


#endif
