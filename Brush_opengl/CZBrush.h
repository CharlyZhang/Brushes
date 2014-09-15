
///  \file CZBrush.h
///  \brief This is the file declare the Class CZBrush.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-12
///  \note

#ifndef _CZBRUSH_H_
#define _CZBRUSH_H_

#include "CZProperty.h"

class CZBrush
{
public:
	CZProperty weight;             // [1.0, 512.0] -- pixels
	CZProperty intensity;          // [0.0, 1.0]

	CZProperty angle;              // [0.0, 1.0];
	CZProperty spacing;            // [0.01, 2.0] -- percentage of brush width
	CZProperty rotationalScatter;  // [0.0, 1.0]
	CZProperty positionalScatter;  // [0.0, 1.0]

	CZProperty angleDynamics;      // [-1.0, 1.0]
	CZProperty weightDynamics;     // [-1.0, 1.0]
	CZProperty intensityDynamics;  // [-1.0, 1.0]

};
#endif