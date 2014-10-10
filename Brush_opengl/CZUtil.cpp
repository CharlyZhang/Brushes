
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

using namespace std;

/// ȡ�����[0,1]
float CZRandomFloat()
{
	return rand()*1.0f / RAND_MAX;
}

/// ��ͬ��ɫģʽ��ת��
void HSVtoRGB(float h, float s, float v, float &r, float &g, float &b)
{
	if (s == 0) 
	{
		r = g = b = v;
	} 
	else 
	{
		float   f,p,q,t;
		int     i;

		h *= 360;

		if (h == 360.0f) 
		{
			h = 0.0f;
		}

		h /= 60;
		i = floor(h);

		f = h - i;
		p = v * (1.0 - s);
		q = v * (1.0 - (s*f));
		t = v * (1.0 - (s * (1.0 - f)));

		switch (i) {
		case 0: r = v; g = t; b = p; break;
		case 1: r = q; g = v; b = p; break;
		case 2: r = p; g = v; b = t; break;
		case 3: r = p; g = q; b = v; break;
		case 4: r = t; g = p; b = v; break;
		case 5: r = v; g = p; b = q; break;
		}
	}
}   
void RGBtoHSV(float r, float g, float b, float &h, float &s, float &v)
{
	float max = Max(r, Max(g, b));
	float min = Min(r, Min(g, b));
	float delta = max - min;

	v = max;
	s = (max != 0.0f) ? (delta / max) : 0.0f;

	if (s == 0.0f) 
	{
		h = 0.0f;
	} 
	else
	{
		if (r == max) {
			h = (g - b) / delta;
		} else if (g == max) {
			h = 2.0f + (b - r) / delta;
		} else if (b == max) {
			h = 4.0f + (r - g) / delta;
		}

		h *= 60.0f;

		if (h < 0.0f) 
		{
			h += 360.0f;
		}
	}

	h /= 360.0f;
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
void drawPathDataDirectly(vector<CZ2DPoint> &points)
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

/// ���Һ���,��[0,1]��[0,1] -CZFreehandTool������
float sineCurve(float input)
{
	float result;

	input *= M_PI; // move from [0.0, 1.0] tp [0.0, Pi]
	input -= M_PI_2; // shift back onto a trough

	result = sin(input) + 1; // add 1 to put in range [0.0,2.0]
	result /= 2; // back to [0.0, 1.0];

	return result;
}

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
				
				cout << i <<"\t" << j << endl;
				cout << pix[4*ind+0] << "\t"
						  << pix[4*ind+1] << "\t"
						  << pix[4*ind+2] << "\t"
						  << pix[4*ind+3] << "\n";
				over =true;
				//break;
		}
		//if(over) break;
	}

	cout << "finished!\n";

	delete [] pix;
}


void CZCheckGLError_(const char *file, int line){};
