
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
 
CZPath::CZPath(std::vector<CZBezierNode> *nodes_ /* = NULL */)
{
	nodes.clear();
	if(nodes_ != NULL)
	{
		int n = nodes_->size();
		for(int i=0; i<n; i++) nodes.push_back((*nodes_)[i]);
	}

	ptrBrush = NULL;
	scale = 1.0f;
	remainder = 0.0f;
	limitBrushSize = false;
	boundsDirty = true;
}
CZPath::~CZPath()
{
	nodes.clear();
	points.clear();
	sizes.clear();
	angles.clear();
	alphas.clear();
}

/// 设置所有结点
void CZPath::setNodes(const std::vector<CZBezierNode> &nodes_)
{
	nodes.clear();
	invalidatePath();

	int n = nodes_.size();
	for(int i=0; i<n; i++) nodes.push_back(nodes_[i]);
}

/// 添加结点
void CZPath::addNode(CZBezierNode &node)
{
	nodes.push_back(node);
}

/// 获取首尾结点
CZBezierNode CZPath::firstNode()
{
	return nodes.front();
}
CZBezierNode CZPath::lastNode()
{
	return (closed ? nodes.front() : nodes.back());
}

/// 生成轨迹的绘制数据（返回所有数据的范围）
CZRect CZPath::getPaintData(CZRandom *randomizer_,unsigned int &dataNum, vertexData* &data)
{   
	this->points.clear();
	this->sizes.clear();
	this->alphas.clear();
	this->angles.clear();

	if (this->nodes.size() == 1) 
	{
		paintStamp(randomizer_);
	} 
	else 
	{
		std::vector<CZ3DPoint> linePoints;		/// <贝塞尔曲线的绘制点
		int numPoints = flattenNodes2Points(nodes,closed,linePoints);

		if(ptrBrush == NULL) 
		{
			std::cerr << "CZPath::paint - Brush is NULL\n";
			return CZRect();
		}

		for (int ix = 0; ix < numPoints - 1; ix++) 
		{
			this->paintBetweenPoints(linePoints[ix],linePoints[ix+1],randomizer_);
		}
	}

	return this->drawData(dataNum, data);

}

/// 设置闭合
void CZPath::setClosed(bool closed_)
{
	closed = closed_;
}

/// 设置笔刷
void CZPath::setBrush(CZBrush *brush_)
{
	ptrBrush = brush_;
}

/// 设置颜色
void CZPath::setColor(CZColor &color_)
{
	this->color = color_;
}

/// 获取随机数器（根据该轨迹的笔刷的生成器）
CZRandom *CZPath::getRandomizer()
{
	return ptrBrush->generator->getRandomizer();
}

/// 实现coding 接口
void CZPath::update(CZDecoder *decoder_, bool deep /* = false */)
{
	/*
	self.color = [decoder decodeObjectForKey:WDColorKey];
	NSString *brushID = [decoder decodeObjectForKey:WDBrushIDKey];
	self.brush = [[WDActiveState sharedInstance] brushWithID:brushID];
	self.scale = [decoder decodeFloatForKey:WDScaleKey defaultTo:1.f];

	NSString *cnodes = [decoder decodeStringForKey:WDNodesKey];
	if (!cnodes) {
		// handle legacy formats
		NSArray *nodeList = nil;
		nodeList = [decoder decodeArrayForKey:WDNodesKeyV1];
		if (nodeList) {
			self.nodes = [self decodeLegacyNodesA:nodeList v1:YES];
		} else {
			nodeList = [decoder decodeArrayForKey:WDNodesKeyV2];
			if (nodeList) {
				self.nodes = [self decodeLegacyNodesA:nodeList v1:NO];
			} else {
				nodeList = [decoder decodeArrayForKey:WDNodesKeyV3];
				self.nodes = [self decodeLegacyNodesB:nodeList];
			}
		}
	} else {
		NSData *binaryNodes = [NSData dataFromBase64String:cnodes];
		NSMutableArray *nodes = [NSMutableArray arrayWithCapacity:(binaryNodes.length / (9 * sizeof(CFSwappedFloat32)))];
		for (int i = 0; i < binaryNodes.length; i += 9 * sizeof(CFSwappedFloat32)) {
			CFSwappedFloat32 f;
			[binaryNodes getBytes:&f range:NSMakeRange(i + 0 * sizeof(f), sizeof(f))];
			float ix = CFConvertFloat32SwappedToHost(f);
			[binaryNodes getBytes:&f range:NSMakeRange(i + 1 * sizeof(f), sizeof(f))];
			float iy = CFConvertFloat32SwappedToHost(f);
			[binaryNodes getBytes:&f range:NSMakeRange(i + 2 * sizeof(f), sizeof(f))];
			float ip = CFConvertFloat32SwappedToHost(f);
			[binaryNodes getBytes:&f range:NSMakeRange(i + 3 * sizeof(f), sizeof(f))];
			float ax = CFConvertFloat32SwappedToHost(f);
			[binaryNodes getBytes:&f range:NSMakeRange(i + 4 * sizeof(f), sizeof(f))];
			float ay = CFConvertFloat32SwappedToHost(f);
			[binaryNodes getBytes:&f range:NSMakeRange(i + 5 * sizeof(f), sizeof(f))];
			float ap = CFConvertFloat32SwappedToHost(f);
			[binaryNodes getBytes:&f range:NSMakeRange(i + 6 * sizeof(f), sizeof(f))];
			float ox = CFConvertFloat32SwappedToHost(f);
			[binaryNodes getBytes:&f range:NSMakeRange(i + 7 * sizeof(f), sizeof(f))];
			float oy = CFConvertFloat32SwappedToHost(f);
			[binaryNodes getBytes:&f range:NSMakeRange(i + 8 * sizeof(f), sizeof(f))];
			float op = CFConvertFloat32SwappedToHost(f);
			WDBezierNode *node = [WDBezierNode bezierNodeWithInPoint:[WD3DPoint pointWithX:ix y:iy z:ip]
anchorPoint:[WD3DPoint pointWithX:ax y:ay z:ap]
outPoint:[WD3DPoint pointWithX:ox y:oy z:op]];
			[nodes addObject:node];
		}
		self.nodes = nodes;
	}

	boundsDirty_ = YES;
	*/
}
void CZPath::encode(CZCoder *coder_, bool deep /* = false */)
{
	/*
	[coder encodeObject:self.color forKey:WDColorKey deep:deep];
	[coder encodeString:self.brush.uuid forKey:WDBrushIDKey];
	if (self.scale != 1.f) {
		[coder encodeFloat:self.scale forKey:WDScaleKey];
	}

	NSMutableData *binaryNodes = [NSMutableData dataWithCapacity:self.nodes.count * 9 * sizeof(CFSwappedFloat32)];
	for (WDBezierNode *node in self.nodes) {
		CFSwappedFloat32 f;
		f = CFConvertFloat32HostToSwapped(node.inPoint.x);
		[binaryNodes appendBytes:&f length:sizeof(f)];
		f = CFConvertFloat32HostToSwapped(node.inPoint.y);
		[binaryNodes appendBytes:&f length:sizeof(f)];
		f = CFConvertFloat32HostToSwapped(node.inPressure);
		[binaryNodes appendBytes:&f length:sizeof(f)];
		f = CFConvertFloat32HostToSwapped(node.anchorPoint.x);
		[binaryNodes appendBytes:&f length:sizeof(f)];
		f = CFConvertFloat32HostToSwapped(node.anchorPoint.y);
		[binaryNodes appendBytes:&f length:sizeof(f)];
		f = CFConvertFloat32HostToSwapped(node.anchorPressure);
		[binaryNodes appendBytes:&f length:sizeof(f)];
		f = CFConvertFloat32HostToSwapped(node.outPoint.x);
		[binaryNodes appendBytes:&f length:sizeof(f)];
		f = CFConvertFloat32HostToSwapped(node.outPoint.y);
		[binaryNodes appendBytes:&f length:sizeof(f)];
		f = CFConvertFloat32HostToSwapped(node.outPressure);
		[binaryNodes appendBytes:&f length:sizeof(f)];
	}
	[coder encodeString:[[binaryNodes base64EncodedString] stringByReplacingOccurrencesOfString:@"\n" withString:@""]forKey:WDNodesKey];
	*/
}

/// 生成绘制数据
/// 
///		以最小粒度的离散点(points_)为中心，形成小矩形。并将此矩形数据返回。
///
CZRect CZPath::drawData(unsigned int &dataNum, vertexData* &data)
{
	int iPointSize = points.size();
	vertexData *vertexD = new vertexData[iPointSize * 4 + (iPointSize - 1) * 2];
	CZRect dataBounds = CZRect(0.0f,0.0f,0.0f,0.0f);

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
	
	dataNum = n;
	data = vertexD;

	return dataBounds;
}

/// 绘制一个stamp点
void CZPath::paintStamp(CZRandom *randomizer)
{
	float weight = scale * (limitBrushSize ? 50 : ptrBrush->weight.value);

	CZ2DPoint start(nodes[0].anchorPoint.x, nodes[0].anchorPoint.y);

	float brushSize = weight;
	float rotationalScatter = randomizer->nextFloat() * ptrBrush->rotationalScatter.value * M_PI * 2;
	float angleOffset = ptrBrush->angle.value * (M_PI / 180.0f);
	float alpha = Max(0.01, ptrBrush->intensity.value);

	points.push_back(start);
	sizes.push_back(brushSize);
	angles.push_back(rotationalScatter+angleOffset);
	alphas.push_back(alpha);
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
///		/note	利用画笔参数生成部分的算法没看懂
void CZPath::paintBetweenPoints(const CZ3DPoint &lastLocation, const CZ3DPoint &location, CZRandom *randomizer)
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
	float           weight = this->scale * (this->limitBrushSize ? 50 : this->ptrBrush->weight.value);

	if (distance > 0.0f) 
	{
		unitVector = vector / distance;
	}

	CZ2DPoint start = lastLocation2D + (unitVector*this->remainder);

	float step = 0.0f, pressureStep = 0.0f;
	float f;
	/// step linearly from last to current, pasting brush image
	for (f = this->remainder; f <= distance; f += step, pressure += pressureStep) {

		int sign = this->ptrBrush->weightDynamics.value >= 0 ? 0:1;
		float p = sign ? pressure : (1.0f - pressure);
		float brushSize = Max(1, weight - fabs(ptrBrush->weightDynamics.value) * p * weight);

		float rotationalScatter = randomizer->nextFloat() * ptrBrush->rotationalScatter.value * M_PI * 2;
		float angleOffset = ptrBrush->angle.value * (M_PI / 180.0f);

		float positionalScatter = randomizer->nextFloat(-1.0f,1.0f);
		positionalScatter *= ptrBrush->positionalScatter.value;
		CZ2DPoint orthog;
		orthog.x = unitVector.y;
		orthog.y = -unitVector.x;
		orthog = orthog * weight / 2.0f * positionalScatter;
		CZ2DPoint pos = start + orthog;

		sign = sign = ptrBrush->intensityDynamics.value >= 0 ? 0:1;
		p = sign ? pressure : (1.0f - pressure);
		float alpha = Max(0.01, ptrBrush->intensity.value - fabs(ptrBrush->intensityDynamics.value) * p * ptrBrush->intensity.value);

		this->points.push_back(pos);
		this->sizes.push_back(brushSize);
		this->angles.push_back(vectorAngle * ptrBrush->angleDynamics.value + rotationalScatter + angleOffset);
		this->alphas.push_back(alpha);

		step = Max(1.0, ptrBrush->spacing.value * brushSize);
		start = start + (unitVector * step);
		pressureStep = pDelta / (distance / step);
	}

	// how much extra spacing should we add when we paint the next time?
	// this keeps the spacing uniform across move events
	this->remainder = (f - distance);
}

/// 使轨迹无效化
void CZPath::invalidatePath()
{

	//CGPathRelease(pathRef_);
	//pathRef_ = NULL;

	boundsDirty = true;
}