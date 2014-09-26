
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

/// 求最大最小值
float Max(float a, float b)
{
	return a>b ? a:b;
}
float Min(float a, float b)
{
	return a<b ? a:b;
}


/// 绘制轨迹数据（利用图形接口）
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
	// 装载顶点
	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &data[0].x, GL_STREAM_DRAW);
	// 装载纹理
	glGenBuffers(1, &mTexCoordBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &data[0].s, GL_STREAM_DRAW);
	// 装载属性
	glGenBuffers(1, &mAttributeBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mAttributeBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &data[0].a, GL_STREAM_DRAW);

	// 绑定顶点
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,sizeof(vertexData),0);
	// 绑定纹理
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,sizeof(vertexData),0);
	// 绑定属性
	glBindBuffer(GL_ARRAY_BUFFER, mAttributeBufferObject);
	GLuint alphaLoc = shader->getAttributeLocation("alpha");
	glEnableVertexAttribArray(alphaLoc);
	glVertexAttribPointer(alphaLoc, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), NULL);

	/// 绘制
	glDrawArrays(GL_TRIANGLE_STRIP,0,n);

	glDisableVertexAttribArray(alphaLoc);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// 消除
	glDeleteBuffers(1, &mVertexBufferObject);
	glDeleteBuffers(1, &mTexCoordBufferObject);
	glDeleteBuffers(1, &mAttributeBufferObject);

#endif
	CZCheckGLError();
}
/// 直接绘制轨迹数据（不带纹理）
void drawPathDataDirectly(std::vector<CZ2DPoint> &points)
{
#if USE_OPENGL
	//glEnable(GL_LINE_SMOOTH);		///< 个人感觉还是不启用抗锯齿来得好
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	GLfloat w = rand()*9/RAND_MAX +1;			///< 线大小原来是10以内
	glLineWidth(w);
	glPointSize(w*0.7);

	GLfloat c = rand()*1.0/RAND_MAX;
	glColor4f(c,c,c,c);
	int n = points.size();

	GLuint mVertexBufferObject;
	// 装载顶点
	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(CZ2DPoint), &points[0].x, GL_STREAM_DRAW);


	// 绑定顶点
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,0);

	/// 绘制
	glDrawArrays(GL_LINE_STRIP,0,n);
	//glDrawArrays(GL_POINTS,0,n);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// 消除
	glDeleteBuffers(1, &mVertexBufferObject);

	glDisable(GL_LINE_SMOOTH);
#endif
}

/// 添加监听器到预览 -CZBrushPreview类引用
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