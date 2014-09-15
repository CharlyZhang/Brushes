#ifndef _VECTOR3_H_
#define _VECTOR3_H_
#include <iostream>
// 定义3D点的类，用于保存模型中的顶点
class CVector3 
{
public:
	CVector3( float xx=0.0, float yy=0.0, float zz=0.0 )
	{
		this->x = xx;
		this->y = yy;
		this->z = zz;
	}
	bool  operator<( const  CVector3& a) const ;
	bool  operator==( const  CVector3& a) const ;
	void  operator=( const  CVector3& a) ;

	// Here we overload the + operator so we can add vectors together 
	CVector3 operator+(CVector3 vVector)
	{
		// Return the added vectors result.
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// Here we overload the - operator so we can subtract vectors 
	CVector3 operator-(CVector3 vVector)
	{
		// Return the subtracted vectors result
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	// Here we overload the * operator so we can multiply by scalars
	CVector3 operator*(float num)
	{
		// Return the scaled vector
		return CVector3(x * num, y * num, z * num);
	}
	
	float operator*(CVector3 vVector)
	{
		return (x*vVector.x+y*vVector.y+z*vVector.z);
	}
	// Here we overload the / operator so we can divide by a scalar
	CVector3 operator/(float num)
	{
		// Return the scale vector
		return CVector3(x / num, y / num, z / num);
	}

	void SetFromUIntColor(unsigned int col) 
	{
		x = ((float)(((unsigned char*)&col)[0]))/255.0f;
		y = ((float)(((unsigned char*)&col)[1]))/255.0f;
		z = ((float)(((unsigned char*)&col)[2]))/255.0f;
	}

	friend std::ostream& operator<<(std::ostream &ostr, CVector3 v);
	friend std::istream& operator>>(std::istream &istr, CVector3& v);

	//cast to pointer to float for glVertex4fv etc
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	float x, y, z;
};

bool cmp( CVector3 v1, CVector3 v2 );

CVector3 equ( int x, int y, int z );

// 下面的函数求两点决定的矢量
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2);

// 下面的函数两个矢量相加
CVector3 AddVector(CVector3 vVector1, CVector3 vVector2);

// 下面的函数处理矢量的缩放
CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler);

// 下面的函数返回两个矢量的叉积
CVector3 Cross(CVector3 vVector1, CVector3 vVector2);
//
//// 下面的函数规范化矢量
float Magnitude(CVector3 vNormal);
CVector3 Normalize(CVector3 vVector);

#endif