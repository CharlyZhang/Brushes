
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
#include "CZNotificationCenter.h"

using namespace std;

string CZColorBalanceChanged = "ColorBalanceChanged";
string CZHueSaturationChanged = "HueSaturationChanged";
string CZLayerVisibilityChanged = "LayerVisibilityChanged";
string CZLayerLockedStatusChanged = "LayerLockedStatusChanged";
string CZLayerAlphaLockedStatusChanged = "LayerAlphaLockedStatusChanged";
string CZLayerOpacityChanged = "LayerOpacityChanged";
string CZLayerBlendModeChanged = "LayerBlendModeChanged";
string CZLayerContentsChangedNotification = "LayerContentsChangedNotification";
string CZLayerThumbnailChangedNotification = "LayerThumbnailChangedNotification";
string CZLayerTransformChangedNotification = "LayerTransformChangedNotification";

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
	CZRect bounds(0,0,ptrPainting->dimensions.width, ptrPainting->dimensions.height);
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

	CZPaintingRender *ptrRender = ptrPainting->render;
	
	ptrRender->ptrLayer = this;
	CZImage *ret = ptrRender->drawLayerInRect(rect);
	
	return ret;
}

/// 将图层内容按类型绘制
void CZLayer::blit(CZPaintingRender* ptrRender, CZMat4 &projection)
{
	ptrRender->ptrLayer = this;

	if(!transform.isIdentity())
		ptrRender->drawLayerWithTransform(projection,transform);
	else if(colorBalance)
		ptrRender->drawLayerWithcolorBalance(projection,colorBalance);
	else if(hueSaturation)
		ptrRender->drawLayerWithhueSaturation(projection,hueSaturation);
}
/// 叠加擦除纹理
void CZLayer::blitWithEraseMask(CZPaintingRender* ptrRender, CZMat4 &projection)
{
	ptrRender->ptrLayer = this;
	ptrRender->drawLayerWithEraseMask(projection);
}
/// 叠加绘制纹理
void CZLayer::blitWithMask(CZPaintingRender* ptrRender, CZMat4 &projection,CZColor *bgColor)
{
	ptrRender->ptrLayer = this;
	ptrRender->drawLayerWithMask(projection,bgColor);
}

/// 设置转换矩阵
void CZLayer::setTransform(CZAffineTransform &trans)
{
	transform = trans;

	CZNotificationCenter::getInstance()->notify(CZLayerTransformChangedNotification,ptrPainting);
}
/// 设置混合模式
void CZLayer::setBlendMode(CZRender::BlendMode &bm)
{
	if(bm == blendMode) return;

	//[[[self.painting undoManager] prepareWithInvocationTarget:self] setBlendMode:blendMode_];

	blendMode = bm;

	if (!isSuppressingNotifications()) 
	{
		CZNotificationCenter::getInstance()->notify(CZLayerBlendModeChanged,ptrPainting,this);
	
		/*NSDictionary *userInfo = @{@"layer": self,
			@"rect": [NSValue valueWithCGRect:self.painting.bounds]};

		[[NSNotificationCenter defaultCenter] postNotificationName:WDLayerBlendModeChanged
object:self.painting
userInfo:userInfo]; */
	}

}
/// 设置调整颜色
void CZLayer::setColorBalance(CZColorBalance *cb)
{
	if(cb && *cb ==  *colorBalance) return;
	
	colorBalance = cb;

	if (!isSuppressingNotifications()) 
	{
		CZNotificationCenter::getInstance()->notify(CZColorBalanceChanged,ptrPainting);
	}
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

/// 是否抑制消息
bool CZLayer::isSuppressingNotifications()
{
	if(!ptrPainting || ptrPainting->isSuppressingNotifications()) return true; // 没有被绘制，也不能发送消息
	return false;
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