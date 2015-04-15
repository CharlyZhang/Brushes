
///  \file CZPainting.cpp
///  \brief This is the file implements the class CZPainting.
///
///		
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-26
///  \note

#include "CZPainting.h"
#include "../CZUtil.h"

using namespace  std;

const int iMaxLayerNumber = 10;		///< 支持的最大图层数目


CZPainting::CZPainting(const CZSize &size)
{
	flattenMode = false;
	
	colors.clear();
	brushPtrs.clear();
	layers.clear();
	undoBrushPtrs.clear();
	strokeCount = 0;


	ptrActivePath = NULL;
	ptrLastBrush = NULL;
	uuid = CZUtil::generateUUID();

	/// set up gl context
	glContext = new CZGLContext;
	glContext->setAsCurrent();
	loadShaders();
	fbo = new CZFbo;
	quadVAO = quadVBO = 0;
	activePaintTexture = NULL;
	brushStampTex = NULL;

	/// set the dimesion
	setDimensions(size);

	/// add the default blank layer
	ptrActiveLayer = new CZLayer(this);
	addLayer(ptrActiveLayer);
}
CZPainting::~CZPainting()
{
	colors.clear();
	brushPtrs.clear();
	undoBrushPtrs.clear();

	glContext->setAsCurrent();
	/// 删除着色器
	for(map<string,CZShader*>::iterator itr = shaders.begin(); itr!=shaders.end(); itr++)
		if(itr->second)
		{
			delete itr->second;
			itr->second = NULL;
		}

	shaders.clear();
	/// 删除图层
	for(vector<CZLayer*>::iterator itr = layers.begin(); itr != layers.end(); itr++)
		delete *itr;
	layers.clear();

	if (quadVBO) { glDeleteBuffers(1,&quadVBO); quadVBO = 0;}
	if (quadVAO) { GL_DEL_VERTEXARRAY(1, &quadVAO);	quadVAO = 0;}
	if (activePaintTexture) { delete activePaintTexture; activePaintTexture = NULL;}
	if (brushStampTex) { delete brushStampTex; brushStampTex = NULL;}
	if (fbo) { delete fbo; fbo = NULL;}
	
	CZCheckGLError();

	delete glContext;
}

/// 将图像绘制出来（没绑定FBO）
void CZPainting::blit(CZMat4 &projection)
{
	if (flattenMode) 
	{
		//[self blitFlattenedTexture:projection];
		return;
	}

	for (vector<CZLayer*>::iterator itr = layers.begin(); itr != layers.end(); itr++) 
	{
		CZLayer *layer = *itr;
		if (!layer->isVisible()) continue;

		if (ptrActiveLayer == layer && ptrActivePath) 
		{
			if (ptrActivePath->action == CZPathActionErase)
				layer->blit(projection,activePaintTexture);
			else 
				layer->blit(projection,activePaintTexture,ptrActivePath->color);
		} else 
			layer->blit(projection);
	}
}

/// 生成所有图层的图像（不包括当前绘制的笔画）
CZImage *CZPainting::imageWithSize(CZSize &size, CZColor *backgroundColor /*= NULL*/)
{
	/// 获得运行所需要的数据
	int w = size.width;
	int h = size.height;
	CZMat4 projection;
	projection.SetOrtho(0,w,0,h,-1.0f,1.0f);

	/// 开始绘制
	glContext->setAsCurrent();

	fbo->setColorRenderBuffer(w,h);

	fbo->begin();

	// 用背景颜色清除缓存
	if (backgroundColor) 
		glClearColor(backgroundColor->red, backgroundColor->green, backgroundColor->blue, backgroundColor->alpha);
	else 
		glClearColor(0,0,0,0);

	glClear(GL_COLOR_BUFFER_BIT);

	for (vector<CZLayer*>::iterator itr = layers.begin(); itr != layers.end(); itr++) 
	{
		CZLayer *layer = *itr;
		if (!layer->isVisible()) continue;

		layer->blit(projection);
	}

	CZImage *ret = new CZImage(w,h,CZImage::RGBA);
	glReadPixels(0, 0, w, h, GL_RGBA, GL_PIXEL_TYPE, ret->data);

	fbo->end();

	return ret;
}

/// 生成当前状态的图像
CZImage *CZPainting::imageForCurrentState(CZColor *backgroundColor)
{
	glContext->setAsCurrent();

	fbo->setColorRenderBuffer(dimensions.width, dimensions.height);

	fbo->begin();

	// 用背景颜色清除缓存
	if (backgroundColor) 
		glClearColor(backgroundColor->red, backgroundColor->green, backgroundColor->blue, backgroundColor->alpha);
	else 
		glClearColor(0,0,0,0);

	glClear(GL_COLOR_BUFFER_BIT);

	for (vector<CZLayer*>::iterator itr = layers.begin(); itr != layers.end(); itr++) 
	{
		CZLayer *layer = *itr;
		if (!layer->isVisible()) continue;

		if (ptrActiveLayer == layer && ptrActivePath) 
		{
			if (ptrActivePath->action == CZPathActionErase) 
				layer->blit(projMat,activePaintTexture);
			else 
				layer->blit(projMat,activePaintTexture,*backgroundColor);
		} else 
			layer->blit(projMat);
	}

	CZImage *ret = new CZImage(dimensions.width,dimensions.height,CZImage::RGBA);
	glReadPixels(0, 0, dimensions.width, dimensions.height, GL_RGBA, GL_PIXEL_TYPE, ret->data);

	fbo->end();

	return ret;
}

/// 绘制一条轨迹（绘制到纹理）
CZRect CZPainting::paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer /* = false */)
{
	if (path_ == NULL || randomizer == NULL)
	{
		LOG_ERROR("either path or randomizer is null!\n");
		return CZRect();
	}

	ptrActivePath = path_;

	glContext->setAsCurrent();
	
	fbo->setTexture(activePaintTexture);

	// 开启fbo
	fbo->begin();

	if (clearBuffer) 
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	configureBrush(path_->ptrBrush);
	
	// use shader program
	CZShader *shader = shaders["brush"];
	shader->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brushStampTex->texId);

	glUniform1i(shader->getUniformLocation("texture"),0);
	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"), 1, GL_FALSE, projMat);
	CZCheckGLError();

	/// 绘制轨迹
	CZRect pathBounds = path_->paint(randomizer);

	shader->end();

	// 关闭启fbo
	fbo->end();

	return pathBounds;
}

/// 设置范围
void CZPainting::setDimensions(const CZSize &size)
{
	if(dimensions == size) return;

	dimensions = size;
	projMat.SetOrtho(0,size.width,0,size.height,-1.0f,1.0f);
	
	glContext->setAsCurrent();
	if(activePaintTexture) delete activePaintTexture; 
	activePaintTexture = new CZTexture(size.width,size.height);
}

/// 设置当前激活图层
///
///		\param idx - 当前需要激活的图层序号
///		\ret	   - 原来激活的图层序号
int CZPainting::setActiveLayer(int idx)
{
	int oldIndex = indexOfLayers(ptrActiveLayer);

	if (idx < 0 || idx >= layers.size()) 
	{
		LOG_ERROR("idx is out of range\n");
		return oldIndex;
	}

	ptrActiveLayer = layers[idx];

	return oldIndex;

}

/// 通过UID获取图层
CZLayer *CZPainting::layerWithUID(unsigned int uid_)
{
// 	int num = layers.size();
// 	for(int i = 0; i < num; i ++)
// 		if(uid_ == layers[i]->getUID()) return layers[i];

	return NULL;
}

/// 删除图层
/// 
///		\param - 需要删除的图层
///		\ret   - 原图层所在的序号
int CZPainting::removeLayer(CZLayer *layer)
{
	if(layer == NULL)
	{
		LOG_ERROR("layer is NULL\n");
		return -1;
	}

	int oldIdx = indexOfLayers(layer);

	if(layer->isLocked())
	{
		LOG_ERROR("layer is locked\n");
		return oldIdx;
	}

	if (layer == ptrActiveLayer && layers.size()> 1)
	{
		// choose another layer to be active before we remove it
		int index;
		if (oldIdx >= 1)
		{
			index = oldIdx-1;
		}
		else
		{
			index = 1;
		}
		ptrActiveLayer = layers[index];
	}

	// do this before decrementing index
	for(vector<CZLayer*>::iterator itr=layers.begin(); itr!=layers.end(); itr++)
		if(*itr == layer)
		{
			layers.erase(itr);
			break;
		}

	return oldIdx;
}

/// 插入图层
void CZPainting::insertLayer(int idx, CZLayer *layer)
{
	if(idx < 0 || idx > layers.size())
	{
		LOG_ERROR("idx is out of range\n");
		return;
	}

	layers.insert(layers.begin()+idx,layer);
}

/// 添加图层
/// 
///		\param layer - 添加的图层
///		\ret		 - 在所有图层中的序号
int CZPainting::addLayer(CZLayer *layer)
{
	if(layer == NULL)
	{
		LOG_ERROR("layer is NULL\n");
		return -1;
	}

	if(layers.size() > iMaxLayerNumber)
	{
		LOG_ERROR("painting has reached the max number of Layers\n");
		return -1;
	}

	int newIdx = indexOfLayers(ptrActiveLayer)+1;
	
	insertLayer(newIdx, layer);
	ptrActiveLayer = layer;

	return newIdx;
}

/// 向下合并当前图层
/// 
///		\ret - 是否合并成功
bool CZPainting::mergeActiveLayerDown()
{
	int activeIdx = indexOfLayers(ptrActiveLayer);
	
	/// in case the active layer is at bottom
	if(activeIdx <= 0)
	{
		LOG_ERROR("active layer is NULL or at bottom\n");
		return false;
	}

	/// in case there's only one layer
	if(layers.size() <= 1)
	{
		LOG_ERROR("layers number is no more than 1\n");
		return false;
	}

	CZLayer *bottomLayer = layers[activeIdx - 1];

	/// in case the layer to merged with is not editable
	if(!bottomLayer->isEditable())
	{
		LOG_ERROR("layer to be merged with is not editable\n");
		return false;
	}

	bool ret = bottomLayer->merge(ptrActiveLayer);
	removeLayer(ptrActiveLayer);
	ptrActiveLayer = bottomLayer;

	return ret;
}

/// 移动图层
/// 
///		\param layer - 需要移动的图层
///		\param idx	 - 移动到的位置
bool CZPainting::moveLayer(CZLayer* layer, int idx)
{ 
	return false;
}

/// 获得图层在所有图层中的标号，不存在返回负值
int CZPainting::indexOfLayers(CZLayer *layer)
{
	int ret;
	int num = layers.size();
	for(ret = 0; ret < num; ret ++)
		if(layer == layers[ret]) return ret;

	return -1;
}


/// 设置激活轨迹
void CZPainting::setActivePath(CZPath *path)
{
	ptrActivePath = path;
}

/// 获取激活轨迹
CZPath* CZPainting::getActivePath()
{
	return ptrActivePath;
}

/// 设置激活图层
void CZPainting::setActiveLayer(CZLayer *layer)
{
	ptrActiveLayer = layer;
}

/// 获取激活图层
CZLayer* CZPainting::getActiveLayer()
{
	return ptrActiveLayer;
}

/// 获取着色器
CZShader* CZPainting::getShader(string name)
{
	glContext->setAsCurrent();
	map<string,CZShader*>::iterator itr = shaders.find(name);
	if (itr == shaders.end())
	{
		return NULL;
	}

	return itr->second;
}

///
CZTexture* CZPainting::generateTexture(CZImage* img /* = NULL */)
{
	if (img)	return CZTexture::produceFromImage(img);
	else		return new CZTexture(dimensions.width,dimensions.height);
}

/// 返回quadVAO
GLuint CZPainting::getQuadVAO()
{
	if(!quadVAO)
	{
		glContext->setAsCurrent();

		const GLfloat vertices[] = 
		{
			0.0, 0.0, 0.0, 0.0,
			(GLfloat)dimensions.width, 0.0, 1.0, 0.0,
			0.0, (GLfloat)dimensions.height, 0.0, 1.0,
			(GLfloat)dimensions.width, (GLfloat)dimensions.height, 1.0, 1.0,
		};

		GL_GEN_VERTEXARRAY(1,&quadVAO);
		GL_BIND_VERTEXARRAY(quadVAO);
		// create, bind, and populate VBO
		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, vertices, GL_STATIC_DRAW);

		// set up attrib pointers
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)8);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER,0);

		GL_BIND_VERTEXARRAY(0);
	}

	return quadVAO;
}

/// 获取范围
CZSize& CZPainting::getDimensions()
{
	return dimensions;
}

/// 获取绘制矩形
CZRect& CZPainting::getBounds()
{
    bounds = CZRect(0,0,dimensions.width,dimensions.height);
	return bounds;
}

/// 获取gl上下文
CZGLContext *CZPainting::getGLContext()
{
	return glContext;
};

/// 实现CZCoding接口
void CZPainting::update(CZDecoder *decoder_, bool deep /*= false*/){};
void CZPainting::encode(CZCoder *coder_, bool deep /*= false*/){};

void CZPainting::loadShaders()
{
#if 0
	NSString        *shadersJSONPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"Shaders.json"];
	NSData          *JSONData = [NSData dataWithContentsOfFile:shadersJSONPath];
	NSError         *error = nil;
	NSDictionary    *shaderDict = [NSJSONSerialization JSONObjectWithData:JSONData options:0 error:&error];

	if (!shaderDict) {
		WDLog(@"Error loading 'Shaders.json': %@", error);
		return;
	}

	NSMutableDictionary *tempShaders = [NSMutableDictionary dictionary];

	for (NSString *key in shaderDict.keyEnumerator) {
		NSDictionary *description = shaderDict[key];
		NSString *vertex = description[@"vertex"];
		NSString *fragment = description[@"fragment"];
		NSArray *attributes = description[@"attributes"];
		NSArray *uniforms = description[@"uniforms"];

		WDShader *shader = [[WDShader alloc] initWithVertexShader:vertex
fragmentShader:fragment
attributesNames:attributes
uniformNames:uniforms];
		tempShaders[key] = shader;
	}
	WDCheckGLError();

	shaders = tempShaders;
#endif
	/// 笔刷shader
	vector<string> attributes;
	attributes.push_back("inPosition");
	attributes.push_back("inTexcoord");
	attributes.push_back("alpha");
	vector<string> uniforms;
	uniforms.push_back("mvpMat");
	uniforms.push_back("texture");

	CZShader *shader = new CZShader("brush.vert","brush.frag",attributes,uniforms);
	shaders.insert(make_pair("brush",shader));

	/// 将图层和绘制笔画输出到屏幕
	attributes.clear();
	attributes.push_back("inPosition");
	attributes.push_back("inTexcoord");
	uniforms.clear();
	uniforms.push_back("mvpMat");
	uniforms.push_back("texture");
	uniforms.push_back("opacity");
	uniforms.push_back("mask");
	uniforms.push_back("color");
	uniforms.push_back("lockAlpha");

	shader = new CZShader("blit.vert","blitWithMask.frag",attributes,uniforms);
	shaders.insert(make_pair("blitWithMask",shader));

	/// 将图层纹理绘制出来
	uniforms.clear();
	uniforms.push_back("mvpMat");
	uniforms.push_back("texture");
	uniforms.push_back("opacity");

	shader = new CZShader("blit.vert","blit.frag",attributes,uniforms);
	shaders.insert(make_pair("blit",shader));

	/// 合并绘制笔画到图层
	uniforms.clear();
	uniforms.push_back("mvpMat");
	uniforms.push_back("texture");
	uniforms.push_back("mask");
	uniforms.push_back("color");
	uniforms.push_back("lockAlpha");

	shader = new CZShader("blit.vert","compositeWithMask.frag",attributes,uniforms);
	shaders.insert(make_pair("compositeWithMask",shader));

	/// 合并擦除笔画到图层
	uniforms.clear();
	uniforms.push_back("mvpMat");
	uniforms.push_back("texture");
	uniforms.push_back("mask");
	uniforms.push_back("lockAlpha");

	shader = new CZShader("blit.vert","compositeWithEraseMask.frag",attributes,uniforms);
	shaders.insert(make_pair("compositeWithEraseMask",shader));

	CZCheckGLError();
}

/// 配置画刷
void CZPainting::configureBrush(CZBrush* brush_)
{
	if(brush_ != ptrLastBrush)
	{
		ptrLastBrush = brush_;

		if (brushStampTex) { delete brushStampTex; brushStampTex = NULL;}

		CZStampGenerator *gen = brush_->getGenerator();
		CZImage *img = gen->getStamp(false);
		brushStampTex = CZTexture::produceFromImage(img);		///< get the normal stamp;
	}
}
