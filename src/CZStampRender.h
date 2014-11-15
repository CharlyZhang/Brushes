
///  \file CZStampRender.h
///  \brief This is the file declares the Class CZStampRender.
///
///		�����������Ʊ�ˢͼ�εĻ��������Ǹ�������
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note		ֻ�õ����߻���

#ifndef _CZSTAMPRENDER_H_
#define _CZSTAMPRENDER_H_

#include "CZRender.h"
#include "CZGeometry.h"
#include "CZ3DPoint.h"
#include "CZMat4.h"
#include <vector>

class CZStampGenerator;

class CZStampRender :public CZRender
{
public:
	/// ��ɵ�����ȡ����
	static CZStampRender * getInstance()
	{
		static CZStampRender instance;   //�ֲ���̬����  
		return &instance; 
	}
	/// ���ô�С
	void configure(int w, int h);
	/// ����stampͼ��
	CZImage *drawStamp();
	
	/// ����������
	void drawSpiralData(std::vector<CZ2DPoint> &points);
	void drawSpiralData(std::vector<CZ3DPoint> &points);

private:
	CZStampRender();  //���캯����˽�е�
	CZStampRender(const CZStampRender &);
	CZStampRender & operator = (const CZStampRender &);
	~CZStampRender();

public:
	CZStampGenerator *ptrGenerator;		///< ��ǰ���Ƶ�������
private:
	CZMat4 projMat;						///< ͶӰ����
	CZShader *shader;					///< ��ɫ��
};

#endif