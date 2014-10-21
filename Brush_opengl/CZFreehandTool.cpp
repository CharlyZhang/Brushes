
///  \file CZFreehandTool.cpp
///  \brief This is the file implements the Class CZFreehandTool.
///
///		The CZFreehandTool is designed for input of drawing. 
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-28
///  \note

#include "CZFreehandTool.h"
#include "CZSpiralGenerator.h"		///< for moveEnd() Brush

#include "gl/glew.h"				///< for paintPath GL render

#define kMaxError                   10.0f
#define kSpeedFactor                3
#define kBezierInterpolationSteps   5

/// !������
#define SIZE	600

CZFreehandTool::CZFreehandTool(bool supportPressure /* = false */)
{
	realPressure = supportPressure;
	scale = 1.0f;
	lastRemainder = 0.0f;

	///�� ������
	{
		// configure some default GL state
		glDisable(GL_DITHER);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		/// ���û�ˢ������shader��������
		ptrBrush = CZActiveState::getInstance()->brush;

		brushShader = new CZShader("brush.vert","brush.frag");

		/// ��ʼ������
		texture = new CZTexture(SIZE,SIZE);
		render.configTexture(texture);

	}
}

/// ��ʼ�ƶ�
void CZFreehandTool::moveBegin(CZ2DPoint &p_, float pressure_ /* = 0.0f */)
{
	CZTool::moveBegin(p_);

	firstEver = true;

	strokeBounds = CZRect::zeroRect();
	accumulatedStrokePoints.clear();

	CZ2DPoint &location = p_;

	// capture first point
	lastLocation = location;
	float pressure = 1.0f;

#if 0
	/// 
	// see if we've got real pressure
	realPressure = NO;
	if ([recognizer isKindOfClass:[WDPanGestureRecognizer class]]) {
		UITouch *touch = [recognizer.touches anyObject];
		pressure = [[WDStylusManager sharedStylusManager] pressureForTouch:touch realPressue:&realPressure];
	}
#endif

	if (realPressure) pressure = pressure_;

	CZBezierNode node(location,pressure);
	pointsToFit[0] = node;
	pointsIndex = 1;

	clearBuffer = true;
}

/// �����ƶ�
///		/param p_				  - ��ǰ�ƶ��ĵ�λ��
///		/param pressureOrVelocity - ���豸֧��ѹ��ֵʱ��Ϊѹ��ֵ������Ϊ�ƶ��ٶ�ֵ
void CZFreehandTool::moving(CZ2DPoint &p_, float pressureOrSpeed)
{
	CZTool::moving(p_);

	CZ2DPoint &location = p_;
	float       distanceMoved = location.distanceTo2DPoint(lastLocation);

	if (distanceMoved < 3.0 / scale) 
	{
		// haven't moved far enough
		return;
	}

	float pressure = 1.0f;

	if(!realPressure)
	{
		// account for view scale
		//pressureOrSpeed /= scale;

		/// convert speed into "pressure"
		pressure = sineCurve(1.0f - Min(kSpeedFactor, pressureOrSpeed) / kSpeedFactor);
		pressure = 1.0f - pressure;
	}
	else
		pressure = pressureOrSpeed;

	if (firstEver)
	{
		pointsToFit[0].inPoint.z = pressure;
		pointsToFit[0].anchorPoint.z = pressure;
		pointsToFit[0].outPoint.z = pressure;
		firstEver = false;
#if CZ_DEBUG
		std::cout << "0 - " << pressure << std::endl;
#endif
	} 
	else if (pointsIndex != 0)
	{
#if CZ_DEBUG
		std::cout << pointsIndex-1 << " - " << pressure << std::endl;
#endif
		// average out the pressures
		pressure = (pressure + pointsToFit[pointsIndex - 1].anchorPoint.z) / 2;
#if CZ_DEBUG
		std::cout << pointsIndex-1 << " - " << pressure << std::endl;
#endif
	}

	pointsToFit[pointsIndex++] = CZBezierNode(location,pressure);

	// special case: otherwise the 2nd overall point never gets averaged
	if (pointsIndex == 3)
	{
		// since we just incremented pointsIndex (it was really just 2)
		averagePointsBetween(1,2);     
#if CZ_DEBUG
		std::cout << "averagePointsBetween(1,2)" << std::endl;
		for(int i=0; i<pointsIndex; i++)
			std::cout << pointsToFit[i].anchorPoint << std::endl;
#endif
	}

	if (pointsIndex == 5) 
	{
		paintFittedPoints();
	}

	// save data for the next iteration
	lastLocation = location;
}

/// �ƶ�����
void CZFreehandTool::moveEnd(CZ2DPoint &p_)
{
	//CZColor     color;// = [WDActiveState sharedInstance].paintColor;
	//CZBrush     brush(new CZSpiralGenerator);// = [WDActiveState sharedInstance].brush;
	//CZCanvas    canvas;// = (WDCanvas *) recognizer.view;
	CZSize size;
	CZPainting  painting(size);// = canvas.painting;

	CZ2DPoint   &location = p_;
	
	if (!moved) 
	{ // draw a single stamp
		CZBezierNode node(location,1.0);
		
		CZPath path;
		path.addNode(node);
		accumulatedStrokePoints.push_back(node);

		paintPath(path);
	} 
	else
	{
		paintFittedPoints();
	}

	/*
	if (CGRectIntersectsRect(strokeBounds_, painting.bounds)) {
		if (accumulatedStrokePoints_.count > 0) {
			WDPath *accumulatedPath = [[WDPath alloc] init];
			accumulatedPath.nodes = accumulatedStrokePoints_;
			accumulatedPath.color = color;
			accumulatedPath.brush = [brush copy];
			changeDocument(painting, [WDAddPath addPath:accumulatedPath erase:eraseMode layer:painting.activeLayer sourcePainting:painting]);
		}
	}

	if (CGRectIntersectsRect(strokeBounds_, painting.bounds)) {
		[painting.activeLayer commitStroke:strokeBounds_ color:color erase:eraseMode undoable:YES];
	}

	painting.activePath = NULL;
	*/
	CZTool::moveEnd(p_);
}

/// ����ʱ��������start_��end_�ĵ����ƽ������
///		
///		���崦����������ǰ��ֱ���ǰ�����������߳�l1��l2,�ٽ�l1��l2���е����߳�l3,
///					ȡl3���е�Ϊ��ǰ���ƽ�������
void CZFreehandTool::averagePointsBetween(int start_, int end_)
{
	for (int i = start_; i < end_; i++) 
	{
		CZ3DPoint current = pointsToFit[i].anchorPoint * 0.5;
		CZ3DPoint prev =	pointsToFit[i-1].anchorPoint * 0.25;
		CZ3DPoint next =	pointsToFit[i+1].anchorPoint * 0.25;

		pointsToFit[i].anchorPoint = current + prev + next ;
	}
}

/// ��������ĵ�
///
///		ÿ�ν��ٽ��ļ�������о������ټ�����ڶ����㵽�����ڶ������Ŀ��Ƶ㣻
///		����ǰ3�����������ߣ����л��ƣ�������������������һ�ֻ��ƣ��Ա�ƽ�����ɡ�
void CZFreehandTool::paintFittedPoints()
{
	bool    touchEnding = (pointsIndex != 5) ? true : false;	///< �жϻ����Ƿ����
	int     loopBound = touchEnding ? pointsIndex - 1 : 4;		///< �߶���Ŀ
	int     drawBound = touchEnding ? pointsIndex - 1 : 2;		///< ���λ������ĵ�ı��

	averagePointsBetween(2, loopBound);

#if CZ_DEBUG
	std::cout << "averagePointsBetween(2,loopBound)" << loopBound << std::endl;
	for(int i=0; i<pointsIndex; i++)
		std::cout << pointsToFit[i].anchorPoint << std::endl;
#endif

	/// �������β����������ģ�������Ƶ�ͳ�����Ƶ㡣
	for (int i = 1; i < loopBound; i++) 
	{
		CZ3DPoint current = pointsToFit[i].anchorPoint;
		CZ3DPoint prev =	pointsToFit[i-1].anchorPoint;
		CZ3DPoint next =	pointsToFit[i+1].anchorPoint;

		CZ3DPoint delta = next - prev;
		delta.normalize();

		float inMagnitude = prev.distanceTo3DPoint(current) / 3.0f;
		float outMagnitude = next.distanceTo3DPoint(current) / 3.0f;

		CZ3DPoint in = current - ( delta * inMagnitude);
		CZ3DPoint out = current + (delta * outMagnitude);

		pointsToFit[i].inPoint = in;
		pointsToFit[i].outPoint = out;
	}

	/// ����һ���켣path
	CZPath path;
	path.limitBrushSize = true;

	for (int i = 0; i <= drawBound; i++) 
	{
		path.addNode(pointsToFit[i]);

		if (i == 0 && accumulatedStrokePoints.size())
		{	/// ��ȥ���ֻ������Ľ��
			accumulatedStrokePoints.pop_back(); 
		}
		
		accumulatedStrokePoints.push_back(pointsToFit[i]);
	}

	paintPath(path);

	/// ����û�н�����������û�д������ǰ��
	if (!touchEnding) 
	{
		for (int i = 0; i < 3; i++) 
		{
			pointsToFit[i] = pointsToFit[i+2];
		}
		pointsIndex = 3;
	}
}

void CZFreehandTool::paintPath(CZPath &path) 
{
#if CZ_DEBUG
	int n = path.nodes.size();
	std::cout << "path node number " << n << std::endl;
	for(int i=0; i<n; i++)
		std::cout << path.nodes[i].anchorPoint << std::endl;
#endif

	path.ptrBrush = this->ptrBrush;//CZActiveState::getInstance()->brush;
	//path.color = CZColor[WDActiveState sharedInstance].paintColor;
	//path.action = eraseMode ? WDPathActionErase : WDPathActionPaint;

	CZRandom *randomizer = NULL;

	if (clearBuffer)
	{
		// depends on the path's brush
		randomizer = path.getRandomizer();
		lastRemainder = 0.f;
	}

	path.remainder = lastRemainder;

	CZRect pathBounds;// = [canvas.painting paintStroke:path randomizer:randomizer clear:clearBuffer];
	
	///! ������
	{
		/// ����ͶӰ����
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, (GLfloat) SIZE, 0.0f, 
			(GLfloat) SIZE, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		/// ���ù켣����
		path.setClosed(false);
//		path.ptrShader = brushShader;		///< !û�б�Ҫ


		/// ���û�����
		render.ptrShader = brushShader;

		/// ������
		CZImage *stamp = ptrBrush->generator->getStamp();
		CZTexture *stampTex = CZTexture::produceFromImage(stamp);
		glBindTexture(GL_TEXTURE_2D,stampTex->id);
		
		render.begin();

		/// ���ƹ켣
		path.paint(&render,path.getRandomizer());

		render.end();

		delete stampTex;
	}

	strokeBounds.unionWith(pathBounds);
	lastRemainder = path.remainder;

	//[canvas drawViewInRect:pathBounds];

	clearBuffer = false;
}