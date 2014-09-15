
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
#include <cmath>
#include "gl/glew.h"
#include "glut/glut.h"

/// 绘制路径
CZRect CZPath::paint(/*randomizer*/)
{    
	this->points_.clear();
	this->sizes_.clear();
	this->alphas_.clear();
	this->angles_.clear();

	if (this->nodes_.size() == 1) 
	{
		//[self paintStamp:randomizer];
	} 
	else 
	{
		std::vector<CZ3DPoint> points;
		int numPoints = this->flattenedPoints(points);

		for (int ix = 0; ix < numPoints - 1; ix++) 
		{
			this->paintBetweenPoints(points[ix],points[ix+1]);
		}
	}

	return this->drawData();
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

	int iPointSize = points_.size();
	vertexData *vertexD = new vertexData[iPointSize * 4 + (iPointSize - 1) * 2];
	CZRect dataBounds = CZRect(0.0f,0.0f,0.0f,0.0f);

	int n = 0;
	for (int i = 0; i < iPointSize; i++) 
	{
		CZ2DPoint result = points_[i];
		float angle = angles_[i];
		float size = sizes_[i] / 2;
		float alpha = alphas_[i];

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

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), &vertexD[0].x);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(vertexData), &vertexD[0].s);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(vertexData), &vertexD[0].a);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, n);
	delete [] vertexD;
	//WDCheckGLError();

	return dataBounds;
}

/// 绘制两点之间的线.
///
///		将两绘制点(points)之间的线离散成更小粒度的离散点(points_)
///
///		/param lastLocation - 路径最后离散点的位置
///		/param location		- 当前绘制点的位置
///		/param randomizer	- 随机器
///		/note	我用系统自带的随机参数暂时替代了randomizer
///				利用画笔参数生成部分的算法没看懂
void CZPath::paintBetweenPoints(CZ3DPoint &lastLocation, CZ3DPoint &location/*,randomizer randomizer*/)
{
	float           pA = lastLocation.z;
	float           pB = location.z;
	float           pDelta = (pB - pA);

	CZ2DPoint		lastLoaction2D	= CZ2DPoint(lastLocation.x, lastLocation.y);
	CZ2DPoint		loaction2D		= CZ2DPoint(location.x, location.y);
	float           distance = lastLoaction2D.distanceTo2DPoint(loaction2D);
	CZ2DPoint       vector = location2D -lastLocation2D;
	CZ2DPoint       unitVector = CZ2DPoint(1.0f, 1.0f);
	float           vectorAngle = atan2(vector.y, vector.x);
	float           pressure = pA;
	float           weight = this->scale * (this->limitBrushSize ? 50 : this->brush->weight.value);

	if (distance > 0.0f) 
	{
		unitVector = vector / distance;
	}

	CZ2DPoint start = lastLocation2D + (unitVector*this->remainder_);

	float step = pressureStep = 0.0f;
	/// step linearly from last to current, pasting brush image
	for (float f = this->remainder_; f <= distance; f += step, pressure += pressureStep) {

		int sign = this->brush->weightDynamics.value >= 0 ? 0:1;
		float p = sign ? pressure : (1.0f - pressure);
		float p = pressure;
		float brushSize = Max(1, weight - fabs(this->brush.weightDynamics.value) * p * weight);

		float rotationalScatter = /*[randomizer nextFloat]*/rand() * this->brush.rotationalScatter.value * M_PI * 2;
		float angleOffset = this->brush.angle.value * (M_PI / 180.0f);

		float positionalScatter = /*[randomizer nextFloatMin:-1.0f max:1.0f]*/rand()*2 - 1.0f;
		positionalScatter *= this->brush.positionalScatter.value;
		CZ2DPoint orthog;
		orthog.x = unitVector.y;
		orthog.y = -unitVector.x;
		orthog = orthog * weight / 2.0f * positionalScatter;
		CZ2DPoint pos = start + orthog;

		sign = sign = this->brush->intensityDynamics.value >= 0 ? 0:1;
		p = sign ? pressure : (1.0f - pressure);
		float alpha = Max(0.01, this->brush.intensity.value - fabs(this->brush.intensityDynamics.value) * p * this->brush.intensity.value);

		this->points_.push_back(pos);
		this->sizes_.push_back(brushSize);
		this->angles_.push_back(vectorAngle * this->brush.angleDynamics.value + rotationalScatter + angleOffset);
		this->alphas_.push_back(alpha);

		step = Max(1.0, brush.spacing * brushSize);
		start = start + (unitVector * step);
		pressureStep = pDelta / (distance / step);
	}

	// how much extra spacing should we add when we paint the next time?
	// this keeps the spacing uniform across move events
	this->remainder_ = (f - distance);
}

/// 将结点打散成绘制点
/// 
///		两个结点（nodes）形成一根三次贝塞尔曲线，再将曲线打散成若干个绘制点（points）
/// 
///		/param points		- 离散后得到的绘制点容器
///		/ret				- 离散后得到的绘制点数目
int CZPath::flattenedPoints(std::vector<CZ3DPoint> & points)
{
	int numNodes = this->nodes_.size();
	if (numNodes == 1)
	{
		//WDBezierNode *lonelyNode = [nodes_ lastObject];
		//return @[lonelyNode.anchorPoint];
	}

	int numSegs = this->closed_ ? numNodes : numNodes - 1;
	WDBezierSegment     *segment = nil;

	for (int i = 0; i < numSegs; i++) 
	{
		WDBezierNode *a = this->nodes_[i];
		WDBezierNode *b = this->nodes_[(i+1) % numNodes];

		segment = [WDBezierSegment segmentWithStart:a end:b];
		segment->flattenIntoArray(points);
	}

	return points_.size();
}