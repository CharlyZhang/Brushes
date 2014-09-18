//#include "stdafx.h"
#include "assert.h"

#ifndef __glew_h__
 #include"GL\glew.h"
#endif
#include"CFbo.h"
//PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
//PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
//PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
//PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
//PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
//PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
//PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
//PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
//PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
//PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");

CFbo::CFbo(int _bufferWidth, int _bufferHeight, int _numBuffers):bufferWidth(_bufferWidth),
	bufferHeight(_bufferHeight),numBuffers(_numBuffers)
{
	texID = new GLuint[_numBuffers];
	memset(texID,0,_numBuffers*sizeof(GLuint));
	texArray = 0;
	renderTexId = 0;
}

CFbo::~CFbo()
{
	glDeleteFramebuffersEXT(1,&fboID);
	glDeleteRenderbuffersEXT(1,&depthID);
	if(texID[0]) glDeleteTextures(numBuffers,texID);
	delete[] texID;

	if(texArray) glDeleteTextures(1,&texArray);
}

void CFbo::init(texType textype)
{
	fboType = TEXTURE;

	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	
	GLint texFileterPara;
	if		(textype == TEXTURE)	texFileterPara = GL_NEAREST;
	else if (textype == SHADOWMAP)	texFileterPara = GL_NEAREST;	///

	glGenTextures (numBuffers, texID);
	for(int i=0;i<numBuffers;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texFileterPara);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texFileterPara);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		GLfloat ones[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, ones);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, bufferWidth, bufferHeight, 0,
			GL_RGBA, GL_FLOAT, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i/*COLOR_ATTACHMENT[i]*/, GL_TEXTURE_2D, texID[i], 0);
	}

	//申请Z BUFFER
	glGenRenderbuffersEXT(1,&depthID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,depthID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,bufferWidth,bufferHeight);
	//将深度缓冲与FBO绑定
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,depthID);

	//check status
	CheckFramebufferStatus();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
void CFbo::init_Shadow()
{
	fboType = SHADOWMAP;

	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	// Try to use a texture depth component
	glGenTextures(1, &depthID);
	glBindTexture(GL_TEXTURE_2D, depthID);

	// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Remove artefact on the edges of the shadowmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	GLfloat ones[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, ones);

	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available 
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, depthID, 0);

	//check status
	CheckFramebufferStatus();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
void CFbo::init_TexArray(int layerNum)
{
	fboType = TEXARRAY;

	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	glGenTextures(1, &texArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texArray);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	GLfloat ones[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, ones);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, bufferWidth, bufferHeight, layerNum,0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// attach the texture to FBO depth attachment point
	glFramebufferTextureLayer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,texArray,0,0);

	//check status
	CheckFramebufferStatus();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
void CFbo::init_ZBufferFree()
{
	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	glGenTextures (numBuffers, texID);
	for(int i=0;i<numBuffers;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, bufferWidth, bufferHeight, 0,
			GL_RGBA, GL_FLOAT, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i/*COLOR_ATTACHMENT[i]*/, GL_TEXTURE_2D, texID[i], 0);
	}
	//check status
	CheckFramebufferStatus();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
void CFbo::init_Layered_ZBFree(GLint internalFormat)
{
	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	glGenTextures (numBuffers, texID);
	for(int i=0;i<numBuffers;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat/*GL_RGBA32F_ARB*//*GL_ALPHA32F_ARB*/, bufferWidth>>i, bufferHeight>>i, 0,
			GL_RGBA, GL_FLOAT, 0);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, /*GL_COLOR_ATTACHMENT0_EXT+i*/COLOR_ATTACHMENT[i], GL_TEXTURE_2D, texID[i], 0);
	}
	//check status
	CheckFramebufferStatus();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
void CFbo::init_Stencil()
{
	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	glGenTextures (numBuffers, texID);
	for(int i=0;i<numBuffers;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, bufferWidth, bufferHeight, 0,
			GL_RGBA, GL_FLOAT, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i/*COLOR_ATTACHMENT[i]*/, GL_TEXTURE_2D, texID[i], 0);
	}

	//申请Z BUFFER
	glGenRenderbuffersEXT(1,&depthID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,depthID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,bufferWidth,bufferHeight);
	//将深度缓冲与FBO绑定
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,depthID);

	//申请stencil
	glGenRenderbuffersEXT(1,&stencilID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,stencilID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_STENCIL_INDEX,bufferWidth,bufferHeight);
	//将深度缓冲与FBO绑定
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_STENCIL_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,stencilID);


	//check status
	CheckFramebufferStatus();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void CFbo::begin(int colorAttachment, int numDrawBuffers,int texArrayLayer)
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,&preFbo);//获取当前的FBO，用于还原
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID); 
	if (fboType == TEXTURE)
	{
		if(numDrawBuffers==1)
			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + colorAttachment);
		else
			glDrawBuffers(numDrawBuffers,COLOR_ATTACHMENT + colorAttachment);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// add by ZXW
	}
	else if (fboType == SHADOWMAP)
	{
		glClear(GL_DEPTH_BUFFER_BIT);	
	}
	else if (fboType == TEXARRAY)
	{
		// attach the texture to FBO depth attachment point
		glFramebufferTextureLayer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,texArray,0,texArrayLayer);
		//check status
		//CheckFramebufferStatus();
		glClear(GL_DEPTH_BUFFER_BIT);	
	}
	
	glPushAttrib(/*GL_ALL_ATTRIB_BITS*/GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,bufferWidth,bufferHeight);
}
void CFbo::begin_NoClear(int colorAttachment, int numDrawBuffers)
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,&preFbo);//获取当前的FBO，用于还原
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID); 
	if(numDrawBuffers==1)
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + colorAttachment);
	else
		glDrawBuffers(numDrawBuffers,COLOR_ATTACHMENT + colorAttachment);

	glPushAttrib(/*GL_ALL_ATTRIB_BITS*/GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,bufferWidth,bufferHeight);
}
void CFbo::begin_clearDepthWhite(int colorAttachment, int numDrawBuffers, int depthAttachment)
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,&preFbo);//获取当前的FBO，用于还原
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID); 

	if(numDrawBuffers==1)
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + colorAttachment);
	else
		glDrawBuffers(numDrawBuffers,COLOR_ATTACHMENT + colorAttachment);

	glClearColor(0,0,0,0);		///set the alpha channel as 0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// add by ZXW

	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + depthAttachment);
	//glClearColor(1.0,1.0,1.0,1.0);
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if(numDrawBuffers==1)
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + colorAttachment);
	else
		glDrawBuffers(numDrawBuffers,COLOR_ATTACHMENT + colorAttachment);

	glPushAttrib(/*GL_ALL_ATTRIB_BITS*/GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,bufferWidth,bufferHeight);
}
void CFbo::begin_pingpong()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,&preFbo);//获取当前的FBO，用于还原
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID); 
	
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + renderTexId);

	glPushAttrib(/*GL_ALL_ATTRIB_BITS*/GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,bufferWidth,bufferHeight);
}
void CFbo::end()
{
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, preFbo);

	renderTexId = (renderTexId+1) % numBuffers;
}

GLuint CFbo::getTexID_pingpong()
{
	return this->texID[(renderTexId+1) % numBuffers];
}
/*
 *	glBindFramebufferEXT
 *	glPushAttrib(GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT)
 */
void CFbo::bind()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,&preFbo);//获取当前的FBO，用于还原
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	glPushAttrib(/*GL_ALL_ATTRIB_BITS*/GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
 *	undo glBindFramebufferEXT
 *	glPopAttrib
 */
void CFbo::unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, preFbo);
	glPopAttrib();
}

/*
 *	glViewport
 *	glDrawBuffer or glDrawBuffers
 */
void CFbo::draw( int x, int y, int width, int height, int colorAttachment /*= 0*/, int numDrawBuffers /*= 1*/ )
{
	glViewport(x,y,width,height);
	if(numDrawBuffers==1)
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + colorAttachment);
//	else
//		glDrawBuffers(numDrawBuffers,COLOR_ATTACHMENT + colorAttachment);
}

GLuint CFbo::getFboID()
{
	return this->fboID;
}
GLuint CFbo::getTexID(int texIdx)
{
	return this->texID[texIdx];
}

void CFbo::showDepthOnScreen( int x,int y,int width /*= 128*/,int height /*= 128*/,GLenum type /*= GL_TEXTURE_2D*/ )
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(type);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(x,y,width,height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glBindTexture(type,depthID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  0.0f);
	glEnd();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glDisable(type);
	glEnable(GL_DEPTH_TEST);
}
void CFbo::showTextureOnScreen( int x,int y,int width /*= 128*/,int height /*= 128*/,GLuint tID /*= 0*/,GLenum type /*= GL_TEXTURE_2D*/ )
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(type);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(x,y,width,height);
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
	    glPushMatrix();
		glLoadIdentity();
    glBindTexture(type,texID[tID]);
    glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  0.0f);
    glEnd();
   	glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
    glMatrixMode(GL_PROJECTION);
	    glPopMatrix();
	glPopAttrib();
	glDisable(type);
	glEnable(GL_DEPTH_TEST);
}
void CFbo::showTextureOnScreen_Replace( int x,int y,int width /*= 128*/,int height /*= 128*/,GLuint tID /*= 0*/,GLenum type /*= GL_TEXTURE_2D*/ )
{
//	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(type);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(x,y,width,height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glBindTexture(type,texID[tID]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  0.0f);
	glEnd();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glDisable(type);
	glEnable(GL_DEPTH_TEST);
}

void CFbo::readPixel( int tIdx,int x, int y, int width, int height )
{
	float *pData = NULL;
	pData = new float[width * height * 4];
	//memset(pData, 0, width * height * 4 * sizeof(float));

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID); 
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
		GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, texID[tIdx], 0);
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0,0,width,height,GL_RGBA,GL_FLOAT,pData);

	//int pos = 0;
	//for (int i = 0;i < height; ++i)
	//{
	//	for (int j = 0;j < width; ++j)
	//	{
	//		printf(" <%f,%f,%f,%f> ",pData[pos++],pData[pos++],pData[pos++],pData[pos++]);
	//	}
	//}
	for (int i = 0;i < 4;++i)
	{
		printf("%f ",pData[i]);
	}
	printf("\n");

	delete [] pData;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//////////////////////////////////////////////////////////////////////////

	float aabb[4];
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,texID[tIdx], 0);

	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glReadPixels(0,0,1, 1,GL_RGBA,GL_FLOAT,aabb);
	//printf("aabb_min: %f %f %f %f\n",aabb[0],aabb[1],aabb[2],aabb[3]);
	for (int i = 0;i < 4;++i)
	{
		printf("%f ",aabb[i]);
	}
	printf("\n");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void CFbo::CheckFramebufferStatus()
{
	int status;
	status = (GLenum) glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			printf("Unsupported framebuffer format\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			printf("Framebuffer incomplete, missing attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			printf("Framebuffer incomplete, incomplete attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			printf("Framebuffer incomplete, attached images must have same dimensions\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			printf("Framebuffer incomplete, attached images must have same format\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			printf("Framebuffer incomplete, missing draw buffer\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			printf("Framebuffer incomplete, missing read buffer\n");
			break;
		default:
			exit(0);
	}
}
