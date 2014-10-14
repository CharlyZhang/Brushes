
///  \file CZPainting.h
///  \brief This is the file declare the class CZPainting.
///
///		
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-26
///  \note

#ifndef _CZPAINTING_H_
#define _CZPAINTING_H_

#include "CZGeometry.h"
#include "CZPath.h"
#include "CZRandom.h"
#include "CZFbo.h"
#include "CZCanvasRender.h"
#include "CZTexture.h"
#include <map>
#include <string>

class CZPainting
{
public:
	CZPainting(const CZSize &size);
	~CZPainting();
	/// ����һ���켣
	CZRect paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer = false);
private:
	/// ���ñ�ˢ
	///	
	///		���úû����ñ�ˢ�������Լ�Shader������CZPainting���漰�����ֱ�ˢ������������ǰ��Ҫ����ǰ�õı�ˢ���úá�
	/// 
	///		/param brush_	- ���õĻ�ˢ
	///		/return			- �û�ˢ��Ӧ��Shader
	CZShader* configureBrush(CZBrush *brush_);
	/// ��ȡ��Ӧ��Shader��ͬʱ�趨��ǰGL�����ģ�
	CZShader* getShader(std::string shaderName);
	/// ��ȡ����������
	CZTexture* getPaintTexture();
	/// ����Shader
	void loadShaders();

private:
	CZFbo *fbo;						///< �����õ�FBO
	CZFbo reusableFBo;				///< �ظ�ʹ�õ�FBO�����Ƶ�����
	CZPath *ptrActivePath;			///< �����·�����˴���Ϊ����
	CZTexture *activePaintTexture;	///< �����õ�����
	CZSize dimensions;				///< ���Ʒ�Χ

	std::vector<CZColor*>	colors;	
	std::vector<CZBrush*>	brushes;
	std::vector<CZBrush*>	undoBrushes;
	int						strokeCount;	///< �ʻ���Ŀ
	unsigned int			uid;
	

	std::map<std::string,CZShader *> shaders;	///< ���ƻ��õ��ĸ���shader
	CZCanvasRender			render;

};
#endif