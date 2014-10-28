
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
#include "CZPath.h"
#include "CZMat4.h"
#include "CZCommon.h"

class CZBrushPreview;

class CZPreviewRender :public CZRender
{
public:
	CZPreviewRender();
	~CZPreviewRender();
	/// ���û�����Ϣ
	void configure(int w, int h);
	/// ����һ���켣,������ͼ��
	CZImage *drawPath(CZPath *path);
	/// ���ñ�ˢ����
	void configureBrush(CZImage *img);
	/// �����ˢ����
	void clearBrush();

private:
	CZShader		*brushShader;
	CZTexture		*brushTexture;
	CZMat4			projMat;		///< ���Ƶ�ͶӰ����
	CZBrushPreview	*ptrPreview;	///< ָ���������Ƶ�preview
};

#endif