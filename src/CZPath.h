
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
///  \note		�����Ʋ��ִ�CZPath���޳�����ֻ�����������ݡ�ͬʱ�޸���Ӧ��������

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
	/// ��ʼ��
	void initital();
	/// �������н��
	void setNodes(const std::vector<CZBezierNode> &nodes_);
	/// ��ӽ��
	void addNode(CZBezierNode &node);
	/// ��ȡ��β���
	CZBezierNode firstNode();
	CZBezierNode lastNode();
	/// ���ƹ켣�������������ݵķ�Χ��
	CZRect paintPath(CZRender *render,CZRandom *randomizer_);
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
	/// ��������
	/// 
	///		����С���ȵ���ɢ��(points_)Ϊ���ģ��γ�С���Σ�ί��render���ƣ������˾������ݷ��ء�
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
	CZBrush							*ptrBrush;			///< ֻ�����ã���������������
	CZPathAction					action;				///< ��ǹ켣�Ķ����ǻ��ƻ��ǲ���
	CZColor							color;

private:
	bool					closed;
	bool					boundsDirty;				///< ���ڱ���Ƿ���Ҫ���¼����Χ����
	CZRect					bounds;						///< �켣�ķ�Χ
	float					scale;

	std::vector<CZBezierNode>		nodes;				///< �������������п��Ƶ�
	/// ���ջ��Ƶ�Ĳ���
	std::vector<CZ2DPoint>	points;
	std::vector<float>		sizes;
	std::vector<float>		angles;
	std::vector<float>		alphas;

	CZRender				*ptrRender;					///< �����ڲ���Ϣ���ݣ�ָ�������						
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

