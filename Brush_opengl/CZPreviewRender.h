
///  \file CZPreviewRender.h
///  \brief This is the file declare the class CZPreviewRender.
///
///		�����ˢԤ��ͼ�Ļ��ơ�
///
///  \version	1.0.1
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-14
///  \note


#ifndef _CZPREVIEWRENDER_H_
#define _CZPREVIEWRENDER_H_

#include "CZRender.h"
#include "CZImage.h"
#include "CZUtil.h"
#include <string>
#include <vector>

class CZPreviewRender :public CZRender
{
public:
	CZPreviewRender();
	~CZPreviewRender();
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

	/// ���ñ�ˢ����
	void configureBrush(CZImage *img);
	/// �����ˢ����
	void clearBrush();

private:
	/// ���ƹ켣���ݣ�����ͼ�νӿڣ�
	void drawPathData(vertexData *data, unsigned int n);

private:
	CZShader *brushShader;
	CZTexture *brushTexture;

};

#endif