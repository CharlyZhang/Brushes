
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
#include "Macro.h"
#include "CZUtil.h"

CZPainting::CZPainting()
{
	fbo = NULL;
	ptrActivePath = NULL;
}
CZPainting::~CZPainting()
{
	if(fbo != NULL) {delete fbo; fbo = NULL;}
}

/// ����һ���켣
CZRect CZPainting::paintStroke(CZPath *path_, CZRandom *randomizer, bool clearBuffer /* = false */)
{
	ptrActivePath = path_;

	CZRect pathBounds = CZRect::zeroRect();

#if USE_OPENGL_ES
	[EAGLContext setCurrentContext:self.context];
	glBindFramebuffer(GL_FRAMEBUFFER, self.reusableFramebuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self.activePaintTexture, 0);

	GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status == GL_FRAMEBUFFER_COMPLETE) {
		glViewport(0, 0, self.width, self.height);

		if (clearBuffer) {
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		[self configureBrush:path.brush];
		pathBounds = [path paint:randomizer];
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	WDCheckGLError();
#endif

#if USE_OPENGL && 0
	fbo
	fbo->begin();

	glClearColor(.0f, .0f, .0f, .0f);
	glClear(GL_COLOR_BUFFER_BIT);

	{	/// �Է������ط��ƻ���������״̬
		glEnable(GL_BLEND);							
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	/// ���ù켣����
	path->setBrush(ptrBrush);
	path->remainder = 0.0f;
	path->setClosed(false);
	path->shader = brushShader;		///< !û�б�Ҫ

	glColor4f(1.0,1.0,1.0,0.5);
	brushShader->begin();
	/// ���ƹ켣
	path->paint();
	brushShader->end();

	fbo->end();
#endif

	//NSDictionary *userInfo = @{@"rect": [NSValue valueWithCGRect:pathBounds]};
	//[[NSNotificationCenter defaultCenter] postNotificationName:WDStrokeAddedNotification object:self userInfo:userInfo];

	return pathBounds;
}

/// ���ñ�ˢ
///	
///		���úû����ñ�ˢ�������Լ�Shader������CZPainting���漰�����ֱ�ˢ������������ǰ��Ҫ����ǰ�õı�ˢ���úá�
/// 
CZShader* CZPainting::configureBrush(CZBrush *brush_)
{
	CZShader *brushShader = getShader("brush");

#if USE_OPENGL_ES
	glUseProgram(brushShader.program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, [self brushTexture:brush].textureName);

	glUniform1i([brushShader locationForUniform:@"texture"], 0);
	glUniformMatrix4fv([brushShader locationForUniform:@"modelViewProjectionMatrix"], 1, GL_FALSE, projection_);
#endif

	/// ������
	CZTexture *stampTex = brush_->getTexture(true);
	if(stampTex == NULL)
	{
		std::cerr << "CZBrushPreview::configureBrush - stampTex is NULL\n";
		return NULL;
	}
	glBindTexture(GL_TEXTURE_2D,stampTex->id);

	CZCheckGLError();
}