
///  \file CZPaintingRender.cpp
///  \brief This is the file implements the Class CZPaintingRender.
///
///		负责画图区域的绘制。
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note

#include "CZPaintingRender.h"
#include "CZGeometry.h"
#include "CZStampGenerator.h"
#include "CZLayer.h"
#include "CZBrush.h"

using namespace  std;

CZPaintingRender::CZPaintingRender(const CZSize &size)
{
	// create context

	quadVAO = 0;
	quadVBO = 0;
	
	resize(size);

	layerTextures.clear();
	layerHueChromaLumaTex.clear();

	brushTex = NULL;
	ptrClearColor = NULL;

	loadShaders();

	// configure some default GL state
	glEnable(GL_BLEND);
	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
}
CZPaintingRender::~CZPaintingRender()
{
	//setContext();
	if (quadVBO) { glDeleteBuffers(1,&quadVBO); quadVBO = 0;}
	if (quadVAO) { glDeleteVertexArrays(1,&quadVAO); quadVAO = 0;}
	if (activePaintTexture) { delete activePaintTexture; activePaintTexture = NULL;}
	if (brushTex) { delete brushTex; brushTex = NULL;}

	/// 删除着色器
	for(map<string,CZShader*>::iterator itr = shaders.begin(); itr!=shaders.end(); itr++)
		delete itr->second;
	shaders.clear();
	/// 删除图层纹理
	/*for(map<CZLayer*,CZTexture*>::iterator itr = layerTextures.begin(); itr!=layerTextures.end(); itr++)
		delete itr->second;*/
	layerTextures.clear();
	/// 删除图层色调/浓度/亮度纹理
	/*for(map<CZLayer*,CZTexture*>::iterator itr = layerHueChromaLumaTex.begin(); itr!=layerHueChromaLumaTex.end(); itr++)
		delete itr->second;*/
	layerHueChromaLumaTex.clear();
};

/// 配置绘制信息（改变内部变量）
void CZPaintingRender::configure(map<string,void*> &conf)
{
	// setContext
	map<string,void*>::iterator itr;
	
	/// 设置离线绘制缓存
	itr = conf.find("colorBuffer");
	if(itr != conf.end())
	{
		CZSize *size = (CZSize*) itr->second;
		fbo.setColorRenderBuffer(size->width,size->height);
	}

	itr = conf.find("size");
	if(itr != conf.end())
	{
		CZSize *size = (CZSize*) itr->second;
		resize(*size);
	}

	itr = conf.find("layer");
	if(itr != conf.end()) ptrRenderingLayer = (CZLayer *)itr->second;

	itr = conf.find("bgColor");
	if(itr != conf.end()) ptrClearColor = (CZColor *)itr->second;
}

/// 开始绘制
void CZPaintingRender::begin(DrawType type)
{
	CZShader *shader;
	switch(type)
	{
	case kDrawPath:
		// 开启fbo
		fbo.begin();
		if (clearBuffer) 
		{
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		// use shader program
		shader = shaders["brush"];
		shader->begin();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brushTex->id);

		glUniform1i(shader->getUniformLocation("texture"),0);
		///glUniformMatrix4fv([brushShader locationForUniform:@"modelViewProjectionMatrix"), 1, GL_FALSE, projection_);
		
		break;

		/// ===  以下是CZLayer中的绘制 ===
	case kPaintingErase:
		// use shader program
		shader = shaders["blitWithEraseMask"];
		shader->begin();

		glUniformMatrix4fv(shader->getUniformLocation("mvpMat"),1,GL_FALSE,ptrRenderingLayer->ptrProjection);
		glUniform1i(shader->getUniformLocation("texture"), 0);
		glUniform1f(shader->getUniformLocation("opacity"), ptrRenderingLayer->opacity);
		glUniform1i(shader->getUniformLocation("mask"), 1);


		// Bind the texture to be used
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrRenderingLayer)->id);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, getPaintTexture()->id);

		/// 配置绘制模式
		configureBlendMode(ptrRenderingLayer->blendMode);

		break;

	case kPaintingMask:
		// use shader program
		shader = shaders["blitWithMask"];
		shader->begin();

		glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, ptrRenderingLayer->ptrProjection);
		glUniform1i(shader->getUniformLocation("texture"), 0);
		glUniform1f(shader->getUniformLocation("opacity"), ptrRenderingLayer->opacity);
		glUniform4f(shader->getUniformLocation("color"), ptrRenderingLayer->ptrColor->red, ptrRenderingLayer->ptrColor->green, \
			ptrRenderingLayer->ptrColor->blue, ptrRenderingLayer->ptrColor->alpha);
		glUniform1i(shader->getUniformLocation("mask"), 1);
		glUniform1i(shader->getUniformLocation("lockAlpha"), ptrRenderingLayer->alphaLocked);

		// Bind the texture to be used
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrRenderingLayer)->id);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, getPaintTexture()->id);

		/// 配置绘制模式
		configureBlendMode(ptrRenderingLayer->blendMode);

		break;

	case kPaintingBlit:
		// use shader program
		shader = shaders["blit"];
		shader->begin();

		glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, ptrRenderingLayer->ptrProjection);
		glUniform1i(shader->getUniformLocation("texture"), 0);
		glUniform1f(shader->getUniformLocation("opacity"), ptrRenderingLayer->opacity);

		// Bind the texture to be used
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrRenderingLayer)->id);
		/// 配置绘制模式
		configureBlendMode(ptrRenderingLayer->blendMode);

		break;

	case kPaintingBlitColorBalance:
		// use shader program
		shader = shaders["colorBalanceBlit"];
		shader->begin();

		glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, ptrRenderingLayer->ptrProjection);
		glUniform1i(shader->getUniformLocation("texture"), 0);
		glUniform1f(shader->getUniformLocation("opacity"), ptrRenderingLayer->opacity);

		glUniform1f(shader->getUniformLocation("redShift"), ptrRenderingLayer->colorBalance->redShift);
		glUniform1f(shader->getUniformLocation("greenShift"), ptrRenderingLayer->colorBalance->greenShift);
		glUniform1f(shader->getUniformLocation("blueShift"), ptrRenderingLayer->colorBalance->blueShift);
		glUniform1i(shader->getUniformLocation("premultiply"), 1);


		// Bind the texture to be used
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrRenderingLayer)->id);
		/// 配置绘制模式
		configureBlendMode(ptrRenderingLayer->blendMode);

		break;
		
	case kPaintingBlitHueSaturation:
		// use shader program
		shader = shaders["blitFromHueChromaLuma"];
		shader->begin();

		glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, ptrRenderingLayer->ptrProjection);
		glUniform1i(shader->getUniformLocation("texture"), 0);
		glUniform1f(shader->getUniformLocation("opacity"), ptrRenderingLayer->opacity);

		//glUniform1f(shader->getUniformLocation("hueShift"), hueSaturation.hueShift);
		//glUniform1f(shader->getUniformLocation("saturationShift"), hueSaturation_.saturationShift);
		//glUniform1f(shader->getUniformLocation("brightnessShift"), hueSaturation_.brightnessShift);
		glUniform1i(shader->getUniformLocation("premultiply"), 1);


		// Bind the texture to be used
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, getLayerHCLTexture(ptrRenderingLayer)->id);
		/// 配置绘制模式
		configureBlendMode(ptrRenderingLayer->blendMode);

		break;

	case kPaintingBlitTransform:
		// use shader program
		shader = shaders["blit"];
		shader->begin();

		glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, ptrRenderingLayer->ptrProjection);
		glUniform1i(shader->getUniformLocation("texture"), 0);
		glUniform1f(shader->getUniformLocation("opacity"), ptrRenderingLayer->opacity);

		// Bind the texture to be used
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrRenderingLayer)->id);

		/// 配置绘制模式
		configureBlendMode(ptrRenderingLayer->blendMode);

		break;

	case kPaingtingImageData:
		/// 开始fbo
		fbo.begin();

		glViewport(0, 0, width, height);

		// use shader program
		shader = shaders["nonPremultipliedBlit"];
		shader->begin();

		glUniform1i(shader->getUniformLocation("texture"), (GLuint) 0);
		glUniform1f(shader->getUniformLocation("opacity"), 1.0f); // fully opaque

		glActiveTexture(GL_TEXTURE0);
		// Bind the texture to be used
		glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrRenderingLayer)->id);

		// clear the buffer to get a transparent background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// set up premultiplied normal blend
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case kPaintingImageForCurrent:
		fbo.setColorRenderBuffer(width,height);
		fbo.begin();

		// 用背景颜色清除缓存
		if (ptrClearColor) 
		{
			glClearColor(ptrClearColor->red, ptrClearColor->green, ptrClearColor->blue, ptrClearColor->alpha);
		} 
		else 
		{
			glClearColor(0,0,0,0);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		ptrClearColor = NULL;		// restore the pointer
		break;
	}
}

/// 绘制
void CZPaintingRender::draw(DrawType type, void* data /*= NULL*/, unsigned int num /*= 0*/)
{
	GLfloat proj[16], effectiveProj[16],final[16];
	CZRect *rect = (CZRect*) data;

	switch(type)
	{
	case kPaintingErase:
	case kPaintingMask:
	case kPaintingBlit:
	case kPaintingBlitColorBalance:
	case kPaintingBlitHueSaturation:
	case kPaingtingImageData:
		// setup projection matrix (orthographic)
		//mat4f_LoadOrtho(0, (GLuint) rect->size.width, 0, (GLuint) rect->size.height, -1.0f, 1.0f, proj);

		//CZAffineTransform translate = CZAffineTransform::makeFromTranslation(-rect->origin.x, -rect->origin.y);
		//mat4f_LoadCGAffineTransform(effectiveProj, translate);
		//mat4f_MultiplyMat4f(proj, effectiveProj, final);

		//glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, final);
		//glBindVertexArrayOES(self.painting.quadVAO);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		break;
	case kPaintingBlitTransform:
		//CZRect rect(0,0,width,height);

		if (ptrRenderingLayer->clipWhenTransformed) 
		{
			glEnable(GL_STENCIL_TEST);
			glClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);

			// All drawing commands fail the stencil test, and are not drawn, but increment the value in the stencil buffer.
			glStencilFunc(GL_NEVER, 0, 0);
			glStencilOp(GL_INCR, GL_INCR, GL_INCR);

			drawRect(*rect,CZAffineTransform::makeIdentity());

			// now, allow drawing, except where the stencil pattern is 0x1 and do not make any further changes to the stencil buffer
			glStencilFunc(GL_EQUAL, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}
		
		drawRect(*rect, ptrRenderingLayer->transform);

		if (ptrRenderingLayer->clipWhenTransformed) 
			glDisable(GL_STENCIL_TEST);

		CZCheckGLError();

		break;
	}
}

/// 结束绘制
void CZPaintingRender::end(DrawType type)
{
	switch(type)
	{
	case kDrawPath:
	case kPaintingImageForCurrent:
		// 关闭启fbo
		fbo.end();
		break;

		/// ===  以下是CZLayer中的绘制 ===
	case kPaintingErase:
	case kPaintingMask:
	case kPaintingBlit:
	case kPaintingBlitColorBalance:
	case kPaintingBlitHueSaturation:
		// unbind VAO
		//glBindVertexArrayOES(0);
		glBindVertexArray(0);
		break;

	case kPaingtingImageData:
		glBindVertexArray(0);
		fbo.end();
		break;

	case kPaintingBlitTransform:
		break;
	}
}

/// 生成图像数据并返回
CZImage* CZPaintingRender::imageForLastDraw()
{
#if USE_OPENGL
	CZImage *ret = new CZImage(fbo.width,fbo.height,CZImage::RGBA);
	glReadPixels(0, 0, fbo.width, fbo.height, GL_RGBA, GL_FLOAT, ret->data);
#elif USE_OPENGL_ES
	UInt8 *pixels = malloc(sizeof(UInt8) * width * 4 * height);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#endif
	return ret;
}


/// 载入着色器
void CZPaintingRender::loadShaders()
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
	/// ！暂时只载入一个shader
	vector<string> attributes;
	attributes.push_back("inPosition");
	attributes.push_back("inTexcoord");
	attributes.push_back("alpha");
	
	vector<string> uniforms;
	uniforms.push_back("mvpMat");
	uniforms.push_back("texture");

	CZShader *shader = new CZShader("brush.vert","brush.frag",attributes,uniforms);

	shaders.insert(make_pair("brush",shader));
}

/// 配置混合模式
void CZPaintingRender::configureBlendMode(BlendMode mode)
{
	switch (mode) 
	{
	case kBlendModeMultiply:
		glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case kBlendModeScreen:
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		break;
	case kBlendModeExclusion:
		glBlendFuncSeparate(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		break;
	default: // WDBlendModeNormal
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // premultiplied blend
		break;
	}
}

/// 返回quadVAO
GLuint CZPaintingRender::getQuadVAO()
{
	if(!quadVAO)
	{
		//setContext()

		const GLfloat vertices[] = 
		{
			0.0, 0.0, 0.0, 0.0,
			width, 0.0, 1.0, 0.0,
			0.0, height, 0.0, 1.0,
			width, height, 1.0, 1.0,
		};
#if USE_OPENGL_ES
		glGenVertexArraysOES(1, quadVAO);
		glBindVertexArrayOES(*quadVAO);
#endif
#if USE_OPENGL
		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);
#endif
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
		glBindVertexArray(0);
	}

	return quadVAO;
}

/// 获取绘制用纹理
CZTexture* CZPaintingRender::getPaintTexture()
{
	if(!activePaintTexture)
	{
		//setContext
		activePaintTexture = new CZTexture(width,height);
	}
	return activePaintTexture;
}

/// 获取图层纹理
CZTexture* CZPaintingRender::getLayerTexture(CZLayer* layer)
{
	pair<map<CZLayer*,CZTexture*>::iterator, bool> ret;  

//	ret = layerTextures.insert(make_pair(layer,NULL));

	if(ret.second)	
	{	
		/// 生成图层纹理
		CZTexture *tex = CZTexture::produceFromImage(layer->image);
		ret.first->second = tex;
	}

	return ret.first->second;
}
/// 清除图层纹理
void CZPaintingRender::clearLayerTexture(CZLayer* layer)
{
	//map<CZLayer*, CZTexture*>::iterator itr = layerTextures.find(layer);

	//if(itr != layerTextures.end())
	//{
	//	delete itr->second;
	//	layerTextures.erase(itr);
	//}
}

/// 获取图层的色调/浓度/亮度纹理
CZTexture* CZPaintingRender::getLayerHCLTexture(CZLayer *layer)
{
	pair<map<CZLayer*,CZTexture*>::iterator, bool> ret;  

	//ret = layerHueChromaLumaTex.insert(make_pair(layer,NULL));

	if(ret.second)	
	{	
		/// 生成图层纹理
		CZTexture *tex = CZTexture::produceFromImage(layer->image);
		ret.first->second = tex;
	}

	return ret.first->second;
}
/// 清除图层的色调/浓度/亮度纹理
void CZPaintingRender::clearLayerHCLTexture(CZLayer *layer)
{
	//map<CZLayer*, CZTexture*>::iterator itr ;//= layerHueChromaLumaTex.find(layer);

	//if(itr != layerHueChromaLumaTex.end())
	//{
	//	delete itr->second;
	//	layerHueChromaLumaTex.erase(itr);
	//}
}

/// 设置成绘制到纹理
void CZPaintingRender::setRenderToTexture()
{
	// setContext
	fbo.setTexture(activePaintTexture);
}

/// 设置是否清楚缓存
void CZPaintingRender::setClearBuffer(bool flag)
{
	clearBuffer = flag;
}

/// 更换笔刷纹理
void CZPaintingRender::changeBrushTex(CZBrush *brush)
{
	if (brushTex) { delete brushTex; brushTex = NULL;}

	CZStampGenerator *gen = brush->generator;
	brushTex = CZTexture::produceFromImage(gen->getStamp(false));		///< get the normal stamp;
}

/// 调整绘制器大小
void CZPaintingRender::resize(const CZSize &size)
{
	width = size.width;
	height = size.height;

	if(activePaintTexture) { delete activePaintTexture; activePaintTexture = NULL;}
}

/// 绘制矩形
void CZPaintingRender::drawRect(const CZRect &rect, const CZAffineTransform &transform)
{
	CZ2DPoint corners[4];
	GLuint  vbo = 0;

	corners[0] = rect.origin;
	corners[1] = CZ2DPoint(rect.getMaxX(), rect.getMinY());
	corners[2] = CZ2DPoint(rect.getMaxX(), rect.getMaxY());
	corners[3] = CZ2DPoint(rect.getMinX(), rect.getMaxY());

	for (int i = 0; i < 4; i++) 
	{
		corners[i] = transform.applyTo2DPoint(corners[i]);
	}

	const GLfloat quadVertices[] = 
	{
		corners[0].x, corners[0].y, 0.0, 0.0,
		corners[1].x, corners[1].y, 1.0, 0.0,
		corners[3].x, corners[3].y, 0.0, 1.0,
		corners[2].x, corners[2].y, 1.0, 1.0,
	};

	// create, bind, and populate VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, quadVertices, GL_STATIC_DRAW);

	// set up attrib pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)8);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDeleteBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}