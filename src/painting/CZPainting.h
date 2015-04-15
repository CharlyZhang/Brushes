
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

#include "../CZDefine.h"
#include "../basic/CZSize.h"
#include "../basic/CZRect.h"
#include "../basic/CZColor.h"
#include "../basic/CZImage.h"
#include "../basic/CZRandom.h"
#include "../basic/CZMat4.h"
#include "../path/CZPath.h"
#include "../brush/CZBrush.h"
#include "../serialization/CZCoding.h"
#include "../graphic/CZGLContext.h"
#include "../graphic/CZShader.h"
#include "../graphic/CZTexture.h"
#include "../graphic/CZFbo.h"
#include "../graphic/glDef.h"
#include "CZLayer.h"

#include <string>
#include <vector>
#include <map>

/// ������
class CZPainting :public CZCoding
{
public:
	CZPainting(const CZSize &size);
	~CZPainting();
	
	/// ��ͼ����Ƴ�����û��FBO��
	void blit(CZMat4 &projection);
	
	/// ��������ͼ���ͼ�񣨲�������ǰ���Ƶıʻ���
	CZImage *imageWithSize(CZSize &size, CZColor *backgroundColor = NULL);

	/// ���ɵ�ǰ״̬��ͼ��
	CZImage *imageForCurrentState(CZColor *backgroundColor);
	
	/// ����һ���켣�����Ƶ�����
	CZRect paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer = false);
	
	/// ���÷�Χ����render�ķ�Χ���䱣��һ�£�
	void setDimensions(const CZSize &size);
	
	/// ���õ�ǰ����ͼ��
	///
	///		\param idx - ��ǰ��Ҫ�����ͼ�����
	///		\ret	   - ԭ�������ͼ�����
	int setActiveLayer(int idx);
	
	/// ͨ��UID��ȡͼ��
	/// 
	///		\note �����ڸ�UID��ͼ���򷵻�NULL
	CZLayer *layerWithUID(unsigned int uid_);
	
	/// ɾ��ͼ��
	/// 
	///		\param - ��Ҫɾ����ͼ��
	///		\ret   - ԭͼ�����ڵ����
	///		\note ��layer����ס��ʱ���ܱ�ɾ�� 
	int removeLayer(CZLayer *layer);
	
	/// ����ͼ��
	void insertLayer(int idx, CZLayer *layer);
	
	/// ���ͼ��
	/// 
	///		\param layer - ��ӵ�ͼ��
	///		\ret		 - ������ͼ���е����,ʧ�ܷ���-1
	int addLayer(CZLayer *layer);
	
	/// ���ºϲ���ǰͼ��
	/// 
	///		\ret - �Ƿ�ϲ��ɹ�
	bool mergeActiveLayerDown();

	/// �ƶ�ͼ��
	/// 
	///		\param layer - ��Ҫ�ƶ���ͼ��
	///		\param idx	 - �ƶ�����λ��
	bool moveLayer(CZLayer* layer, int idx);

	/// ���ͼ��������ͼ���еı�ţ������ڷ��ظ�ֵ
	int indexOfLayers(CZLayer *layer);

	/// ���ü���켣
	void setActivePath(CZPath *path);

	/// ��ȡ����켣
	CZPath* getActivePath();

	/// ���ü���ͼ��
	void setActiveLayer(CZLayer *layer);

	/// ��ȡ����ͼ��
	CZLayer* getActiveLayer();

	/// ��ȡ��ɫ��
	CZShader* getShader(std::string name);

	///
	CZTexture* generateTexture(CZImage* img = NULL);

	/// ����quadVAO
	GLuint getQuadVAO();
	
	/// ��ȡ��Χ
	CZSize& getDimensions();
	/// ��ȡ���ƾ���
	CZRect& getBounds();

	/// ��ȡgl������
	CZGLContext *getGLContext();

	/// ʵ��coding�Ľӿ�
	void update(CZDecoder *decoder_, bool deep = false);
	void encode(CZCoder *coder_, bool deep = false);

private:
	/// ������ɫ��
	void loadShaders();
	/// ���û�ˢ
	void configureBrush(CZBrush*);

public:
	CZTexture *activePaintTexture;			///< �����õ�����

private:
	CZSize dimensions;				///< ���Ʒ�Χ
    CZRect bounds;
	CZPath *ptrActivePath;			///< �����·�����˴���Ϊ����
	CZLayer *ptrActiveLayer;		///< ��ǰ���Ʋ�

	bool flattenMode;
	std::vector<CZLayer*>	layers;	///< ���ƵĻ���
	std::vector<CZColor*>	colors;	
	std::vector<CZBrush*>	brushPtrs;
	std::vector<CZBrush*>	undoBrushPtrs;
	int						strokeCount;	///< �ʻ���Ŀ
	char*					uuid;

	CZMat4					projMat;		///< ͶӰ����
	CZBrush					*ptrLastBrush;	///< ��һ�ѻ�ˢ
	CZGLContext				*glContext;		///< gl������
	std::map<std::string,CZShader*>	shaders;///< ��ɫ��
	GLuint quadVAO,quadVBO;					///< ���ƾ��ε�VAO��VBO
	CZFbo					*fbo;			
	CZTexture				*brushStampTex;	///< ��ˢ�ʴ�����
};
#endif