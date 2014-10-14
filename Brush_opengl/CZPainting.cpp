
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

CZPainting::CZPainting(const CZSize &size)
{
	dimensions = size;
	colors.clear();
	brushes.clear();
	undoBrushes.clear();
	strokeCount = 0;

	fbo = NULL;
	ptrActivePath = NULL;
	activePaintTexture = NULL;

	render.init();
	loadShaders();
}
CZPainting::~CZPainting()
{
	if(fbo != NULL) {delete fbo; fbo = NULL;}
	if(activePaintTexture != NULL) {delete activePaintTexture; activePaintTexture = NULL;}
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

#if USE_OPENGL
	CZTexture *tex = getPaintTexture();
	reusableFBo.setTexture(tex);

	fbo->begin();

	glClearColor(.0f, .0f, .0f, .0f);
	glClear(GL_COLOR_BUFFER_BIT);

	{	/// �Է������ط��ƻ���������״̬
		glEnable(GL_BLEND);							
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	/// ���ù켣����
	CZShader *brushShader = configureBrush(path_->ptrBrush);
	//path_->ptrShader = brushShader;	///< !û�б�Ҫ
	brushShader->begin();
	/// ���ƹ켣
	//pathBounds = path_->paint(randomizer);
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
	CZImage *stamp = brush_->generator->getStamp();
	CZTexture *stampTex = stamp->toTexture();
	if(stampTex == NULL)
	{
		std::cerr << "CZBrushPreview::configureBrush - stampTex is NULL\n";
		return NULL;
	}
	glBindTexture(GL_TEXTURE_2D,stampTex->id);

	CZCheckGLError();
}

/// ��ȡ��Ӧ��Shader��ͬʱ�趨��ǰGL�����ģ�
CZShader* CZPainting::getShader(std::string shaderName)
{
	return shaders[shaderName];
}

/// ��ȡ����������
CZTexture* CZPainting::getPaintTexture()
{
	if(!activePaintTexture)
		activePaintTexture = new CZTexture(dimensions.width,dimensions.height);
	return activePaintTexture;
}

/// ����Shader
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
	/// ����ʱֻ����һ��shader
	CZShader *shader = new CZShader;
	shader->readVertextShader("brush.vert");
	shader->readFragmentShader("brush.frag");
	shader->setShader();

	shaders.insert(std::pair<std::string,CZShader*>("brush",shader));
}