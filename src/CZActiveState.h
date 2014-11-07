
///  \file CZActiveState.h
///  \brief This is the file declare the CZActiveState Class.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-28
///  \note		It is a Singleton

#ifndef _CZACTIVESTATE_H_
#define _CZACTIVESTATE_H_

#include "CZStampGenerator.h"
#include "CZColor.h"
#include "CZBrush.h"
#include "CZTool.h"
#include <string>
#include <vector>
#include <map>

/// ����״̬��
class CZActiveState
{
public:
	/// ��ȡ����
	static CZActiveState * getInstance()
	{
		static CZActiveState instance;   
		return &instance; 
	}

	/// ���û���ģʽ
	void setEraseMode(bool flag);
	/// ��ȡ����ģʽ
	bool isEraseMode();
	/// ���û�����ɫ
	void setPaintColor(CZColor &pc);
	void setPaintColor(float r, float g, float b, float a = 1.0f);
	/// ��ȡ������ɫ
	CZColor getPaintColor();
	/// ���ÿ��û�ˢ
	void setBrushes(std::vector<CZBrush*> &bs);
	/// ��ӻ�ˢ
	/// 
	///		\note ����ڵ�ǰ����Ļ�ˢ֮��
	void addBrush(CZBrush *b);
	/// ��ÿ��û�ˢ
	std::vector<CZBrush*> &getBrushes();
	/// ���õ�ǰ���ˢ
	///
	///		\param idx - ���û�ˢ����ţ����Ƿ�ʱĬ��Ϊ0
	///		\ret	   - ԭ���ļ��ˢ���
	///		\note	��ǰ���õĻ�ˢ������ģʽ����
	int setActiveBrush(int idx);
	/// ��ȡ��ǰ��ˢ
	/// 
	///		\note ��ˢ�����ɵ�ǰ��״̬eraseMode����
	CZBrush *getActiveBrush();
	/// ɾ����ǰ��ˢ
	void deleteActiveBrush();
	/// ��ȡ��ǰ����
	/// 
	///		\note ���������ɵ�ǰ��״̬eraseMode����
	CZTool *getActiveTool();
	

	/// ��ȡ���һ����ˢ������
	CZStampGenerator * getRandomGenerator();

private:
	CZActiveState();
	CZActiveState(const CZActiveState &);
	CZActiveState & operator = (const CZActiveState &);
	~CZActiveState();

	/// ��û�ˢ�����л�ˢ�еı�ţ������ڷ��ظ�ֵ
	int indexOfBrushes(CZBrush *brush);

private:
	std::vector<CZBrush*> brushes;						///< ���п��õĻ�ˢ����������
	std::vector<CZTool*> tools;							///< ���ù��ߣ�ĿǰΪ���ƻ�ˢ�Ͳ���ˢ����
	std::map<std::string,CZStampGenerator*>	generators;	///< ���п��õĻ�ˢ����������������
	bool eraseMode;										///< ����Ƿ��ڲ���ģʽ
	CZBrush *ptrPaintBrush;								///< �����ñ�ˢ
	CZBrush *ptrEraseBrush;								///< �����ñ�ˢ 

	CZColor paintColor;									///< ������ɫ
};

#endif