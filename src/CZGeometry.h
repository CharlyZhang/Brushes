
///  \file CZGeometry.h
///  \brief This is the file declare a series of geometry Class.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-11
///  \note

#ifndef _CZGEOMETRY_H_
#define _CZGEOMETRY_H_

/// 二维点
class CZ2DPoint
{
public:
	float x,y;

	CZ2DPoint(){};
	CZ2DPoint(float x_, float y_): x(x_), y(y_){};

	bool  operator<( const  CZ2DPoint & p_) const ;
	bool  operator==( const  CZ2DPoint & p_) const ;
	void  operator=( const  CZ2DPoint & p_) ;

	/// Here we overload operators 
	CZ2DPoint operator+(CZ2DPoint vVector)
	{
		// Return the added vectors result.
		return CZ2DPoint(vVector.x + x, vVector.y + y);
	}
	CZ2DPoint operator-(CZ2DPoint vVector)
	{
		// Return the subtracted vectors result
		return CZ2DPoint(x - vVector.x, y - vVector.y);
	}
	CZ2DPoint operator*(float num)
	{
		// Return the scaled vector
		return CZ2DPoint(x * num, y * num);
	}
	float operator*(CZ2DPoint vVector)
	{
		return (x*vVector.x+y*vVector.y);
	}
	CZ2DPoint operator/(float num)
	{
		// Return the scale vector
		return CZ2DPoint(x / num, y / num);
	}


	/// 求到另外一点距离
	float distanceTo2DPoint(CZ2DPoint & p_);
};

/// 尺寸
class CZSize
{
public:
	float width,height;

	CZSize(float w_ = 0.0f, float  h_ = 0.0f): width(w_), height(h_){};

	/*void operator=(const CZSize & s_)
	{
		this->width = s_.width;
		this->height = s_.height;
	}
	*/
	bool operator==(const CZSize &s_)
	{
		if(width == s_.width && height == s_.height) return true;
		else										 return false;
	}

	CZSize operator*(float num) const 
	{
		// Return the scaled vector
		return CZSize(width * num, height * num);
	}
};

/// 矩形类
class CZRect
{
public:
	CZ2DPoint	origin;
	CZSize		size;	

	CZRect(): origin(0.0f,0.0f), size(0.0f,0.0f){};
	CZRect(float x_, float y_, float w_, float h_): origin(x_,y_), size(w_,h_){};

	inline float getMinX() const { return origin.x; };
	inline float getMinY() const { return origin.y; };
	inline float getMaxX() const { return origin.x + size.width; };
	inline float getMaxY() const { return origin.y + size.height; };
	inline CZ2DPoint getCenter()
	{ 
		return CZ2DPoint(origin.x + size.width/2, origin.y + size.height/2); 
	};

	/// 返回零矩形
	static CZRect zeroRect(){ return CZRect(); };
	/// 判断是否为零矩形
	bool isZeroRect() const;
	/// 合并其他矩形
	CZRect unionWith(const CZRect & rect_);
	/// 扩展的整数矩形
	CZRect expandToIntergral();
};

#endif