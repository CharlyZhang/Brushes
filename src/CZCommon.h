
///  \file CZCommon.h
///  \brief This is the file declares all common data struct.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-21
///	 \note

#ifndef _CZCOMMON_H_
#define _CZCOMMON_H_

#include "Macro.h"

#if USE_OPENGL
    typedef float PixDataType;
#elif USE_OPENGL_ES
    typedef unsigned char PixDataType;
#endif

#endif