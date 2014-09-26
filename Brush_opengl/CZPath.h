
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
#include "CZBezierSegment.h"
#include "CZShader.h"
#include "CZRandom.h"
#include "CZColor.h"
#include "CZCoding.h"
#include <vector>

/*

typedef enum {
    WDPathActionPaint,
    WDPathActionErase
} WDPathAction;

*/
class CZPath /*: public CZCoding*/
{
public:
	CZPath(std::vector<CZBezierNode> *nodes_ = NULL);
	~CZPath()
	{
		nodes.clear();
		points.clear();
		sizes.clear();
		angles.clear();
		alphas.clear();
	}

	/// 绘制轨迹
	CZRect paint(bool withBrush = true/*randomizer*/);
	/// 设置闭合
	void setClosed(bool closed_);
	/// 设置笔刷
	void setBrush(CZBrush *brush_);

private:
	/// 绘制数据（调用Util中的外部函数）
	/// 
	///		以最小粒度的离散点(points_)为中心，形成小矩形。并将此矩形数据通过Util中的外部函数调用图形接口绘制出来。
	///
	CZRect drawData();
	
	/// 直接绘制数据（调用Util中的外部函数）
	/// 
	///		通过Util中的外部函数调用图形接口，将轨迹数据不带纹理绘制地直接出来。
	///
	CZRect drawDataDirectly();

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
	void paintBetweenPoints(const CZ3DPoint &lastLocation, const CZ3DPoint &location/*, float remainder*/);
	
	/// 将结点打散成绘制点
	/// 
	///		两个结点（nodes）形成一根三次贝塞尔曲线，再将曲线打散成若干个绘制点（points）
	/// 
	///		/param points		- 离散后得到的绘制点容器
	///		/ret				- 离散后得到的绘制点数目
	int flattenedPoints(std::vector<CZ3DPoint> & linePoints);

public:
	std::vector<CZBezierNode>		nodes;				///< 贝塞尔曲线所有控制点
	bool							limitBrushSize;
	float							remainder;			///< 绘制轨迹最后离散点后多占用的线空间
	CZShader						*shader;			///< !目前做测试用，只是引用

private:
	CZBrush					*pBrush;					///< 只是引用，不负责建立和销毁
	bool					closed;
	bool					boundsDirty;				///< 用于标记是否需要重新计算包围区域

	//WDColor *color;
	//@property (nonatomic) WDPathAction action;
	float					scale;

	/// 最终绘制点的参数
	std::vector<CZ2DPoint>	points;
	std::vector<float>		sizes;
	std::vector<float>		angles;
	std::vector<float>		alphas;
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

