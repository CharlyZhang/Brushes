
#ifndef _CZACTIVESTATE_H_
#define _CZACTIVESTATE_H_

#include "CZSpiralGenerator.h"

class CZActiveState
{
public:
	/// ��ɵ�����ȡ����
	static CZActiveState * getInstance()
	{
		static CZActiveState instance;   //�ֲ���̬����  
		return &instance; 
	}

private:
	CZActiveState(){ initial(); }   //���캯����˽�е�
	CZActiveState(const CZActiveState &);
	CZActiveState & operator = (const CZActiveState &);
	~CZActiveState(){ destroy(); }

	/// ��ʼ������
	bool initial(){ brush = new CZBrush(new CZSpiralGenerator); return true; };
	/// ���ٺ���
	bool destroy() { if(brush != NULL) {delete brush; brush = NULL;} return true;};

public:
	CZBrush *brush;
};

#endif