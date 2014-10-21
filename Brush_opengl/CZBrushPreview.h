
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
#include "CZPreviewRender.h"
#include "CZImage.h"
#include "CZNotificationCenter.h"

class CZBrushPreview : public CZObserver
{
public:
	/// ��ɵ�����ȡ����
	static CZBrushPreview * getInstance()
	{
		static CZBrushPreview instance;   //�ֲ���̬����  
		return &instance; 
	}

	/// ����ָ���ߴ��СԤ��ͼ
	///		
	///		�������ָ���Ĵ�С�����Ƴ���Ӧ��ͼ����Ҫ�����ڲ���Դ�Ľ����Լ����ʹ켣�Ļ��ơ�
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
	/// ��ȡ��ˢͼ��
	CZImage *getBrushImage();

	/// ʵ��Observer�ӿ�
	void updateObserver(std::string &notificationName, void* data = NULL);

private:
	CZBrushPreview(){ initial(); }   //���캯����˽�е�
	CZBrushPreview(const CZBrushPreview &);
	CZBrushPreview & operator = (const CZBrushPreview &);
	~CZBrushPreview(){ destroy(); }

	/// ��ʼ������
	bool initial();
	/// ���ٺ���
	bool destroy();
	/// ������Ԥ��ͼ�����û����������ɻ��ƵĹ켣��
	void setup(const CZSize &size_);
	/// �����켣������һ��sin���ߣ�
	void buildPath();

public:
	/// ��Ա����
	CZPath *path;							///< ���ƵĹ켣 
	float backingWidth, backingHeight;

private:
	CZBrush *ptrBrush;						///< �����ã���������������
	float mainScreenScale;					///< �豸��Ļ�ķֱ��ʱ�������PPI��ÿӢ�������������
	CZPreviewRender render;					///< ������
};