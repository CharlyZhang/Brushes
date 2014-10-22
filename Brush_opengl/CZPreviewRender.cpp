
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
#include "CZBrushPreview.h"
#include "CZUtil.h"

using namespace  std;

CZPreviewRender::CZPreviewRender()
{
	ptrPreview = CZBrushPreview::getInstance();

	brushShader = NULL;
	brushTexture = NULL;

	if(brushShader == NULL)
	{
		vector<string> tmp1,tmp2;
		tmp1.push_back("inPosition");
		tmp1.push_back("inTexcoord");
		tmp1.push_back("alpha");
		tmp2.push_back("mvpMat");
		tmp2.push_back("texture");

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


/// ���û�����Ϣ
void CZPreviewRender::configure(int w, int h)
{
	width  = w;
	height = h;

	//~ ����context(���context�ͳߴ���أ������context)

	fbo.setColorRenderBuffer(width,height);

	/// ����ͶӰ����
	projMat.SetOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f);

}

/// ����һ���켣,������ͼ��
CZImage *CZPreviewRender::drawPath(CZPath *path)
{
	// set context
	fbo.begin();

	glClearColor(.0f, .0f, .0f, .0f);	
	glClear(GL_COLOR_BUFFER_BIT);

	brushShader->begin();

	glUniform1i(brushShader->getUniformLocation("texture"),0);
	glUniformMatrix4fv(brushShader->getUniformLocation("mvpMat"),1,GL_FALSE,projMat);
	CZCheckGLError();

	/// ���ƹ켣
	vertexData *data;
	unsigned int dataNum;
	path->getPaintData(path->getRandomizer(),dataNum,data);
	drawPathData(data,dataNum);
	delete [] data;

	brushShader->end();

	CZImage *ret = new CZImage(width,height,CZImage::RGBA);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, ret->data);

	fbo.end();

	return ret;
}

/// ���ñ�ˢ����
void CZPreviewRender::configureBrush(CZImage *img)
{
	// setContext
	if(brushTexture == NULL)
	{
		brushTexture = CZTexture::produceFromImage(img);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,brushTexture->id);
}
/// �����ˢ����
void CZPreviewRender::clearBrush()
{
	// set context
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
		
	/*	// ����opengl ����λ�ñ���ͨ�����·�ʽ����
		glEnableClientState (GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT , sizeof(vertexData), &data[0].x);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].s);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), &data[0].a);
		glEnableVertexAttribArray(2);

		/// ����
		glDrawArrays(GL_TRIANGLE_STRIP,0,n);
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
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(vertexData),0);
		// ������
		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,2,GL_FLOAT, GL_TRUE, sizeof(vertexData),0);
		// ������
		glBindBuffer(GL_ARRAY_BUFFER, mAttributeBufferObject);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), NULL);

		/// ����
		glDrawArrays(GL_TRIANGLE_STRIP,0,n);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/// ����
		glDeleteBuffers(1, &mVertexBufferObject);
		glDeleteBuffers(1, &mTexCoordBufferObject);
		glDeleteBuffers(1, &mAttributeBufferObject);
		
	#endif
		CZCheckGLError();
}