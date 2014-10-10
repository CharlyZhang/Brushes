
///  \file CZBrushPreview.h
///  \brief This is the file declare the Class CZBrushPreview.
///
///		����һ��������չ�ֱ�ˢ��Ԥ��ͼ��
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-16
///  \note

#include "CZPath.h"
#include "CZFbo.h"
#include "CZShader.h"
#include "CZTexture.h"
#include "CZStampRender.h"
#include "CZImage.h"

class CZBrushPreview : public CZBrushDelegate
{
public:
	/// ��ɵ�����ȡ����
	static CZBrushPreview * getInstance()
	{
		static CZBrushPreview instance;   //�ֲ���̬����  
		return &instance; 
	}

	/// չ��ָ���ߴ��СԤ��ͼ
	///		
	///		�������ָ���Ĵ�С�����Ƴ���Ӧ��������Ҫ�����ڲ���Դ�Ľ����Լ����ʹ켣�Ļ��ơ�
	///		���������������������ⲿ���ƣ�����ֻ�Բ����еĳߴ�Ϊ׼��
	///		��Ҫ����OpenGL��FBO��Shader��
	/// 
	///		/param size - ����Ԥ���ĳߴ��С
	///		/return		- ���Ƶõ��ı�ˢ�켣ͼ
	///		/note		ʹ��֮ǰӦ���ȵ���setBrush�趨��ˢ��
	CZImage *previewWithSize(CZSize size_);
	/// ���û�ˢ
	void setBrush(CZBrush *brush_);
	/// �����豸��Ļ�ֱ��ʱ���
	void setMainScreenScale(float s);
	/// ��ȡ��ˢ����
	CZTexture *getBrushTexture();

	/// ʵ��Brush�ı��ί�нӿ�
	void brushPropertyChanged(std::vector<CZProperty> &properties);	///< �ı�����ʱʵ�ָýӿ�
	void brushGeneratorChanged(CZStampGenerator &gen);				///< �ı���滻������ʱ��ʵ�ָýӿ�

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
	/// ��Ա����
	CZPath *path;							///< ���ƵĹ켣
	CZShader *brushShader;					
	CZTexture *tex;							///< ����Ԥ��ͼ������	
	CZTexture *brushTexture;				///< ��ˢ�������������ı���ı䣨��ί�л���ʵ�֣� 
	float backingWidth, backingHeight;

private:
	CZBrush *ptrBrush;						///< �����ã���������������
	float mainScreenScale;					///< �豸��Ļ�ķֱ��ʱ�������PPI��ÿӢ�������������
	CZStampRender render;					///< ������
};