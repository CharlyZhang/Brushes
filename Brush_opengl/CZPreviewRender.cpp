
///  \file CZPreviewRender.h
///  \brief This is the file declare the class CZPreviewRender.
///
///	
///
///  \version	1.0.1
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-14
///  \note


#include "CZPreviewRender.h"

using namespace  std;

CZPreviewRender::CZPreviewRender()
{
	brushShader = NULL;
	brushTexture = NULL;

	if(brushShader == NULL)
	{
		std::vector<std::string> tmp1,tmp2;
		brushShader = new CZShader("brush.vert","brush.frag",tmp1, tmp2);
	}

	// configure some default GL state
	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}
CZPreviewRender::~CZPreviewRender()
{
	if (brushShader)
	{
		delete brushShader;
		brushShader = NULL;
	}
	if (brushTexture)
	{
		delete brushTexture;
		brushTexture = NULL;
	}
}


/// ���û�����Ϣ���ı��ڲ�������
void CZPreviewRender::configure(map<string,void*> &conf)
{
	int *pW = (int*)conf["width"];
	int *pH = (int*)conf["height"];
	width  = *pW;
	height = *pH;

	//~ ����context

	fbo.setColorRenderBuffer(width,height);

	/// ����ͶӰ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) width, 0.0f, 
		(GLfloat) height, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
}

/// ��ʼ
void CZPreviewRender::begin(DrawType type)
{
	fbo.begin();

	glClearColor(.0f, .0f, .0f, .0f);	
	glClear(GL_COLOR_BUFFER_BIT);

	{	/// �Է������ط��ƻ���������״̬
		glEnable(GL_BLEND);							
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	glColor4f(1.0,1.0,1.0,0.5);

	brushShader->begin();
};

/// ����
void CZPreviewRender::draw(DrawType type, void* data /*= NULL*/, unsigned int num /*= 0*/)
{
	switch(type)
	{
	case kDrawPath:
		drawPathData((vertexData*)data,num);
	}
}

/// ����ͼ�����ݲ�����
CZImage* CZPreviewRender::imageForLastDraw()
{
	CZImage *ret = new CZImage(fbo.width,fbo.height,CZImage::RGBA);
	glReadPixels(0, 0, fbo.width, fbo.height, GL_RGBA, GL_FLOAT, ret->data);
	
	return ret;
}

/// ����
void CZPreviewRender::end(DrawType type)
{
	brushShader->end();
	fbo.end();
};

/// ���ñ�ˢ����
void CZPreviewRender::configureBrush(CZImage *img)
{
	if(brushTexture == NULL)
	{
		// setContext
		brushTexture = CZTexture::produceFromImage(img);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,brushTexture->id);
}
/// �����ˢ����
void CZPreviewRender::clearBrush()
{
	if(brushTexture) {delete brushTexture; brushTexture=NULL;}
}

/// ���ƹ켣���ݣ�����ͼ�νӿڣ�
void CZPreviewRender::drawPathData(vertexData *data, unsigned int n)
{
#if USE_OPENGL_ES
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), &data[0].x);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].s);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].a);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, n);
#endif

#if USE_OPENGL
	/*
	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT , sizeof(vertexData), &data[0].x); 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT, sizeof(vertexData), &data[0].s);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].a);
	*/
	GLuint mVertexBufferObject, mTexCoordBufferObject, mAttributeBufferObject;
	// װ�ض���
	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &data[0].x, GL_STREAM_DRAW);
	// װ������
	glGenBuffers(1, &mTexCoordBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &data[0].s, GL_STREAM_DRAW);
	// װ������
	glGenBuffers(1, &mAttributeBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mAttributeBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &data[0].a, GL_STREAM_DRAW);

	// �󶨶���
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,sizeof(vertexData),0);
	// ������
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,sizeof(vertexData),0);
	// ������
	glBindBuffer(GL_ARRAY_BUFFER, mAttributeBufferObject);
	GLuint alphaLoc = brushShader->getAttributeLocation("alpha");
	glEnableVertexAttribArray(alphaLoc);
	glVertexAttribPointer(alphaLoc, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), NULL);

	/// ����
	glDrawArrays(GL_TRIANGLE_STRIP,0,n);

	glDisableVertexAttribArray(alphaLoc);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// ����
	glDeleteBuffers(1, &mVertexBufferObject);
	glDeleteBuffers(1, &mTexCoordBufferObject);
	glDeleteBuffers(1, &mAttributeBufferObject);

#endif
	CZCheckGLError();
}