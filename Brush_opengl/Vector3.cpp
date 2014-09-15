#include "Vector3.h"
#include "math.h"
bool cmp( CVector3 v1, CVector3 v2 )
{
	if ( v1.x < v2.x )
	{
		return true;
	} 
	else if ( v1.x==v2.x && v1.y < v2.y )
	{
		return true;
	}
	else if ( v1.x==v2.x && v1.y==v2.y && v1.z<v2.z )
	{
		return true;
	} 
	else
	{
		return false;
	}
}
bool CVector3::operator <( const  CVector3& a) const
{
	if ( this->x < a.x )
	{
		return true;
	} 
	else if ( this->x==a.x && this->y < a.y )
	{
		return true;
	}
	else if ( this->x==a.x && this->y==a.y && this->z<a.z )
	{
		return true;
	} 
	else
	{
		return false;
	}
}

bool CVector3::operator ==( const  CVector3& a) const
{
	if ( this->x==a.x && this->y==a.y && this->z==a.z )
	{
		return true;
	} 
	else
	{
		return false;
	}
}

void CVector3::operator =( const  CVector3& a) 
{
	this->x = a.x; 
	this->y = a.y;
	this->z = a.z;
}

// 下面的函数求两点决定的矢量
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
	CVector3 vVector;              

	vVector.x = vPoint1.x - vPoint2.x;      
	vVector.y = vPoint1.y - vPoint2.y;      
	vVector.z = vPoint1.z - vPoint2.z;      

	return vVector;                
}

CVector3 equ( int x, int y, int z )
{
	CVector3 res;
	res.x = (float)x;
	res.y = (float)y;
	res.z = (float)z;

	return res;
}

// 下面的函数两个矢量相加
CVector3 AddVector(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vResult;              

	vResult.x = vVector2.x + vVector1.x;    
	vResult.y = vVector2.y + vVector1.y;    
	vResult.z = vVector2.z + vVector1.z;    

	return vResult;                
}

// 下面的函数处理矢量的缩放
CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler)
{
	CVector3 vResult;              

	vResult.x = vVector1.x / Scaler;      
	vResult.y = vVector1.y / Scaler;      
	vResult.z = vVector1.z / Scaler;      

	return vResult;                
}

// 下面的函数返回两个矢量的叉积
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;                

	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));

	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));

	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;                
}

/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the magnitude of a vector
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Magnitude(CVector3 vNormal)
{
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
	return (float)sqrt( (vNormal.x * vNormal.x) + 
		(vNormal.y * vNormal.y) + 
		(vNormal.z * vNormal.z) );
}


/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a normalize vector (A vector exactly of length 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CVector3 Normalize(CVector3 vVector)
{
	// Get the magnitude of our normal
	float magnitude = Magnitude(vVector);				

	// Now that we have the magnitude, we can divide our vector by that magnitude.
	// That will make our vector a total length of 1.  
	vVector = vVector / magnitude;		

	// Finally, return our normalized vector
	return vVector;										
}

//////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &ostr, CVector3 v) {

	return ostr << "(" <<v.x << " " << v.y << " " << v.z << ")";

}

std::istream& operator>>(std::istream &istr, CVector3& v) {

	return istr >> v.x >> v.y >> v.z;

}