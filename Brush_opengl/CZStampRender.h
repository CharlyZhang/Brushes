
///  \file CZStampRender.h
///  \brief This is the file declare the Class CZStampRender.
///
///		�����������Ʊ�ˢͼ�εĻ��������Ǹ�������
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note

#ifndef _CZSTAMPRENDER_H_
#define _CZSTAMPRENDER_H_

#include "CZRender.h"
#include "CZGeometry.h"
#include "CZ3DPoint.h"
#include <vector>

class CZStampRender :public CZRender
{
public:
	/// ��ɵ�����ȡ����
	static CZStampRender * getInstance()
	{
		static CZStampRender instance;   //�ֲ���̬����  
		return &instance; 
	}

	/// ���û�����Ϣ���ı��ڲ�������
	void configure(std::map<std::string,void*> &conf);
	/// ��ʼ���ƣ�ִ�о���Ļ��Ƶ��ã�
	void begin(DrawType type);
	/// ���ýӿڻ���
	void draw(DrawType type, void* data = NULL, unsigned int num = 0);
	/// ����ͼ�����ݲ�����
	CZImage* imageForLastDraw();
	/// ��������
	void end(DrawType type);

	/// ����������
	void drawSpiralData(std::vector<CZ2DPoint> &points);
	void drawSpiralData(std::vector<CZ3DPoint> &points);
private:
	CZStampRender();   
	CZStampRender(const CZStampRender &);
	CZStampRender & operator = (const CZStampRender &);
	~CZStampRender(){ }

};

#endif