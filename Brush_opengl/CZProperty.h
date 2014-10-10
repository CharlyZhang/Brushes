
///  \file CZProperty.h
///  \brief This is the file declare the Class CZProperty and the Interface CZPropertyDelegate.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-12
///  \note

#ifndef _CZPROPERTY_H_
#define _CZPROPERTY_H_

#include "CZCoding.h"
#include <string>

class CZPropertyDelegate;

class CZProperty : public CZCoding
{
public:
	void update(CZDecoder *decoder_, bool deep = false){};
	void encode(CZCoder *coder_, bool deep = false){};

	float value;
	float maximumValue, minimumValue;
	float conversionFactor;
	bool percentage;
	std::string title;

	CZPropertyDelegate *ptrDelegate;
};

class CZPropertyDelegate 
{
public:
	virtual void propertyChanged(CZProperty &property_) = 0;
};

#endif