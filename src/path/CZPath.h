
///  \file CPath.h
///  \brief This is the file declare the Class CPath.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2015-03-26
///  \note		

#ifndef _CZPATH_H_
#define _CZPATH_H_

#include "../serialization/CZCoding.h"
#include "../CZMacro.h"
#include "../basic/CZColor.h"
#include "../basic/CZ2DPoint.h"
#include "CZBezierNode.h"

#include <vector>

class CZ3DPoint;
class CZRect;
class CZBezierNode;
class CZRandom;
class CZBrush;

/// ����CZPathAction
typedef enum 
{
    CZPathActionPaint,
    CZPathActionErase
} CZPathAction;

/// ����CZPath
class CZPath : public CZCoding
{
public:
	CZPath(CZArray<CZBezierNode> *nodes_ = NULL);
	~CZPath();
	/// �������н��
	void CZPath::setNodes(const CZArray<CZBezierNode> &nodes_);
	/// ������п��Ƶ�
	const CZArray<CZBezierNode>& getNodes();
	/// ��ӽ��
	void addNode(CZBezierNode &node);
	/// �����Ƿ���
	void setClosed(bool isClosed);
	bool isClosed();
	/// ���ƹ켣�������������ݵķ�Χ��
	CZRect paint(CZRandom *randomizer_);
	/// ��ȡ������������ݸù켣�ı�ˢ����������
	CZRandom *getRandomizer();
	/// �������캯��
	CZPath&  operator=( const  CZPath & path_) ;
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
	
	/// ��һ��������ɢ�����ڽ�������α�������������
	/// 
	///		������㣨nodes���γ�һ�����α��������ߣ��ٽ����ߴ�ɢ�����ɸ����Ƶ㣨points��
	/// 
	///		/param nodes		- ��������ά���
	///		/param closed		- �Ƿ��γɱջ�
	///		/param points		- ��ɢ��õ��Ļ��Ƶ�����
	///		/return				- ��ɢ��õ��Ļ��Ƶ���Ŀ
	unsigned int flattenNodes2Points(const CZArray<CZBezierNode> &nodes, bool closed,CZArray<CZ3DPoint> &points);

public:
	bool							limitBrushSize;
	float							remainder;			///< ���ƹ켣�����ɢ����ռ�õ��߿ռ�
	CZBrush							*ptrBrush;			///< ֻ�����ã���������������
	CZPathAction					action;				///< ��ǹ켣�Ķ����ǻ��ƻ��ǲ���
	CZColor							color;

private:
	float							scale;
	bool							closed;

	CZArray<CZBezierNode>			nodes;				///< �������������п��Ƶ�
	/// ���ջ��Ƶ�Ĳ���
	CZArray<CZ2DPoint>				points;
	CZArray<float>					sizes;
	CZArray<float>					angles;
	CZArray<float>					alphas;				
};

#endif

