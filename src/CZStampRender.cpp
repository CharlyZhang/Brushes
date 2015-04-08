
///  \file CZStampRender.cpp
///  \brief This is the file implements the Class CZStampRender.
///
///		�����������Ʊ�ˢͼ�εĻ��������Ǹ�������
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note

#include "CZStampRender.h"
#include "CZStampGenerator.h"
#include <vector>
#include <string>

#if USE_OPENGL
#include "GL/glew.h"
#endif

using namespace std;

CZStampRender::CZStampRender()
{
	// ����context ������

	// configure some default GL state
	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	vector<string> attributes, uniforms;
	attributes.push_back("inPosition");
	uniforms.push_back("mvpMat");
	shader = new CZShader("stamp.vert","stamp.frag",attributes,uniforms);

	fbo = new CZFbo;

	CZCheckGLError();
}
CZStampRender::~CZStampRender()
{
	if(shader)	{	delete shader; shader = NULL; }
	if(fbo)	{	delete fbo; fbo = NULL; }
}

/// ���ô�С
void CZStampRender::configure(int w, int h)
{
	if(width == w && height == h) return;

	width = w;
	height = h;
	fbo->setColorRenderBuffer(width,height);
	projMat.SetOrtho(0.0f ,width, 0.0f, height, -1.0f, 1.0f);
}

/// ����stampͼ��
CZImage *CZStampRender::drawStamp()
{
	setContext();

	//glDisable(GL_TEXTURE_2D);

	fbo->begin();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->begin();

	glUniformMatrix4fv(shader->getUniformLocation("mvpMat"),1,GL_FALSE,projMat);
	ptrGenerator->renderStamp();

	shader->end();

	CZImage *ret = new CZImage(width,height,CZImage::RGBA);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_PIXEL_TYPE, ret->data);

	fbo->end();

	return ret;
}

/// ����������
void CZStampRender::drawSpiralData(std::vector<CZ2DPoint> &points)
{
	//glEnable(GL_LINE_SMOOTH);		///< ���˸о����ǲ����ÿ�������ú�
	GLfloat w = rand()*9/RAND_MAX +1;			///< �ߴ�Сԭ����10����
	glLineWidth(w);
	//glPointSize(w*0.7);   // NOT SUPPORTED IN ES
	int n = points.size();
    
#if USE_OPENGL
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

	GLfloat c = rand()*1.0/RAND_MAX;
	glColor4f(c,c,c,c);

	GLuint mVertexBufferObject;
	// װ�ض���
	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(CZ2DPoint), &points[0].x, GL_STREAM_DRAW);


	// �󶨶���
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(CZ2DPoint),0);

	/// ����
	glDrawArrays(GL_LINE_STRIP,0,n);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// ����
	glDeleteBuffers(1, &mVertexBufferObject);

	glDisable(GL_LINE_SMOOTH);

#elif USE_OPENGL_ES
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CZ2DPoint), &points[0].x);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, n);
    glDisableVertexAttribArray(0);
#endif
}
void CZStampRender::drawSpiralData(std::vector<CZ3DPoint> &points)
{
#if USE_OPENGL
	//glEnable(GL_LINE_SMOOTH);		///< ���˸о����ǲ����ÿ�������ú�
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	GLfloat w = rand()*9/RAND_MAX +1;			///< �ߴ�Сԭ����10����
	glLineWidth(w);
	glPointSize(w*0.7);

	GLfloat c = rand()*1.0/RAND_MAX;
	glColor4f(c,c,c,c);
	int n = points.size();

	GLuint mVertexBufferObject;
	// װ�ض���
	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(CZ3DPoint), &points[0].x, GL_STREAM_DRAW);


	// �󶨶���
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(CZ3DPoint),0);

	/// ����
	glDrawArrays(GL_LINE_STRIP,0,n);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// ����
	glDeleteBuffers(1, &mVertexBufferObject);

	//glDisable(GL_LINE_SMOOTH);

#elif USE_OPENGL_ES
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CZ2DPoint), &points[0].x);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_STRIP, 0, n);
	glDisableVertexAttribArray(0);
#endif
	CZCheckGLError();

}