
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
#include "CZFbo.h"
#include "CZShader.h"
#include "CZTexture.h"

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
	/// ������Ԥ��ͼ������FBO���������ɻ��ƵĹ켣��
	void setup(const CZSize &size_);
	/// �����켣������һ��sin���ߣ�
	void buildPath();
	/// ���û�ˢ������shader��������
	void configureBrush();

public:
	/// ��ɵ�����ȡ����
	static CZBrushPreview * getInstance()
	{
		static CZBrushPreview instance;   //�ֲ���̬����  
		return &instance; 
	}

	/// ��Ա����
	CZPath *path;							///< ���ƵĹ켣
	CZBrush *ptrBrush;						///< �����ã���������������
	CZShader *brushShader;					
	CZFbo *fbo;
	CZTexture *tex;							///< ����Ԥ��ͼ������		
	float backingWidth, backingHeight;

	
	/// չ��ָ���ߴ��СԤ��ͼ
	CZTexture *previewWithSize(const CZSize &size_);
	/// ���û�ˢ
	void setBrush(CZBrush *brush_);
};