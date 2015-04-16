
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

/// 绘制类
class CZPainting :public CZCoding
{
public:
	CZPainting(const CZSize &size);
	~CZPainting();
	
	/// 将图像绘制出来（没绑定FBO）
	void blit(CZMat4 &projection);
	
	/// 生成所有图层的图像（不包括当前绘制的笔画）
	CZImage *imageWithSize(CZSize &size, CZColor *backgroundColor = NULL);

	/// 生成当前状态的图像
	CZImage *imageForCurrentState(CZColor *backgroundColor);
	
	/// 绘制一条轨迹（绘制到纹理）
	CZRect paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer = false);
	
	/// 设置范围（让render的范围与其保持一致）
	void setDimensions(const CZSize &size);
	
	/// 设置当前激活图层
	///
	///		\param idx - 当前需要激活的图层序号
	///		\ret	   - 原来激活的图层序号
	int setActiveLayer(int idx);
	
	/// 通过UID获取图层
	/// 
	///		\note 不存在该UID的图层则返回NULL
	CZLayer *layerWithUID(unsigned int uid_);
	
	/// 删除图层
	/// 
	///		\param - 需要删除的图层
	///		\ret   - 原图层所在的序号
	///		\note 当layer被锁住的时候不能被删除 
	int removeLayer(CZLayer *layer);
	
	/// 插入图层
	void insertLayer(int idx, CZLayer *layer);
	
	/// 添加图层
	/// 
	///		\param layer - 添加的图层
	///		\ret		 - 在所有图层中的序号,失败返回-1
	int addLayer(CZLayer *layer);
	
	/// 向下合并当前图层
	/// 
	///		\ret - 是否合并成功
	bool mergeActiveLayerDown();

	/// 移动图层
	/// 
	///		\param layer - 需要移动的图层
	///		\param idx	 - 移动到的位置
	bool moveLayer(CZLayer* layer, int idx);

	/// 获得图层在所有图层中的标号，不存在返回负值
	int indexOfLayers(CZLayer *layer);

	/// 设置激活轨迹
	void setActivePath(CZPath *path);

	/// 获取激活轨迹
	CZPath* getActivePath();

	/// 设置激活图层
	void setActiveLayer(CZLayer *layer);

	/// 获取激活图层
	CZLayer* getActiveLayer();

	/// 获取着色器
	CZShader* getShader(std::string name);

	///
	CZTexture* generateTexture(CZImage* img = NULL);

	/// 返回quadVAO
	GLuint getQuadVAO();
	
	/// 获取范围
	CZSize& getDimensions();
	/// 获取绘制矩形
	CZRect& getBounds();

	/// 获取gl上下文
	CZGLContext *getGLContext();

	/// 实现coding的接口
	void update(CZDecoder *decoder_, bool deep = false);
	void encode(CZCoder *coder_, bool deep = false);

private:
	/// 载入着色器
	void loadShaders();
	/// 配置画刷
	void configureBrush(CZBrush*);

public:
	CZTexture *activePaintTexture;			///< 绘制用的纹理

private:
	CZSize dimensions;				///< 绘制范围
    CZRect bounds;
	CZPath *ptrActivePath;			///< 激活的路径，此处仅为引用
	CZLayer *ptrActiveLayer;		///< 当前绘制层

	bool flattenMode;
	std::vector<CZLayer*>	layers;	///< 绘制的画层
	std::vector<CZColor*>	colors;	
	std::vector<CZBrush*>	brushPtrs;
	std::vector<CZBrush*>	undoBrushPtrs;
	int						strokeCount;	///< 笔画数目
	char*					uuid;

	CZMat4					projMat;		///< 投影矩阵
	CZBrush					*ptrLastBrush;	///< 上一把画刷
	CZGLContext				*glContext;		///< gl上下文
	std::map<std::string,CZShader*>	shaders;///< 着色器
	GLuint quadVAO,quadVBO;					///< 绘制矩形的VAO、VBO
	CZFbo					*fbo;			
	CZTexture				*brushStampTex;	///< 画刷笔触纹理
};
#endif