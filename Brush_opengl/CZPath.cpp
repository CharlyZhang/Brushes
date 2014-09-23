
///  \file CZPath.cpp
///  \brief This is the file implement the Class CPath.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-11
///  \note

#include "CZPath.h"
#include "CZAffineTransform.h"
#include "CZUtil.h"
#include "Macro.h"
#include "gl/glew.h"
#include "gl/glut.h"
#include <cmath>				
#include <iostream>

#define M_PI       3.14159265358979323846
 
/// 绘制轨迹
CZRect CZPath::paint(bool withBrush/* = true*//*randomizer*/)
{    
	this->points.clear();
	this->sizes.clear();
	this->alphas.clear();
	this->angles.clear();

	if (this->nodes.size() == 1) 
	{
		//[self paintStamp:randomizer];
	} 
	else 
	{
		std::vector<CZ3DPoint> linePoints;		/// <贝塞尔曲线的绘制点
		int numPoints = this->flattenedPoints(linePoints);

		if(withBrush)
		{
			if(brush == NULL) 
			{
				std::cerr << "CZPath::paint - Brush is NULL\n";
				return CZRect();
			}

			for (int ix = 0; ix < numPoints - 1; ix++) 
			{
				this->paintBetweenPoints(linePoints[ix],linePoints[ix+1]);
			}

			return this->drawData();
		}
		else
		{
			for (int ix = 0; ix < numPoints; ix++) 
			{
				points.push_back(CZ2DPoint(linePoints[ix].x, linePoints[ix].y));
			}
			drawDataDirectly();
		}
	}

}

/// 设置闭合
void CZPath::setClosed(bool closed_)
{
	closed = closed_;
}

/// 绘制数据（利用OpenGL等图形接口）
/// 
///		以最小粒度的离散点(points_)为中心，形成小矩形。并将此矩形数据通过图形接口绘制出来。
///
CZRect CZPath::drawData()
{
	typedef struct {
		GLfloat     x, y;
		GLfloat     s, t;
		GLfloat     a;
	} vertexData;

	int iPointSize = points.size();
	vertexData *vertexD = new vertexData[iPointSize * 4 + (iPointSize - 1) * 2];
	CZRect dataBounds = CZRect(0.0f,0.0f,0.0f,0.0f);

	//////////////////////////////////////////////////////////////////////////
#if RENDER_STAMP		///< Render One Rect Use Brush	
	delete [] vertexD;
	points.clear();
	angles.clear();
	float s = sizes.front();
	sizes.clear();
	alphas.clear();

	points.push_back(CZ2DPoint(300,300));
	angles.push_back(0);
	sizes.push_back(s);
	alphas.push_back(1.0);
	
	iPointSize = points.size();
	vertexD = new vertexData[iPointSize * 4 + (iPointSize - 1) * 2];
#endif
	//////////////////////////////////////////////////////////////////////////
	int n = 0;
	for (int i = 0; i < iPointSize; i++) 
	{
		CZ2DPoint result = points[i];
		float angle = angles[i];
		float size = sizes[i] / 2;
		float alpha = alphas[i];

		// get the four corners of the dot
		CZRect rect = CZRect(result.x - size, result.y - size, size*2, size*2);
		CZ2DPoint a = CZ2DPoint(rect.getMinX(), rect.getMinY());
		CZ2DPoint b = CZ2DPoint(rect.getMaxX(), rect.getMinY());
		CZ2DPoint c = CZ2DPoint(rect.getMinX(), rect.getMaxY());
		CZ2DPoint d = CZ2DPoint(rect.getMaxX(), rect.getMaxY());

		// rotate the dot
		CZ2DPoint center = rect.getCenter();
		CZAffineTransform t = CZAffineTransform::makeFromTranslation(center.x, center.y);
		t.rotate(angle);
		t.translate(-center.x, -center.y);

		a = t.applyTo2DPoint(a);
		b = t.applyTo2DPoint(b);
		c = t.applyTo2DPoint(c);
		d = t.applyTo2DPoint(d);

		// update the bounds box of data
		CZRect boxBounds = t.applyToRect(rect);
		dataBounds = dataBounds.unionWith(boxBounds.expandToIntergral());

		if (n != 0) {
			// dup 1st vertex
			vertexD[n].x = a.x;
			vertexD[n].y = a.y;
			vertexD[n].s = 0;
			vertexD[n].t = 0;
			vertexD[n].a = alpha;
			n++;
		}

		vertexD[n].x = a.x;
		vertexD[n].y = a.y;
		vertexD[n].s = 0;
		vertexD[n].t = 0;
		vertexD[n].a = alpha;
		n++;

		vertexD[n].x = b.x;
		vertexD[n].y = b.y;
		vertexD[n].s = 1;
		vertexD[n].t = 0;
		vertexD[n].a = alpha;
		n++;

		vertexD[n].x = c.x;
		vertexD[n].y = c.y;
		vertexD[n].s = 0;
		vertexD[n].t = 1;
		vertexD[n].a = alpha;
		n++;

		vertexD[n].x = d.x;
		vertexD[n].y = d.y;
		vertexD[n].s = 1;
		vertexD[n].t = 1;
		vertexD[n].a = alpha;
		n++;

		if (i != (iPointSize - 1)) {
			// dup last vertex
			vertexD[n].x = d.x;
			vertexD[n].y = d.y;
			vertexD[n].s = 1;
			vertexD[n].t = 1;
			vertexD[n].a = alpha;
			n++;
		}
	}

#if USE_OPENGL_ES
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), &vertexD[0].x);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(vertexData), &vertexD[0].s);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), &vertexD[0].a);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, n);
#endif

#if USE_OPENGL
	/*
	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT , sizeof(vertexData), &vertexD[0].x); 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT, sizeof(vertexData), &vertexD[0].s);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), &vertexD[0].a);
	*/
	GLuint mVertexBufferObject, mTexCoordBufferObject, mAttributeBufferObject;
	// 装载顶点
	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &vertexD[0].x, GL_STREAM_DRAW);
	// 装载纹理
	glGenBuffers(1, &mTexCoordBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &vertexD[0].s, GL_STREAM_DRAW);
	// 装载属性
	glGenBuffers(1, &mAttributeBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mAttributeBufferObject);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(vertexData), &vertexD[0].a, GL_STREAM_DRAW);

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

	delete [] vertexD;
	//WDCheckGLError();

	return dataBounds;
}

/// 直接绘制数据（利用OpenGL等图形接口）
/// 
///		利用图形接口直接将数据绘制出来。
///
CZRect CZPath::drawDataDirectly()
{
	
#if USE_OPENGL
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
	glDrawArrays(GL_POINTS,0,n);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/// 消除
	glDeleteBuffers(1, &mVertexBufferObject);

#endif

	return CZRect();
}

/// 绘制两点之间的线.
///
///		将两绘制点(linePoints)之间的线离散成更小粒度的离散点(points)，
///		更新points,sizes,alphas,angles等向量，
///		这里利用到了画刷的参数。
///
///		/param lastLocation - 轨迹最后离散点的位置
///		/param location		- 当前绘制点的位置
///		/param randomizer	- 随机器
///		/note	我用系统自带的随机参数暂时替代了randomizer
///				利用画笔参数生成部分的算法没看懂
void CZPath::paintBetweenPoints(const CZ3DPoint &lastLocation, const CZ3DPoint &location/*,randomizer randomizer*/)
{
	float           pA = lastLocation.z;
	float           pB = location.z;
	float           pDelta = (pB - pA);

	CZ2DPoint		lastLocation2D	= CZ2DPoint(lastLocation.x, lastLocation.y);
	CZ2DPoint		location2D		= CZ2DPoint(location.x, location.y);
	float           distance = lastLocation2D.distanceTo2DPoint(location2D);
	CZ2DPoint       vector = location2D -lastLocation2D;
	CZ2DPoint       unitVector = CZ2DPoint(1.0f, 1.0f);
	float           vectorAngle = atan2(vector.y, vector.x);
	float           pressure = pA;
	float           weight = this->scale * (this->limitBrushSize ? 50 : this->brush->weight.value);

	if (distance > 0.0f) 
	{
		unitVector = vector / distance;
	}

	CZ2DPoint start = lastLocation2D + (unitVector*this->remainder);

	float step = 0.0f, pressureStep = 0.0f;
	float f;
	/// step linearly from last to current, pasting brush image
	for (f = this->remainder; f <= distance; f += step, pressure += pressureStep) {

		int sign = this->brush->weightDynamics.value >= 0 ? 0:1;
		float p = sign ? pressure : (1.0f - pressure);
		float brushSize = Max(1, weight - fabs(brush->weightDynamics.value) * p * weight);

		float rotationalScatter = /*[randomizer nextFloat]*/rand()*1.0/RAND_MAX * brush->rotationalScatter.value * M_PI * 2;
		float angleOffset = brush->angle.value * (M_PI / 180.0f);

		float positionalScatter = /*[randomizer nextFloatMin:-1.0f max:1.0f]*/rand()*2.0/RAND_MAX - 1.0f;
		positionalScatter *= brush->positionalScatter.value;
		CZ2DPoint orthog;
		orthog.x = unitVector.y;
		orthog.y = -unitVector.x;
		orthog = orthog * weight / 2.0f * positionalScatter;
		CZ2DPoint pos = start + orthog;

		sign = sign = brush->intensityDynamics.value >= 0 ? 0:1;
		p = sign ? pressure : (1.0f - pressure);
		float alpha = Max(0.01, brush->intensity.value - fabs(brush->intensityDynamics.value) * p * brush->intensity.value);

		this->points.push_back(pos);
		this->sizes.push_back(brushSize);
		this->angles.push_back(vectorAngle * brush->angleDynamics.value + rotationalScatter + angleOffset);
		this->alphas.push_back(alpha);

		step = Max(1.0, brush->spacing.value * brushSize);
		start = start + (unitVector * step);
		pressureStep = pDelta / (distance / step);
	}

	// how much extra spacing should we add when we paint the next time?
	// this keeps the spacing uniform across move events
	this->remainder = (f - distance);
}

/// 将结点打散成绘制点
/// 
///		两个结点（nodes）形成一根三次贝塞尔曲线，再将曲线打散成若干个绘制点（points）
/// 
///		/param points		- 离散后得到的绘制点容器
///		/return				- 离散后得到的绘制点数目
int CZPath::flattenedPoints(std::vector<CZ3DPoint> & linePoints)
{
	int numNodes = this->nodes.size();
	if (numNodes == 1)
	{
		CZBezierNode lonelyNode = nodes.back();
		linePoints.push_back(lonelyNode.anchorPoint);
		return 1;
	}

	int numSegs = this->closed ? numNodes : numNodes - 1;
	
	CZBezierSegment   *segment = NULL;
	for (int i = 0; i < numSegs; i++) 
	{
		CZBezierNode a = this->nodes[i];
		CZBezierNode b = this->nodes[(i+1) % numNodes];

		segment = CZBezierSegment::segmentBetweenNodes(a,b);
		segment->flattenIntoArray(linePoints);
		delete segment;
	}

	return linePoints.size();
}