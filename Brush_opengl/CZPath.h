
///  \file CPath.h
///  \brief This is the file declare the Class CPath.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-11
///  \note

#ifndef _CZPATH_H_
#define _CZPATH_H_

#include "CZGeometry.h"
#include "CZ3DPoint.h"
#include "CZBrush.h"
#include <vector>
//#include "WDBezierSegment.h"
//#include "WDCoding.h"
/*
@class WDBezierNode;
@class WDBrush;
@class WDColor;
@class WDRandom;

typedef enum {
    WDPathActionPaint,
    WDPathActionErase
} WDPathAction;

*/
class CZPath
{
private:
	std::vector<float>		nodes_;
	bool					closed_;
	CZBrush					*brush;
	//WDColor *color;
	//@property (nonatomic, assign) float remainder;
	//@property (nonatomic) WDPathAction action;
	float					scale;
	bool					limitBrushSize;


	// rendering assistance
	float					remainder_;			///< 绘制路径最后离散点后多占用的线空间
	std::vector<CZ2DPoint>	points_;
	std::vector<float>		sizes_;
	std::vector<float>		angles_;
	std::vector<float>		alphas_;

public:
	CZPath()
	{
		this->brush = NULL;
	}
	~CZPath()
	{
		if(this->brush)
		{
			delete this->brush;
			this->brush = NULL;
		}
	}

	/// 绘制路径
	CZRect paint(/*randomizer*/);

private:
	/// 绘制数据（利用OpenGL等图形接口）
	/// 
	///		以最小粒度的离散点(points_)为中心，形成小矩形。并将此矩形数据通过图形接口绘制出来。
	///
	CZRect drawData();
	/// 绘制两点之间的线.
	///
	///		将两绘制点(points)之间的线离散成更小粒度的离散点(points_)
	///
	///		/param lastLocation - 路径最后离散点的位置
	///		/param location		- 当前绘制点的位置
	///		/param randomizer	- 随机器
	void paintBetweenPoints(CZ3DPoint &lastLocation, CZ3DPoint &location/*, float remainder*/);
	/// 将结点打散成绘制点
	/// 
	///		两个结点（nodes）形成一根三次贝塞尔曲线，再将曲线打散成若干个绘制点（points）
	/// 
	///		/param points		- 离散后得到的绘制点容器
	///		/ret				- 离散后得到的绘制点数目
	int flattenedPoints(std::vector<CZ3DPoint> & points);
};

/*
    NSMutableArray      *nodes_;
    BOOL                closed_;
    CGMutablePathRef    pathRef_;
    CGRect              bounds_;
    BOOL                boundsDirty_;
    

@property (nonatomic, assign) BOOL closed;
@property (nonatomic, strong) NSMutableArray *nodes;
@property (nonatomic, strong) WDBrush *brush;
@property (nonatomic) WDColor *color;
@property (nonatomic, assign) float remainder;
@property (nonatomic) WDPathAction action;
@property (nonatomic, assign) float scale;

@property (nonatomic, assign) BOOL limitBrushSize;

+ (WDPath *) pathWithRect:(CGRect)rect;
+ (WDPath *) pathWithOvalInRect:(CGRect)rect;
+ (WDPath *) pathWithStart:(CGPoint)start end:(CGPoint)end;

- (id) initWithRect:(CGRect)rect;
- (id) initWithOvalInRect:(CGRect)rect;
- (id) initWithStart:(CGPoint)start end:(CGPoint)end;
- (id) initWithNode:(WDBezierNode *)node;

- (void) invalidatePath;

- (void) addNode:(WDBezierNode *)node;
- (void) addAnchors;

- (WDBezierNode *) firstNode;
- (WDBezierNode *) lastNode;

- (CGRect) controlBounds;
- (void) computeBounds;

- (void) setClosedQuiet:(BOOL)closed;

- (CGPathRef) pathRef;

- (NSArray *) flattenedPoints;
- (void) flatten;

- (WDRandom *) newRandomizer;
- (CGRect) paint:(WDRandom *)randomizer;

*/
#endif

