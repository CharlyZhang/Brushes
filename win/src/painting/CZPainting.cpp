
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

const int iMaxLayerNumber = 10;		///< ֧�ֵ����ͼ����Ŀ


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
	/// ɾ����ɫ��
	for(map<string,CZShader*>::iterator itr = shaders.begin(); itr!=shaders.end(); itr++)
		if(itr->second)
		{
			delete itr->second;
			itr->second = NULL;
		}

	shaders.clear();
	/// ɾ��ͼ��
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

/// ��ͼ����Ƴ�����û��FBO��
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

/// ��������ͼ���ͼ�񣨲�������ǰ���Ƶıʻ���
CZImage *CZPainting::imageWithSize(CZSize &size, CZColor *backgroundColor /*= NULL*/)
{
	/// �����������Ҫ������
	int w = size.width;
	int h = size.height;
	CZMat4 projection;
	projection.SetOrtho(0,w,0,h,-1.0f,1.0f);

	/// ��ʼ����
	glContext->setAsCurrent();

	fbo->setColorRenderBuffer(w,h);

	fbo->begin();

	// �ñ�����ɫ�������
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

/// ���ɵ�ǰ״̬��ͼ��
CZImage *CZPainting::imageForCurrentState(CZColor *backgroundColor)
{
	glContext->setAsCurrent();

	fbo->setColorRenderBuffer(dimensions.width, dimensions.height);

	fbo->begin();

	// �ñ�����ɫ�������
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

/// ����һ���켣�����Ƶ�����
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

	// ����fbo
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

	/// ���ƹ켣
	CZRect pathBounds = path_->paint(randomizer);

	shader->end();

	// �ر���fbo
	fbo->end();

	return pathBounds;
}

/// ���÷�Χ
void CZPainting::setDimensions(const CZSize &size)
{
	if(dimensions == size) return;

	dimensions = size;
	projMat.SetOrtho(0,size.width,0,size.height,-1.0f,1.0f);
	
	glContext->setAsCurrent();
	if(activePaintTexture) delete activePaintTexture; 
	activePaintTexture = new CZTexture(size.width,size.height);
}

/// ���õ�ǰ����ͼ��
///
///		\param idx - ��ǰ��Ҫ�����ͼ�����
///		\ret	   - ԭ�������ͼ�����
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

/// ͨ��UID��ȡͼ��
CZLayer *CZPainting::layerWithUID(unsigned int uid_)
{
// 	int num = layers.size();
// 	for(int i = 0; i < num; i ++)
// 		if(uid_ == layers[i]->getUID()) return layers[i];

	return NULL;
}

/// ɾ��ͼ��
/// 
///		\param - ��Ҫɾ����ͼ��
///		\ret   - ԭͼ�����ڵ����
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

/// ����ͼ��
void CZPainting::insertLayer(int idx, CZLayer *layer)
{
	if(idx < 0 || idx > layers.size())
	{
		LOG_ERROR("idx is out of range\n");
		return;
	}

	layers.insert(layers.begin()+idx,layer);
}

/// ���ͼ��
/// 
///		\param layer - ��ӵ�ͼ��
///		\ret		 - ������ͼ���е����
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

/// ���ºϲ���ǰͼ��
/// 
///		\ret - �Ƿ�ϲ��ɹ�
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

/// �ƶ�ͼ��
/// 
///		\param layer - ��Ҫ�ƶ���ͼ��
///		\param idx	 - �ƶ�����λ��
bool CZPainting::moveLayer(CZLayer* layer, int idx)
{ 
	return false;
}

/// ���ͼ��������ͼ���еı�ţ������ڷ��ظ�ֵ
int CZPainting::indexOfLayers(CZLayer *layer)
{
	int ret;
	int num = layers.size();
	for(ret = 0; ret < num; ret ++)
		if(layer == layers[ret]) return ret;

	return -1;
}


/// ���ü���켣
void CZPainting::setActivePath(CZPath *path)
{
	ptrActivePath = path;
}

/// ��ȡ����켣
CZPath* CZPainting::getActivePath()
{
	return ptrActivePath;
}

/// ���ü���ͼ��
void CZPainting::setActiveLayer(CZLayer *layer)
{
	ptrActiveLayer = layer;
}

/// ��ȡ����ͼ��
CZLayer* CZPainting::getActiveLayer()
{
	return ptrActiveLayer;
}

/// ��ȡ��ɫ��
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

/// ����quadVAO
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

/// ��ȡ��Χ
CZSize& CZPainting::getDimensions()
{
	return dimensions;
}

/// ��ȡ���ƾ���
CZRect& CZPainting::getBounds()
{
    bounds = CZRect(0,0,dimensions.width,dimensions.height);
	return bounds;
}

/// ��ȡgl������
CZGLContext *CZPainting::getGLContext()
{
	return glContext;
};

/// ʵ��CZCoding�ӿ�
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
	/// ��ˢshader
	vector<string> attributes;
	attributes.push_back("inPosition");
	attributes.push_back("inTexcoord");
	attributes.push_back("alpha");
	vector<string> uniforms;
	uniforms.push_back("mvpMat");
	uniforms.push_back("texture");

	CZShader *shader = new CZShader("brush.vert","brush.frag",attributes,uniforms);
	shaders.insert(make_pair("brush",shader));

	/// ��ͼ��ͻ��Ʊʻ��������Ļ
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

	/// ��ͼ��������Ƴ���
	uniforms.clear();
	uniforms.push_back("mvpMat");
	uniforms.push_back("texture");
	uniforms.push_back("opacity");

	shader = new CZShader("blit.vert","blit.frag",attributes,uniforms);
	shaders.insert(make_pair("blit",shader));

	/// �ϲ����Ʊʻ���ͼ��
	uniforms.clear();
	uniforms.push_back("mvpMat");
	uniforms.push_back("texture");
	uniforms.push_back("mask");
	uniforms.push_back("color");
	uniforms.push_back("lockAlpha");

	shader = new CZShader("blit.vert","compositeWithMask.frag",attributes,uniforms);
	shaders.insert(make_pair("compositeWithMask",shader));

	/// �ϲ������ʻ���ͼ��
	uniforms.clear();
	uniforms.push_back("mvpMat");
	uniforms.push_back("texture");
	uniforms.push_back("mask");
	uniforms.push_back("lockAlpha");

	shader = new CZShader("blit.vert","compositeWithEraseMask.frag",attributes,uniforms);
	shaders.insert(make_pair("compositeWithEraseMask",shader));

	CZCheckGLError();
}

/// ���û�ˢ
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
