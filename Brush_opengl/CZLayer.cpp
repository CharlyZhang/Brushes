
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
	hueSaturation = false;
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
}

/// ͼ���ͼ������
CZImage *CZLayer::imageData()
{
	CZRect bounds(0,0,ptrPainting->dimensions.width, ptrPainting->dimensions.height);
	return imageDataInRect(bounds);
}

/// �����ض����������ͼ������
CZImage *CZLayer::imageDataInRect(const CZRect &rect)
{
	if(!ptrPainting)
	{
		cerr << "CZLayer::imageDataInRect - ptrPainting is null\n";
		return NULL;
	}

	CZRender *ptrRender = (CZRender *) (ptrPainting->render);
	
	map<string,void*> conf;
	/// ������Ⱦ��
	conf.insert(make_pair("colorBuffer",(void*)&rect.size));
	conf.insert(make_pair("layer",(void*)this));
	ptrRender->configure(conf);
	
	ptrRender->begin(CZRender::kPaingtingImageData);

	ptrRender->draw(CZRender::kPaingtingImageData,(void*) &rect, 1);

	/// ���ص�ͼ��
	CZImage *ret = ptrRender->imageForLastDraw();

	ptrRender->end(CZRender::kPaingtingImageData);
	
	return ret;
}

/// ��ͼ�����ݰ����ͻ��ƣ���� or ������
void CZLayer::blit(float *proj, CZRender* ptrRender, CZRender::DrawType type, CZColor *color /* = NULL */)
{
	/// ������׼����
	ptrProjection = proj;
	ptrColor = color;

	/// ��һ��ϸ��չ�ֵ�ģʽ
	if(type == CZRender::kPaintingBlit)
	{
		/// ����λ��
		if(!transform.isIdentity()) 
			type = CZRender::kPaintingBlitTransform;
		else if(colorBalance)
			type = CZRender::kPaintingBlitColorBalance;
		else if(hueSaturation)
			type = CZRender::kPaintingBlitHueSaturation;
	}

	ptrRender->begin(type);
	
	ptrRender->draw(type,NULL,0);

	ptrRender->end(type);
}

/// ����ת������
void CZLayer::setTransform(CZAffineTransform &trans)
{
	transform = trans;

	CZNotificationCenter::getInstance()->notify(CZLayerTransformChangedNotification,ptrPainting);
}
/// ���û��ģʽ
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
/// ���õ�����ɫ
void CZLayer::setColorBalance(CZColorBalance *cb)
{
	if(cb && *cb ==  *colorBalance) return;
	
	colorBalance = cb;

	if (!isSuppressingNotifications()) 
	{
		CZNotificationCenter::getInstance()->notify(CZColorBalanceChanged,ptrPainting);
	}
}

/// ������ɫ
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

/// �л��ɼ���
void CZLayer::toggleVisibility()
{
	visible = !visible;
}
/// �л�alpha����
void CZLayer::toggleAlphaLocked()
{
	alphaLocked = !alphaLocked;
}
/// �л�ͼ������
void CZLayer::toggleLocked()
{
	locked = !locked;
}

/// �Ƿ�������Ϣ
bool CZLayer::isSuppressingNotifications()
{
	if(!ptrPainting || ptrPainting->isSuppressingNotifications()) return true; // û�б����ƣ�Ҳ���ܷ�����Ϣ
	return false;
}

/// ʵ��coding�Ľӿ�
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