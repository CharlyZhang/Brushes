
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
#include "CZPainting.h"
#include "CZBrush.h"
#include "CZPath.h"
#include "CZRandom.h"
#include "CZColorBalance.h"
#include "CZHueSaturation.h"

using namespace  std;

CZPaintingRender::CZPaintingRender(const CZSize &size)
{
	// create context

	quadVAO = 0;
	quadVBO = 0;
	
	layerTextures.clear();
	layerHueChromaLumaTex.clear();

	brushTex = NULL;
	activePaintTexture = NULL;

	resize(size);

	loadShaders();

	// configure some default GL state
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
}
CZPaintingRender::~CZPaintingRender()
{
	setContext();
	
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

/// 生成Painting当前状态的图像
CZImage *CZPaintingRender::drawPaintingCurrentState(CZColor *bgColor, std::vector<CZLayer*> &layers)
{
	setContext();

	fbo.setColorRenderBuffer(width,height);
	
	fbo.begin();

	// 用背景颜色清除缓存
	if (bgColor) 
		glClearColor(bgColor->red, bgColor->green, bgColor->blue, bgColor->alpha);
	else 
		glClearColor(0,0,0,0);

	glClear(GL_COLOR_BUFFER_BIT);

	for (vector<CZLayer*>::iterator itr = layers.begin(); itr != layers.end(); itr++) 
	{
		CZLayer *layer = *itr;
		if (!layer->visible) continue;

		if (ptrPainting->ptrActiveLayer == layer && ptrPainting->ptrActivePath) 
		{
			if (ptrPainting->ptrActivePath->action == CZPathActionErase) 
				layer->blitWithEraseMask(this,projectionMat);
			else 
				layer->blitWithMask(this,projectionMat,bgColor);
		} else 
				layer->blit(this,projectionMat);
	}

	CZImage *ret;
#if USE_OPENGL
	ret = new CZImage(width,height,CZImage::RGBA);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, ret->data);
#elif USE_OPENGL_ES
	UInt8 *pixels = malloc(sizeof(UInt8) * width * 4 * height);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#endif

	fbo.end();

	return ret;
}
CZRect CZPaintingRender::drawPaintingStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer)
{
	setContext();
	
	fbo.setTexture(getPaintTexture());

	// 开启fbo
	fbo.begin();
	
	if (clearBuffer) 
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// use shader program
	CZShader *shader = shaders["brush"];
	shader->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brushTex->id);

	glUniform1i(shader->getUniformLocation("texture"),0);
	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"), 1, GL_FALSE, projectionMat);

	CZRect pathBounds;// = path_->paint(render,randomizer);

	/// 绘制轨迹
	pathBounds = path_->paintPath(this,randomizer);

	shader->end();
	// 关闭启fbo
	fbo.end();

	//CZNotificationCenter::getInstance()->notify(CZStrokeAddedNotification,this,(void*)&pathBounds);

	return pathBounds;
}

/// 生成当前图层图像		- for CZLayer
CZImage *CZPaintingRender::drawLayerInRect(const CZRect &rect)
{
	setContext();

	fbo.setColorRenderBuffer(rect.size.width, rect.size.height);

	/// 开始fbo
	fbo.begin();

	glViewport(0, 0, width, height);

	CZMat4 proj;
	proj.SetTranslation(-rect.origin.x, -rect.origin.y, 0);
	proj = projectionMat * proj;

	// use shader program
	CZShader *shader = shaders["nonPremultipliedBlit"];
	shader->begin();

	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"),1,GL_FALSE,proj);
	glUniform1i(shader->getUniformLocation("texture"), (GLuint) 0);
	glUniform1f(shader->getUniformLocation("opacity"), 1.0f); // fully opaque

	glActiveTexture(GL_TEXTURE0);
	// Bind the texture to be used
	glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrLayer)->id);

	// clear the buffer to get a transparent background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// set up premultiplied normal blend
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(getQuadVAO());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	
	shader->end();

	CZImage *ret;
#if USE_OPENGL
	ret = new CZImage(width,height,CZImage::RGBA);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, ret->data);
#elif USE_OPENGL_ES
	UInt8 *pixels = malloc(sizeof(UInt8) * width * 4 * height);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#endif

	fbo.end();

	fbo.setTexture(activePaintTexture);

	return ret;
}
/// 绘制Layer的擦除轨迹
void CZPaintingRender::drawLayerWithEraseMask(CZMat4 &projection)
{
	// use shader program
	CZShader *shader = shaders["blitWithEraseMask"];
	
	shader->begin();
	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"),1,GL_FALSE,projection);
	glUniform1i(shader->getUniformLocation("texture"), 0);
	glUniform1f(shader->getUniformLocation("opacity"), ptrLayer->opacity);
	glUniform1i(shader->getUniformLocation("mask"), 1);

	// Bind the texture to be used
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrLayer)->id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, getPaintTexture()->id);

	/// 配置绘制模式
	configureBlendMode(ptrLayer->blendMode);

	// setup projection matrix (orthographic)
	//mat4f_LoadOrtho(0, (GLuint) rect->size.width, 0, (GLuint) rect->size.height, -1.0f, 1.0f, proj);

	//CZAffineTransform translate = CZAffineTransform::makeFromTranslation(-rect->origin.x, -rect->origin.y);
	//mat4f_LoadCGAffineTransform(effectiveProj, translate);
	//mat4f_MultiplyMat4f(proj, effectiveProj, final);

	//glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, final);
	//glBindVertexArrayOES(self.painting.quadVAO);
	glBindVertexArray(getQuadVAO());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// unbind VAO
	//glBindVertexArrayOES(0);
	glBindVertexArray(0);

	shader->end();
}
/// 绘制Layer的绘画轨迹
void CZPaintingRender::drawLayerWithMask(CZMat4 &projection,CZColor *bgColor)
{
	// use shader program
	CZShader *shader = shaders["blitWithMask"];
	shader->begin();

	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"), 1, GL_FALSE, projection);
	glUniform1i(shader->getUniformLocation("texture"), 0);
	glUniform1f(shader->getUniformLocation("opacity"), ptrLayer->opacity);
	glUniform4f(shader->getUniformLocation("color"), bgColor->red, bgColor->green, bgColor->blue, bgColor->alpha);
	glUniform1i(shader->getUniformLocation("mask"), 1);
	glUniform1i(shader->getUniformLocation("lockAlpha"), ptrLayer->alphaLocked);

	// Bind the texture to be used
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrLayer)->id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, getPaintTexture()->id);

	/// 配置绘制模式
	configureBlendMode(ptrLayer->blendMode);
	// setup projection matrix (orthographic)
	//mat4f_LoadOrtho(0, (GLuint) rect->size.width, 0, (GLuint) rect->size.height, -1.0f, 1.0f, proj);

	//CZAffineTransform translate = CZAffineTransform::makeFromTranslation(-rect->origin.x, -rect->origin.y);
	//mat4f_LoadCGAffineTransform(effectiveProj, translate);
	//mat4f_MultiplyMat4f(proj, effectiveProj, final);

	//glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, final);
	//glBindVertexArrayOES(self.painting.quadVAO);
	glBindVertexArray(getQuadVAO());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// unbind VAO
	//glBindVertexArrayOES(0);
	glBindVertexArray(0);

	shader->end();
}
/// 将Layer的纹理绘制
void CZPaintingRender::drawLayer(CZMat4 &projection)
{
	// use shader program
	CZShader *shader = shaders["blit"];
	shader->begin();

	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"), 1, GL_FALSE, projection);
	glUniform1i(shader->getUniformLocation("texture"), 0);
	glUniform1f(shader->getUniformLocation("opacity"), ptrLayer->opacity);

	// Bind the texture to be used
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrLayer)->id);
	/// 配置绘制模式
	configureBlendMode(ptrLayer->blendMode);
	// setup projection matrix (orthographic)
	//mat4f_LoadOrtho(0, (GLuint) rect->size.width, 0, (GLuint) rect->size.height, -1.0f, 1.0f, proj);

	//CZAffineTransform translate = CZAffineTransform::makeFromTranslation(-rect->origin.x, -rect->origin.y);
	//mat4f_LoadCGAffineTransform(effectiveProj, translate);
	//mat4f_MultiplyMat4f(proj, effectiveProj, final);

	//glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, final);
	//glBindVertexArrayOES(self.painting.quadVAO);
	glBindVertexArray(getQuadVAO());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// unbind VAO
	//glBindVertexArrayOES(0);
	glBindVertexArray(0);

	shader->end();
}
/// 将Layer的纹理转换后绘制	
void CZPaintingRender::drawLayerWithTransform(CZMat4 &projection, const CZAffineTransform &transform)
{
	// use shader program
	CZShader *shader = shaders["blit"];
	shader->begin();

	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"), 1, GL_FALSE, projection);
	glUniform1i(shader->getUniformLocation("texture"), 0);
	glUniform1f(shader->getUniformLocation("opacity"), ptrLayer->opacity);

	// Bind the texture to be used
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrLayer)->id);

	/// 配置绘制模式
	configureBlendMode(ptrLayer->blendMode);

	CZRect rect(0,0,width,height);

	if (ptrLayer->clipWhenTransformed) 
	{
		glEnable(GL_STENCIL_TEST);
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		// All drawing commands fail the stencil test, and are not drawn, but increment the value in the stencil buffer.
		glStencilFunc(GL_NEVER, 0, 0);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);

		drawRect(rect,CZAffineTransform::makeIdentity());

		// now, allow drawing, except where the stencil pattern is 0x1 and do not make any further changes to the stencil buffer
		glStencilFunc(GL_EQUAL, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	drawRect(rect, ptrLayer->transform);

	if (ptrLayer->clipWhenTransformed) 
		glDisable(GL_STENCIL_TEST);

	shader->end();

	CZCheckGLError();
}
/// 将Layer的纹理带颜色调整后绘制	- for CZLayer
void CZPaintingRender::drawLayerWithcolorBalance(CZMat4 &projection, CZColorBalance *colorBalance)
{
	// use shader program
	CZShader *shader = shaders["colorBalanceBlit"];
	shader->begin();

	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"), 1, GL_FALSE, projection);
	glUniform1i(shader->getUniformLocation("texture"), 0);
	glUniform1f(shader->getUniformLocation("opacity"), ptrLayer->opacity);

	glUniform1f(shader->getUniformLocation("redShift"), colorBalance->redShift);
	glUniform1f(shader->getUniformLocation("greenShift"), colorBalance->greenShift);
	glUniform1f(shader->getUniformLocation("blueShift"), colorBalance->blueShift);
	glUniform1i(shader->getUniformLocation("premultiply"), 1);


	// Bind the texture to be used
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getLayerTexture(ptrLayer)->id);
	/// 配置绘制模式
	configureBlendMode(ptrLayer->blendMode);

	// setup projection matrix (orthographic)
	//mat4f_LoadOrtho(0, (GLuint) rect->size.width, 0, (GLuint) rect->size.height, -1.0f, 1.0f, proj);

	//CZAffineTransform translate = CZAffineTransform::makeFromTranslation(-rect->origin.x, -rect->origin.y);
	//mat4f_LoadCGAffineTransform(effectiveProj, translate);
	//mat4f_MultiplyMat4f(proj, effectiveProj, final);

	//glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, final);
	//glBindVertexArrayOES(self.painting.quadVAO);
	glBindVertexArray(getQuadVAO());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// unbind VAO
	//glBindVertexArrayOES(0);
	glBindVertexArray(0);

	shader->end();

}
/// 将Layer的纹理带色彩调整后绘制	- for CZLayer
void CZPaintingRender::drawLayerWithhueSaturation(CZMat4 &projection, CZHueSaturation *hueSaturation)
{
	// use shader program
	CZShader *shader = shaders["blitFromHueChromaLuma"];
	shader->begin();

	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"), 1, GL_FALSE, projection);
	glUniform1i(shader->getUniformLocation("texture"), 0);
	glUniform1f(shader->getUniformLocation("opacity"), ptrLayer->opacity);

	glUniform1f(shader->getUniformLocation("hueShift"), hueSaturation->hueShift);
	glUniform1f(shader->getUniformLocation("saturationShift"), hueSaturation->saturationShift);
	glUniform1f(shader->getUniformLocation("brightnessShift"), hueSaturation->brightnessShift);
	glUniform1i(shader->getUniformLocation("premultiply"), 1);

	// Bind the texture to be used
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getLayerHCLTexture(ptrLayer)->id);
	/// 配置绘制模式
	configureBlendMode(ptrLayer->blendMode);

	// setup projection matrix (orthographic)
	//mat4f_LoadOrtho(0, (GLuint) rect->size.width, 0, (GLuint) rect->size.height, -1.0f, 1.0f, proj);

	//CZAffineTransform translate = CZAffineTransform::makeFromTranslation(-rect->origin.x, -rect->origin.y);
	//mat4f_LoadCGAffineTransform(effectiveProj, translate);
	//mat4f_MultiplyMat4f(proj, effectiveProj, final);

	//glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, final);
	//glBindVertexArrayOES(self.painting.quadVAO);
	glBindVertexArray(getQuadVAO());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// unbind VAO
	//glBindVertexArrayOES(0);
	glBindVertexArray(0);

	shader->end();
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

	projectionMat.SetOrtho(0,width,0,height,-1.0f,1.0f);
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
