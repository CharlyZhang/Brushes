
///  \file CZRender.h
///  \brief This is the file declare the Class CZRender.
///
///		�����������ƣ�ÿ��render���е�����context,�������ء�
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-10
///  \note

#ifndef _CZRENDER_H_
#define _CZRENDER_H_

#include "CZFbo.h"

class CZRender
{
public:
	CZRender(){
		/*
		self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

		if (!context || ![EAGLContext setCurrentContext:context]) {
		return nil;
		}
		*/
	};
	~CZRender()
	{
	};
	/// ��ʼ������״̬
	virtual void init() = 0;
	/// ���û�������
	virtual void configure(int w, int h) = 0;
	/// ��ʼ����
	virtual void begin() = 0;
	/// ��������
	virtual void end() = 0;

protected:
	int width, height;
	CZFbo fbo;

};


#endif