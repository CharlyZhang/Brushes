
///  \file CZUtil.cpp
///  \brief This is the file implement a series of utilities.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-11
///  \note

#include "CZUtil.h"
#include "Macro.h"
#include "gl/glut.h"

/// �������Сֵ
float Max(float a, float b)
{
	return a>b ? a:b;
}
float Min(float a, float b)
{
	return a<b ? a:b;
}


/// ���ƹ켣���ݣ�����ͼ�νӿڣ�
void drawPathData(vertexData *data, unsigned int n, CZShader *shader)
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
	GLuint alphaLoc = shader->getAttributeLocation("alpha");
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
/// ֱ�ӻ��ƹ켣���ݣ���������
void drawPathDataDirectly(std::vector<CZ2DPoint> &points)
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
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(CZ2DPoint), &points[0].x, GL_STREAM_DRAW);


	// �󶨶���
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,0);

	/// ����
	glDrawArrays(GL_LINE_STRIP,0,n);
	//glDrawArrays(GL_POINTS,0,n);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// ����
	glDeleteBuffers(1, &mVertexBufferObject);

	glDisable(GL_LINE_SMOOTH);
#endif
}

/// ��Ӽ�������Ԥ�� -CZBrushPreview������
void addObserver2Preview()
{
	/*
	[[NSNotificationCenter defaultCenter] addObserver:self
selector:@selector(brushGeneratorChanged:)
name:WDBrushGeneratorChanged
object:nil];

	[[NSNotificationCenter defaultCenter] addObserver:self
selector:@selector(brushGeneratorChanged:)
name:WDBrushGeneratorReplaced
object:nil];
*/
};

#include <iostream>

void checkPixels(int w_, int h_)
{
	float *pix = new float[w_*h_*4];
	glReadPixels(0,0,w_,h_,GL_RGBA, GL_FLOAT,pix);
	
	bool over = false;
	for(int i=0; i<h_; i++)
	{
		for(int j=0; j<w_; j++)
		{
			int ind = i*w_+j;
			if( pix[4*ind+0] != 0 ||
				pix[4*ind+1] != 0 ||
				pix[4*ind+2] != 0 ||
				pix[4*ind+3] != 1.0)
				
				std::cout << i <<"\t" << j << std::endl;
				std::cout << pix[4*ind+0] << "\t"
						  << pix[4*ind+1] << "\t"
						  << pix[4*ind+2] << "\t"
						  << pix[4*ind+3] << "\n";
				over =true;
				break;
		}
		if(over) break;
	}

	std::cout << "finished!\n";

	delete [] pix;
}