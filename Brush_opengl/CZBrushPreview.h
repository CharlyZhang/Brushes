
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
	~CZBrushPreview(){ destroy(); }

	/// ��ʼ������
	bool initial();
	/// ���ٺ���
	bool destroy();
	/// ������Ԥ��ͼ�����ɻ��ƵĹ켣��
	void setup(const CZSize &size_);
	/// �����켣������һ��sin���ߣ�
	void buildPath();

public:
	/// ��ɵ�����ȡ����
	static CZBrushPreview * getInstance()
	{
		static CZBrushPreview instance;   //�ֲ���̬����  
		return &instance; 
	}

	/// ��Ա����
	CZPath *path;
	CZBrush *brush;						///< �����ã���������������
	float backingWidth, backingHeight;

	
	/// չ��ָ���ߴ��СԤ��ͼ
	void previewWithSize(const CZSize &size_ = CZSize(0,0));
	/// ���û�ˢ
	void setBrush(CZBrush *brush_);
};