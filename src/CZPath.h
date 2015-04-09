
///  \file CPath.h
///  \brief This is the file declare the Class CPath.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-11
///
///  \version	1.1.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-11
///  \note		将绘制部分从CZPath中剔除，其只负责整理数据。同时修改相应函数名。

#ifndef _CZPATH_H_
#define _CZPATH_H_

#include "CZCoding.h"
#include "CZColor.h"
#include "basic/CZRect.h"
#include "basic/CZ2DPoint.h"
#include <vector>

class CZ3DPoint;
class CZBezierNode;
class CZRandom;
class CZBrush;
class CZRender;
typedef enum 
{
    CZPathActionPaint,
    CZPathActionErase
} CZPathAction;


class CZPath : public CZCoding
{
public:
	CZPath(std::vector<CZBezierNode> *nodes_ = NULL);
	~CZPath();
	/// 初始化
	void initital();
	/// 设置所有结点
	void setNodes(const std::vector<CZBezierNode> &nodes_);
	/// 添加结点
	void addNode(CZBezierNode &node);
	/// 获取首尾结点
	CZBezierNode firstNode();
	CZBezierNode lastNode();
	/// 绘制轨迹（返回所有数据的范围）
	CZRect paintPath(CZRender *render,CZRandom *randomizer_);
	/// 设置闭合
	void setClosed(bool closed_);
	/// 设置笔刷
	void setBrush(CZBrush *brush_);
	/// 设置颜色
	void setColor(CZColor &color_);
	/// 获取随机数器（根据该轨迹的笔刷的生成器）
	CZRandom *getRandomizer();
	/// 实现coding 接口
	void update(CZDecoder *decoder_, bool deep = false);
	void encode(CZCoder *coder_, bool deep = false);
private:
	/// 绘制数据
	/// 
	///		以最小粒度的离散点(points_)为中心，形成小矩形，委托render绘制，并将此矩形数据返回。
	///
	CZRect drawData();
	
	/// 绘制一个stamp点
	void paintStamp(CZRandom *randomizer);

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
	void paintBetweenPoints(const CZ3DPoint &lastLocation, const CZ3DPoint &location, CZRandom *randomizer);
	
	/// 使轨迹无效化
	void invalidatePath();

public:
	bool							limitBrushSize;
	float							remainder;			///< 绘制轨迹最后离散点后多占用的线空间
	CZBrush							*ptrBrush;			///< 只是引用，不负责建立和销毁
	CZPathAction					action;				///< 标记轨迹的动作是绘制还是擦除
	CZColor							color;

private:
	bool					closed;
	bool					boundsDirty;				///< 用于标记是否需要重新计算包围区域
	CZRect					bounds;						///< 轨迹的范围
	float					scale;

	std::vector<CZBezierNode>		nodes;				///< 贝塞尔曲线所有控制点
	/// 最终绘制点的参数
	std::vector<CZ2DPoint>	points;
	std::vector<float>		sizes;
	std::vector<float>		angles;
	std::vector<float>		alphas;

	CZRender				*ptrRender;					///< 用于内部信息传递，指向绘制器						
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

