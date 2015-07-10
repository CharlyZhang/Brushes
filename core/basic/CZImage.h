
///  \file CZImage.h
///  \brief This is the file declare the Class CZImage.
///
///		This file includes CZImage
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-09
///  \note

#ifndef _CZIMAGE_H_
#define _CZIMAGE_H_

#include "../CZDefine.h"

typedef enum StorageMode 
{
	NOT_SET,RGB_BYTE, RGBA_BYTE, RGB_FLOAT, RGBA_FLOAT
} StorageMode;			///< 声明图像存储模式

class CZImage
{
public:
	CZImage(int w_=0, int h_=0, StorageMode mode_ = DEFAULT_STORAGEMODE, void* data_ = NULL);
	~CZImage();

	StorageMode getMode() { return mode;}

	/// 深度拷贝函数
	CZImage* copy();
	/// 填充（采用广搜）
	bool modifyDataFrom(int x,int y, float red, float green, float blue, float alpha);
	bool modifyDataFrom1(int x,int y, float red, float green, float blue, float alpha);
	/// 扫描线算法(封闭)
	void ScanLineFill(int x,int y, float r, float g, float b, float a);
private:
	/// 修改
	void modifyData(int x,int y, float fillcolor[]);
	/// 是否颜色相同
	inline bool isSameColorAt(int x,int y, float compareColor[]);
	/// 获取某点颜色
	bool getColorAt(int x, int y, float color[]);

public:
	int			width,height;
	void		*data;		///< 具体类型由mode决定

private:
	StorageMode	mode;
	bool		*flag;///< 用于填充的辅助数组
};

#endif