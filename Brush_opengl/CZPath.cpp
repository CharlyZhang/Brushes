
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
#include <cmath>				
#include <iostream>

#define M_PI       3.14159265358979323846
 
/// ���ƹ켣
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
		std::vector<CZ3DPoint> linePoints;		/// <���������ߵĻ��Ƶ�
		int numPoints = this->flattenedPoints(linePoints);

		if(withBrush)
		{
			if(pBrush == NULL) 
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

/// ���ñպ�
void CZPath::setClosed(bool closed_)
{
	closed = closed_;
}

/// ���ñ�ˢ
void CZPath::setBrush(CZBrush *brush_)
{
	pBrush = brush_;
}

/// �������ݣ�����Util�е��ⲿ������
/// 
///		����С���ȵ���ɢ��(points_)Ϊ���ģ��γ�С���Ρ������˾�������ͨ��Util�е��ⲿ��������ͼ�νӿڻ��Ƴ�����
///
CZRect CZPath::drawData()
{
	int iPointSize = points.size();
	vertexData *vertexD = new vertexData[iPointSize * 4 + (iPointSize - 1) * 2];
	CZRect dataBounds = CZRect(0.0f,0.0f,0.0f,0.0f);

	//////////////////////////////////////////////////////////////////////////
#if STAMP_PATH		///< Render One Rect Use Brush	
	delete [] vertexD;
	points.clear();
	angles.clear();
	float s = sizes.front();
	sizes.clear();
	alphas.clear();

	points.push_back(CZ2DPoint(300,300));
	angles.push_back(0);
	sizes.push_back(100);
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
	
	/// �����ⲿ������������
	drawPathData(vertexD,n,shader);

	delete [] vertexD;
	//WDCheckGLError();

	return dataBounds;
}

/// ֱ�ӻ������ݣ�����Util�е��ⲿ������
/// 
///		ͨ��Util�е��ⲿ��������ͼ�νӿڣ����켣���ݲ���������Ƶ�ֱ�ӳ�����
///
CZRect CZPath::drawDataDirectly()
{
	
	drawPathDataDirectly(this->points);

	return CZRect();
}

/// ��������֮�����.
///
///		�������Ƶ�(linePoints)֮�������ɢ�ɸ�С���ȵ���ɢ��(points)��
///		����points,sizes,alphas,angles��������
///		�������õ��˻�ˢ�Ĳ�����
///
///		/param lastLocation - �켣�����ɢ���λ��
///		/param location		- ��ǰ���Ƶ��λ��
///		/param randomizer	- �����
///		/note	����ϵͳ�Դ������������ʱ�����randomizer
///				���û��ʲ������ɲ��ֵ��㷨û����
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
	float           weight = this->scale * (this->limitBrushSize ? 50 : this->pBrush->weight.value);

	if (distance > 0.0f) 
	{
		unitVector = vector / distance;
	}

	CZ2DPoint start = lastLocation2D + (unitVector*this->remainder);

	float step = 0.0f, pressureStep = 0.0f;
	float f;
	/// step linearly from last to current, pasting brush image
	for (f = this->remainder; f <= distance; f += step, pressure += pressureStep) {

		int sign = this->pBrush->weightDynamics.value >= 0 ? 0:1;
		float p = sign ? pressure : (1.0f - pressure);
		float brushSize = Max(1, weight - fabs(pBrush->weightDynamics.value) * p * weight);

		float rotationalScatter = /*[randomizer nextFloat]*/rand()*1.0/RAND_MAX * pBrush->rotationalScatter.value * M_PI * 2;
		float angleOffset = pBrush->angle.value * (M_PI / 180.0f);

		float positionalScatter = /*[randomizer nextFloatMin:-1.0f max:1.0f]*/rand()*2.0/RAND_MAX - 1.0f;
		positionalScatter *= pBrush->positionalScatter.value;
		CZ2DPoint orthog;
		orthog.x = unitVector.y;
		orthog.y = -unitVector.x;
		orthog = orthog * weight / 2.0f * positionalScatter;
		CZ2DPoint pos = start + orthog;

		sign = sign = pBrush->intensityDynamics.value >= 0 ? 0:1;
		p = sign ? pressure : (1.0f - pressure);
		float alpha = Max(0.01, pBrush->intensity.value - fabs(pBrush->intensityDynamics.value) * p * pBrush->intensity.value);

		this->points.push_back(pos);
		this->sizes.push_back(brushSize);
		this->angles.push_back(vectorAngle * pBrush->angleDynamics.value + rotationalScatter + angleOffset);
		this->alphas.push_back(alpha);

		step = Max(1.0, pBrush->spacing.value * brushSize);
		start = start + (unitVector * step);
		pressureStep = pDelta / (distance / step);
	}

	// how much extra spacing should we add when we paint the next time?
	// this keeps the spacing uniform across move events
	this->remainder = (f - distance);
}

/// ������ɢ�ɻ��Ƶ�
/// 
///		������㣨nodes���γ�һ�����α��������ߣ��ٽ����ߴ�ɢ�����ɸ����Ƶ㣨points��
/// 
///		/param points		- ��ɢ��õ��Ļ��Ƶ�����
///		/return				- ��ɢ��õ��Ļ��Ƶ���Ŀ
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