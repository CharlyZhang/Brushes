
///  \file CZBrushPreview.h
///  \brief This is the file declare the Class CZBrushPreview.
///
///		����һ��������չ�ֱ�ˢ��Ԥ��ͼ.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-16
///  \note

#include "CZPath.h"

class CZBrushPreview
{
private:
	CZBrushPreview(){ initial(); }   //���캯����˽�е�
	CZBrushPreview(const CZBrushPreview &);
	CZBrushPreview & operator = (const CZBrushPreview &);

public:
	/// ��ɵ�����ȡ����
	static CZBrushPreview *pInstance;
	static CZBrushPreview * getInstance()
	{
		if(pInstance == NULL)  pInstance = new CZBrushPreview();  
		return pInstance;
	}

	/// ��Ա����
	CZPath *path;
	CZBrush *brush;
	float backingWidth, backingHeight;

	/// ��ʼ������
	bool initial();
	/// ����·��������һ��sin���ߣ�
	void buildPath();
	/// չ��ָ���ߴ��СԤ��ͼ
	void previewWithSize(const CZSize &size_);
};