
///  \file CZBezierSegment.h
///  \brief This is the file declare the Class CZBezierSegment.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-15
///  \note

#include "CZBezierNode.h"
#include "CZ3DPoint.h"
#include <vector>

class CZBezierSegment
{
public:
	/// ����Ϊ�ñ����������ĸ����Ƶ�
	CZ3DPoint *start;
	CZ3DPoint *outHandle;
	CZ3DPoint *inHandle;
	CZ3DPoint *end;

	CZBezierSegment(CZ3DPoint *start_=NULL, CZ3DPoint *out_=NULL, CZ3DPoint *in_=NULL, CZ3DPoint *end_=NULL);
	~CZBezierSegment();
	/// �����㹹�챴�����߶Σ���̬������
	/// 
	///		/param start_	- �������������
	///		/param end_		- �����������յ�
	///		/return			- ���ɵı���������
	static CZBezierSegment* segmentBetweenNodes(CZBezierNode *start_, CZBezierNode *end_);
	/// �����������ߴ�ɢ�ɻ��Ƶ�
	///
	///		/param &points	- ʢװ��ɢ�Ļ��Ƶ������
	void flattenIntoArray(std::vector<CZ3DPoint*> & points);
	/// �жϱ����������Ƿ�ƽ����
	///
	///		��ʱ�������ף�ϸ���㷨.
	/// 
	///		/param tolerance	- ���ֵ
	///
	bool isFlatWithTolerance(float tolerance);
	/// �����߶δ�t���ָ�
	///
	///		���ü��η��������߶ε��ĸ����Ƶ��γɵı��������ߴ�t���ָ���������α���������
	/// 
	///		/param t	- �ָ���������ߵ�λ�ò���[0,1]
	///		/param *L	- �ָ���󲿷ֵı���������
	///		/param *R	- �ָ���Ҳ��ֵı���������
	///		/return		- �ָ�����ϵĵ�
	/// 
	CZ3DPoint* splitAtT(float t, CZBezierSegment *L, CZBezierSegment *R);

};