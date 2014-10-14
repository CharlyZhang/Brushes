
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
#include "CZPathRender.h"
#include "CZRandom.h"
#include "CZColor.h"
#include "CZCoding.h"
#include <vector>



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
	/// �������н��
	void setNodes(const std::vector<CZBezierNode> &nodes_);
	/// ��ӽ��
	void addNode(CZBezierNode &node);
	/// ��ȡ��β���
	CZBezierNode firstNode();
	CZBezierNode lastNode();
	/// ���ƹ켣
	CZRect paint(CZPathRender *render_, CZRandom *randomizer_);
	/// ���ñպ�
	void setClosed(bool closed_);
	/// ���ñ�ˢ
	void setBrush(CZBrush *brush_);
	/// ������ɫ
	void setColor(CZColor &color_);
	/// ��ȡ������������ݸù켣�ı�ˢ����������
	CZRandom *getRandomizer();
	/// ʵ��coding �ӿ�
	void update(CZDecoder *decoder_, bool deep = false);
	void encode(CZCoder *coder_, bool deep = false);
private:
	/// �������ݣ����ù켣���������ƣ�
	/// 
	///		����С���ȵ���ɢ��(points_)Ϊ���ģ��γ�С���Ρ������˾�������ͨ��CZPathRender����ͼ�νӿڻ��Ƴ�����
	///
	CZRect drawData();
	
	/// ����һ��stamp��
	void paintStamp(CZRandom *randomizer);

	/// ��������֮�����.
	///
	///		�������Ƶ�(linePoints)֮�������ɢ�ɸ�С���ȵ���ɢ��(points)��
	///		����points,sizes,alphas,angles��������
	///		�������õ��˻�ˢ�Ĳ�����
	///
	///		/param lastLocation - �켣�����ɢ���λ��
	///		/param location		- ��ǰ���Ƶ��λ��
	///		/param randomizer	- �����
	///		/note	���û��ʲ������ɲ��ֵ��㷨û����
	void paintBetweenPoints(const CZ3DPoint &lastLocation, const CZ3DPoint &location, CZRandom *randomizer);
	
	/// ʹ�켣��Ч��
	void invalidatePath();

public:
	bool							limitBrushSize;
	float							remainder;			///< ���ƹ켣�����ɢ����ռ�õ��߿ռ�
	CZPathRender					*ptrRender;			///< ·��������
	CZBrush							*ptrBrush;			///< ֻ�����ã���������������

private:
	bool					closed;
	bool					boundsDirty;				///< ���ڱ���Ƿ���Ҫ���¼����Χ����
	CZColor					color;
	CZRect					bounds;						///< �켣�ķ�Χ
	CZPathAction			action;
	float					scale;

	std::vector<CZBezierNode>		nodes;				///< �������������п��Ƶ�
	/// ���ջ��Ƶ�Ĳ���
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

