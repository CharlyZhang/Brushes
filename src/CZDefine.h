#ifndef _CZMACRO_H_
#define _CZMACRO_H_

#include "Macro.h"
/*#include <iostream>*/
#include <stdio.h>

#define GLSL_DIR	"../src/glsl/"

#ifndef NULL 
#	define NULL		0
#endif

#define M_E        2.71828182845904523536
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616


/******************************
 * common define
 *****************************/

//#define CZ_DEBUG		1		// for debugging
//#define USE_OPENGL		1
//#define USE_OPENGL_ES	0
//		
//
//#define RENDER_TWO_RECT		0		///< 绘制两个矩形
//#define RENDER_FULL_RECT	0		///< 绘制全屏纹理
//#define RENDER_STAMP		1		///< 绘制笔刷纹理
//#define RENDER_FREEHAND		0		///< 绘制手绘轨迹（不使用任何以下纹理）
//
//#define BRUSH_TEX		1		///< 笔刷纹理
//#define PATH_TEX		0		///< 轨迹纹理


#if USE_OPENGL
	#define GL_PIXEL_TYPE GL_FLOAT
	typedef float PixDataType;
#elif USE_OPENGL_ES
	#define GL_PIXEL_TYPE GL_UNSIGNED_BYTE
	typedef unsigned char PixDataType;
#endif

/// LOG
#define LOG_LEVEL_CUR	 LOG_LEVEL_DEBUG

#define LOG_LEVEL_ALL    4
#define LOG_LEVEL_DEBUG  3
#define LOG_LEVEL_INFO   2
#define LOG_LEVEL_WARN   1
#define LOG_LEVEL_ERROR  0

#define LOG(fmt, ...) printf(fmt, __VA_ARGS__)

#define LOG_ERROR(fmt, ...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_ERROR) LOG("[ERROR](%s):" fmt , __FUNCTION__, __VA_ARGS__);} while (0)
#define LOG_WARN(fmt,  ...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_WARN)  LOG("[WARN](%s):" fmt ,  __FUNCTION__,  __VA_ARGS__);} while (0)
#define LOG_INFO(fmt,  ...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_INFO)  LOG("[INFO](%s):" fmt ,  __FUNCTION__,  __VA_ARGS__);} while (0)
#define LOG_DEBUG(fmt, ...) do { if (LOG_LEVEL_CUR >= LOG_LEVEL_DEBUG) LOG("[DEBUG](%s):" fmt,  __FUNCTION__ , __VA_ARGS__);} while (0)

#endif