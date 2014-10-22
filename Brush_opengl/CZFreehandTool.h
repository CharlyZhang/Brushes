
///  \file CZFreehandTool.h
///  \brief This is the file declare the Class CZFreehandTool.
///
///		The CZFreehandTool is designed for input of drawing. 
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-28
///  \note It has no consideration of scaling and transfering of the canvas. 

#ifndef _CZFREEHANDTOOL_H_
#define _CZFREEHANDTOOL_H_

#include "CZTool.h"
#include "CZBezierNode.h"
#include "CZGeometry.h"
#include "CZPath.h"
#include "CZRandom.h"
#include "CZPainting.h"
#include "CZActiveState.h"
#include "CZUtil.h"
#include <vector>
#include "CZBrush.h"

class CZFreehandTool : public CZTool
{
public:
	CZFreehandTool(bool supportPressure = false);
	~CZFreehandTool();

	/// ��ʼ�ƶ������֧��ѹ���������Ϊѹ��ֵ��
	void moveBegin(CZ2DPoint &p_, float pressure_ = 0.0f);

	/// �����ƶ�
	///		
	///		������5���㴦��ÿ����һ�λ���һ�顣
	///
	///		/param p_				  - ��ǰ�ƶ��ĵ�λ��
	///		/param pressureOrSpeed	  - ���豸֧��ѹ��ֵʱ��Ϊѹ��ֵ������Ϊ�ƶ��ٶ�ֵ(pixels/millisecond)
	void moving(CZ2DPoint &p_, float pressureOrSpeed);

	/// �ƶ�����
	void moveEnd(CZ2DPoint &p_);

private:
	/// ����ʱ��������start_��end_�ĵ����ƽ������
	///		
	///		���崦����������ǰ��ֱ���ǰ�����������߳�l1��l2,�ٽ�l1��l2���е����߳�l3,
	///					ȡl3���е�Ϊ��ǰ���ƽ�������
	void averagePointsBetween(int start_, int end_);

	/// ��������ĵ�
	///
	///		ÿ�ν��ٽ��ļ�������о������ټ�����ڶ����㵽�����ڶ������Ŀ��Ƶ㣻
	///		����ǰ3�����������ߣ����л��ƣ�������������������һ�ֻ��ƣ��Ա�ƽ�����ɡ�
	void paintFittedPoints();

	void paintPath(CZPath &path);
public:
	float						scale;							///< ��Ļ�����ű�

private:
	std::vector<CZBezierNode>	accumulatedStrokePoints;		///< �ۻ��ıʻ�����
	bool						firstEver;						///< �Ƿ���ĳ�켣���״��ƶ�
	CZRect						strokeBounds;					///< �ʻ��켣�ķ�Χ
	CZ2DPoint					lastLocation;					///< �ʻ��켣�����һ����
	bool						realPressure;					///< �Ƿ�֧�����ѹ��
	CZBezierNode				pointsToFit[5];					///< ������ٽ������
	int							pointsIndex;					///< ����ĵ���
	bool						clearBuffer;					///< �Ƿ�Ҫ������
	float						lastRemainder;					///< ���ֻ��ƵĶ�ռ�ռ�
	CZRandom					*ptrRandomizer;					///< ָ��켣�������

public:
	CZPainting					*ptrPainting;					///< ָ�����
	bool						eraseMode;						///< �Ƿ��ǲ���״̬
};

#endif