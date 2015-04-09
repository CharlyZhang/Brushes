
///  \file CZLayer.cpp
///  \brief This is the file implements the class CZLayer.
///
///		
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note

#include "CZLayer.h"
#include "CZPainting.h"
#include "CZUtil.h"
#include <iostream>

using namespace std;

unsigned int CZLayer::layerNumber = 0;

CZLayer::CZLayer()
{
	visible = true;
	alphaLocked = false;
	locked =  false;
	colorBalance = NULL;
	hueSaturation = NULL;
	clipWhenTransformed = true;
	ptrPainting = NULL;
	transform = CZAffineTransform::makeIdentity();

	blendMode = CZRender::kBlendModeNormal;
	opacity = 1.0f;
	isSaved = kSaveStatusUnsaved;
	image = NULL;

	uid = LAYER_BASE_UID + (layerNumber++);
}
CZLayer::~CZLayer()
{
	if(image) { delete image; image = NULL;}
	if(colorBalance) { delete colorBalance; colorBalance = NULL;}
	if(hueSaturation) { delete hueSaturation; hueSaturation = NULL;}
}

/// 图层的图像数据
CZImage *CZLayer::imageData()
{
	CZRect bounds = ptrPainting->getBounds();
	return imageDataInRect(bounds);
}

/// 生成特定矩形区域的图像数据
CZImage *CZLayer::imageDataInRect(const CZRect &rect)
{
	if(!ptrPainting)
	{
		cerr << "CZLayer::imageDataInRect - ptrPainting is null\n";
		return NULL;
	}

	CZPaintingRender *ptrRender = ptrPainting->getRender();
	
	ptrRender->ptrLayer = this;
	CZImage *ret = ptrRender->drawLayerInRect(rect);
	
	return ret;
}

/// 绘制图层
void CZLayer::basicBlit(CZMat4 &projection)
{
	CZPaintingRender *ptrRender = ptrPainting->getRender();
	ptrRender->ptrLayer = this;

	ptrRender->drawLayer(projection);
}
/// 绘制图层（考虑移动转换、颜色调整等）
void CZLayer::blit(CZMat4 &projection)
{
	CZPaintingRender *ptrRender = ptrPainting->getRender();
	ptrRender->ptrLayer = this;

	if(!transform.isIdentity())
		ptrRender->drawLayerWithTransform(projection,transform);
	else if(colorBalance)
		ptrRender->drawLayerWithcolorBalance(projection,colorBalance);
	else if(hueSaturation)
		ptrRender->drawLayerWithhueSaturation(projection,hueSaturation);
	else
		ptrRender->drawLayer(projection);
}
/// 叠加擦除纹理
void CZLayer::blitWithEraseMask(CZMat4 &projection)
{
	CZPaintingRender *ptrRender = ptrPainting->getRender();
	ptrRender->ptrLayer = this;
	ptrRender->drawLayerWithEraseMask(projection);
}
/// 叠加绘制纹理
void CZLayer::blitWithMask(CZMat4 &projection,CZColor *bgColor)
{
	CZPaintingRender *ptrRender = ptrPainting->getRender();
	ptrRender->ptrLayer = this;
	ptrRender->drawLayerWithMask(projection,bgColor);
}

/// 将绘制的笔画合并到当前图层
void CZLayer::commitStroke(CZRect &bounds, CZColor &color, bool erase, bool undoable)
{
	//if (undoable) [self registerUndoInRect:bounds];

	//ptrPainting->beginSuppressingNotifications();

	isSaved = kSaveStatusUnsaved;

	CZPaintingRender * render = ptrPainting->getRender();
	render->ptrLayer = this;

	render->composeActivePaintTexture(color,erase);
}

/// 调整颜色
void CZLayer::commitColorAdjustments()
{
	/*
	WDShader *shader = nil;

	if (self.colorBalance) {
		shader = [self.painting getShader:@"colorBalanceBlit"];
	} else if (self.hueSaturation) {
		shader = [self.painting getShader:@"blitFromHueChromaLuma"];
	} else {
		return;
	}

	[self modifyWithBlock:^{
		// handle viewing matrices
		GLfloat proj[16];
		// setup projection matrix (orthographic)
		mat4f_LoadOrtho(0, self.painting.width, 0, self.painting.height, -1.0f, 1.0f, proj);

		glUseProgram(shader.program);

		glActiveTexture(GL_TEXTURE0);
		if (self.hueSaturation) {
			glBindTexture(GL_TEXTURE_2D, self.hueChromaLuma);
		} else {
			glBindTexture(GL_TEXTURE_2D, self.textureName);
		}

		glUniform1i(shader->locationForUniform:@"texture"], 0);
		glUniformMatrix4fv(shader->locationForUniform:@"modelViewProjectionMatrix"], 1, GL_FALSE, proj);

		if (self.colorBalance) {
			glUniform1f(shader->:@"redShift"], colorBalance_.redShift);
			glUniform1f(shader->locationForUniform:@"greenShift"], colorBalance_.greenShift);
			glUniform1f(shader->locationForUniform:@"blueShift"], colorBalance_.blueShift);
			glUniform1i(shader->locationForUniform:@"premultiply"], 0);
		} else {
			glUniform1f(shader->locationForUniform:@"hueShift"], hueSaturation_.hueShift);
			glUniform1f(shader->locationForUniform:@"saturationShift"], hueSaturation_.saturationShift);
			glUniform1f(shader->locationForUniform:@"brightnessShift"], hueSaturation_.brightnessShift);
			glUniform1i(shader->locationForUniform:@"premultiply"], 0);
		}

		glBlendFunc(GL_ONE, GL_ZERO);

		glBindVertexArrayOES(self.painting.quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// unbind VAO
		glBindVertexArrayOES(0);
	} newTexture:NO undoBits:YES];

	self.colorBalance = nil;
	self.hueSaturation = nil;
	*/
}


/// 合并另以图层
bool CZLayer::merge(CZLayer *layer)
{
	return true;
}

/// 设置绘制指针
void CZLayer::setPainting(CZPainting *painting)
{
	ptrPainting = painting;
}
/// 设置转换矩阵
bool CZLayer::setTransform(CZAffineTransform &trans)
{
	if(transform == trans) return false;

	transform = trans;
	return true;
}
/// 获取转换矩阵
CZAffineTransform & CZLayer::getTransform()
{
	return transform;
}
/// 设置混合模式
bool CZLayer::setBlendMode(CZRender::BlendMode &bm)
{
	if(bm == blendMode) return false;

	blendMode = bm;
	return true;
}
/// 获取混合模式
CZRender::BlendMode CZLayer::getBlendMode()
{
	return blendMode;
}
/// 设置不透明度
bool CZLayer::setOpacity(float o)
{
	if(opacity == o) return false;

	opacity = CZUtil::Clamp(0.0f,1.0f,o);
	return true;
}
/// 获取不透明度
float CZLayer::getOpacity()
{
	return opacity;
}
/// 设置调整颜色
bool CZLayer::setColorBalance(CZColorBalance *cb)
{
	if(cb && *cb ==  *colorBalance) return false;
	
	colorBalance = cb;
	return true;
}
/// 设置色调调整
bool CZLayer::setHueSaturation(CZHueSaturation *hs)
{
	return true;
}
/// 设置图层图像
/// 
///		\param img - 设置的图像
///		\ret	   - 若img不为空，则设置陈宫
///		\note 调用此函数将覆盖之前对该层的所有绘制
bool CZLayer::setImage(CZImage *img)
{
	if(img == NULL)
	{
		cerr << "CZLayer::setImage - image is NULL\n";
		return false;
	}

	if(image)
	{
		delete image;
		/// 删除对应的老纹理
		CZPaintingRender *ptrRender = ptrPainting->getRender();
		ptrRender->clearLayerTexture(this);
	}

	image = img;
    
    return true;
}

/// 切换可见性
void CZLayer::toggleVisibility()
{
	visible = !visible;
}
/// 切换alpha锁定
void CZLayer::toggleAlphaLocked()
{
	alphaLocked = !alphaLocked;
}
/// 切换图层锁定
void CZLayer::toggleLocked()
{
	locked = !locked;
}
/// 设置可见性
void CZLayer::setVisiblility(bool flag)
{
	visible = flag;
}
/// 设置alpha锁定
void CZLayer::setAlphaLocked(bool flag)
{
	alphaLocked = flag;
}
/// 设置图层锁定
void CZLayer::setLocked(bool flag)
{
	locked = flag;
}
/// 是否被锁住图层
bool CZLayer::isLocked()
{
	return locked;
};
/// 是否被锁住alpha
bool CZLayer::isAlphaLocked()
{
	return alphaLocked;
}
/// 是否可见
bool CZLayer::isVisible()
{
	return visible;
}

/// 是否可编辑
bool CZLayer::isEditable()
{
	return (!locked && visible);
}

/// 获取编号
unsigned int CZLayer::getUID()
{
	return uid;
}

/// 实现coding的接口
void CZLayer::update(CZDecoder *decoder_, bool deep /* = false */)
{
	/*
	// avoid setting these if they haven't changed, to prevent unnecessary notifications
	BOOL visible = [decoder decodeBooleanForKey:WDVisibleKey];
	if (visible != self.visible) {
	self.visible = visible;
	}
	BOOL locked = [decoder decodeBooleanForKey:WDLockedKey];
	if (locked != self.locked) {
	self.locked = locked;
	}
	BOOL alphaLocked = [decoder decodeBooleanForKey:WDAlphaLockedKey];
	if (alphaLocked != self.alphaLocked) {
	self.alphaLocked = alphaLocked;
	}

	WDBlendMode blendMode = (WDBlendMode) [decoder decodeIntegerForKey:WDBlendModeKey];
	blendMode = WDValidateBlendMode(blendMode);
	if (blendMode != self.blendMode) {
	self.blendMode = blendMode;
	}


	uuid_ = [decoder decodeStringForKey:WDUUIDKey];

	id opacity = [decoder decodeObjectForKey:WDOpacityKey];
	self.opacity = opacity ? [opacity floatValue] : 1.f;

	if (deep) {
	[decoder dispatch:^{
	loadedImageData_ = [[decoder decodeDataForKey:WDImageDataKey] decompress];
	self.isSaved = kWDSaveStatusSaved;
	}];
	}
	*/
}
void CZLayer::encode(CZCoder *coder_, bool deep /* = false */)
{
	/*
	[coder encodeBoolean:visible_ forKey:WDVisibleKey];
	[coder encodeBoolean:locked_ forKey:WDLockedKey];
	[coder encodeBoolean:alphaLocked_ forKey:WDAlphaLockedKey];
	[coder encodeInteger:blendMode_ forKey:WDBlendModeKey];
	[coder encodeString:uuid_ forKey:WDUUIDKey];

	if (opacity_ != 1.f) {
	[coder encodeFloat:opacity_ forKey:WDOpacityKey];
	}

	if (deep) {
	WDSaveStatus wasSaved = self.isSaved;
	self.isSaved = kWDSaveStatusTentative;
	id data = (wasSaved == kWDSaveStatusSaved) ? [NSNull null] : self.imageData; // don't bother retrieving imageData if we're not saving it
	WDTypedData *image = [WDTypedData data:data mediaType:@"image/x-brushes-layer" compress:YES uuid:self.uuid isSaved:wasSaved];
	[coder encodeDataProvider:image forKey:WDImageDataKey];
	}
	*/
}