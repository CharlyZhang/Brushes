
///  \file CZSpiralGenerator.h
///  \brief This is the file declare the Class CZSpiralGenerator.
///
///		
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-19
///  \note

#include "CZSpiralGenerator.h"
#include "CZGeometry.h"
#include "CZUtil.h"
#include "CZAffineTransform.h"
#include <stdlib.h>				//for rand()
#include "CZStampRender.h"
#include <cmath>
#include "CZBezierNode.h"

using namespace std;

CZSpiralGenerator::CZSpiralGenerator()
{
	/// 创建属性
	//density.title = NSLocalizedString(@"Density", @"Density");
	density.value = 15;		///< ！先假设的
	density.minimumValue = 1;
	density.maximumValue = 20;
	density.conversionFactor = 1;
	//density.delegate = self;
	//(self.rawProperties)[@"density"] = density;
}

/// 绘制图案
void CZSpiralGenerator::renderStamp()
{
	float dim = baseDimension - 20;

	for (int i = 0; i < density.value; i++)
    {
		CZ2DPoint center(20 + /*[randomizer nextFloat]*/rand()*1.0/RAND_MAX * dim, 20 
			+ /*[randomizer nextFloat]*/rand()*1.0/RAND_MAX * dim);

		float radius = Min(center.x, baseDimension - center.x);
		radius = Min(radius, Min(center.y, baseDimension - center.y));
		radius -= 2;
		
		drawSpiral(center,radius);
	}
}

/// 绘制螺旋线
void CZSpiralGenerator::drawSpiral(const CZ2DPoint &center_, float radius_)
{
	int         segments = 15;
	float       decay = 80;
	float       b = 1.0f - (decay / 100.f);
	float       a = (float) radius_ / pow(M_E, b * segments * M_PI_4);

	//CGMutablePathRef pathRef = CGPathCreateMutable();
	vector<CZBezierNode> nodes;
	CZ2DPoint lastOut(0,0);

	CZAffineTransform transform = CZAffineTransform::makeIdentity();
	transform.translate(center_.x, center_.y);
	transform.rotate(/*[random nextFloat]*/rand()*1.0/RAND_MAX * M_PI * 2);

	for (int segment = 0; segment <= segments; segment++)
	{
		float t = segment * M_PI_4;
		float f = a * pow((float)M_E, b * t);
		float x = f * cos(t);
		float y = f * sin(t);

		CZ2DPoint P3(x, y);

		// derivative
		float t0 = t - M_PI_4;
		float deltaT = (t - t0) / 3;
		
		float xPrime = a*b*pow((float)M_E,b*t)*cos(t) - a*pow((float)M_E,b*t)*sin(t);
		float yPrime = a*pow((float)M_E,b*t)*cos(t) + a*b*pow((float)M_E,b*t)*sin(t);

		CZ2DPoint P2 = P3 - (CZ2DPoint(xPrime, yPrime) * deltaT);
		CZ2DPoint P1 = P3 + (CZ2DPoint(xPrime, yPrime) * deltaT);

		/*
		if (CGPathIsEmpty(pathRef)) {
			CGPathMoveToPoint(pathRef, &transform, P3.x, P3.y);
		} else {
			CGPathAddCurveToPoint(pathRef, &transform, lastOut.x, lastOut.y, P2.x, P2.y, P3.x, P3.y);
		}
		*/
		P1 = transform.applyTo2DPoint(P1);
		P2 = transform.applyTo2DPoint(P2);
		P3 = transform.applyTo2DPoint(P3);
		CZBezierNode node(
			CZ3DPoint(P2.x,P2.y,0),
			CZ3DPoint(P3.x,P3.y,0),
			CZ3DPoint(P1.x,P1.y,0));
		nodes.push_back(node);


		lastOut = P1;
	}

	vector<CZ3DPoint> points;
	flattenNodes2Points(nodes,false,points);
	CZStampRender::getInstance()->drawSpiralData(points);
	/*
	CGContextAddPath(ctx, pathRef);
	CGContextSetGrayStrokeColor(ctx, [random nextFloat], 1.0f);
	CGContextSetLineWidth(ctx, [random nextFloat] * 9 + 1);
	CGContextSetLineCap(ctx, kCGLineCapRound);
	CGContextStrokePath(ctx);
	CGPathRelease(pathRef);
	*/
}