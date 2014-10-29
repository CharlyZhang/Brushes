
#ifndef _CZERASERTOOL_H_
#define _CZERASERTOOL_H_

#include "CZTool.h"

class CZEraserTool : public CZTool
{
public:
	/// ��ʼ�ƶ������֧��ѹ���������Ϊѹ��ֵ��
	void moveBegin(CZ2DPoint &p_, float pressure_ = 0.0f){};

	/// �����ƶ�
	///		
	///		������5���㴦��ÿ����һ�λ���һ�顣
	///
	///		/param p_				  - ��ǰ�ƶ��ĵ�λ��
	///		/param pressureOrSpeed	  - ���豸֧��ѹ��ֵʱ��Ϊѹ��ֵ������Ϊ�ƶ��ٶ�ֵ(pixels/millisecond)
	void moving(CZ2DPoint &p_, float pressureOrSpeed){};

	/// �ƶ�����
	void moveEnd(CZ2DPoint &p_){};
};

#endif