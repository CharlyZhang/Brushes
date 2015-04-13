
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
#include "brush/CZBrushPreview.h"
#include "CZUtil.h"

#if USE_OPENGL
#include "GL/glew.h"
#endif

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

	fbo->setColorRenderBuffer(width,height);

	/// ����ͶӰ����
	projMat.SetOrtho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);

}

/// ����һ���켣,������ͼ��
CZImage *CZPreviewRender::drawPath(CZPath *path)
{
	// set context
	fbo->begin();

	glClearColor(.0f, .0f, .0f, .0f);	
	glClear(GL_COLOR_BUFFER_BIT);

	brushShader->begin();

	glUniform1i(brushShader->getUniformLocation("texture"),0);
	glUniformMatrix4fv(brushShader->getUniformLocation("mvpMat"),1,GL_FALSE,projMat);
	CZCheckGLError();

	/// ���ƹ켣
	path->paint(path->getRandomizer());

	brushShader->end();

	CZImage *ret = new CZImage(width,height,CZImage::RGBA);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, ret->data);

	fbo->end();

	return ret;
}

/// ���ñ�ˢ����
void CZPreviewRender::configureBrush(CZImage *img)
{
	setContext();
	if(brushTexture == NULL)
	{
		brushTexture = CZTexture::produceFromImage(img);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,brushTexture->texId);
}
/// �����ˢ����
void CZPreviewRender::clearBrush()
{
	// set context
	if(brushTexture) {delete brushTexture; brushTexture=NULL;}
}
