
///  \file CZCoder.h
///  \brief This is the file declare the class CZCoder.
///
///		CZCoder is an interface. It defines the imperative methods of coder.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-25
///  \note

#ifndef _CZCODER_H_
#define _CZCODER_H_

#include "CZCoding.h"

class CZCoding;

class CZCoder
{
public:
	virtual void encodeObject(CZCoding* obj, const char* key, bool deep) = 0;
	virtual void encodeFloat(float value, const char* key) = 0;
	virtual void encodeString(char *str, const char * key) = 0;
	virtual void encodeUint(unsigned int value, const char * key) = 0;
};
#endif