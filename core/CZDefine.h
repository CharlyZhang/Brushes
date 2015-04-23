#ifndef _CZMACRO_H_
#define _CZMACRO_H_

#include <stdio.h>

#define GLSL_DIR	"../../core/glsl/"

#ifndef NULL
# define NULL       0
#endif
#ifndef M_E
# define M_E        2.71828182845904523536
#endif

#ifndef M_PI
# define M_PI       3.14159265358979323846
#endif

#ifndef M_PI_2
# define M_PI_2     1.57079632679489661923
#endif

#ifndef M_PI_4
# define M_PI_4     0.785398163397448309616
#endif

#define CZ_DEBUG_GL		1		// for debugging
#define USE_OPENGL		1
#define USE_OPENGL_ES	0

#define RENDER_TWO_RECT		0		///< 绘制两个矩形
#define RENDER_FULL_RECT	0		///< 绘制全屏纹理
#define RENDER_FREEHAND		1		///< 绘制手绘轨迹（不使用任何以下纹理）
#define RENDER_IMGAGE		0		///< render image onto active layer

#define STAMP_TEX		0		///< 笔触纹理
#define STROKE_TEX		0		///< 轨迹纹理

/// GL type
#define GLUINT						unsigned int

#if USE_OPENGL
# define PixDataType				float
# define DEFAULT_STORAGEMODE		RGBA_FLOAT
#elif USE_OPENGL_ES
# define PixDataType				unsigned char
# define DEFAULT_STORAGEMODE		RGBA_BYTE
#endif

/// LOG
#define LOG_LEVEL_CUR	 LOG_LEVEL_DEBUG

#define LOG_LEVEL_ALL    4
#define LOG_LEVEL_DEBUG  3
#define LOG_LEVEL_INFO   2
#define LOG_LEVEL_WARN   1
#define LOG_LEVEL_ERROR  0

#if USE_OPENGL

#define LOG(fmt, ...) printf(fmt, __VA_ARGS__)

#define LOG_ERROR(fmt, ...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_ERROR) LOG("[ERROR](%s):" fmt , __FUNCTION__, __VA_ARGS__);} while (0)
#define LOG_WARN(fmt,  ...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_WARN)  LOG("[WARN](%s):" fmt ,  __FUNCTION__,  __VA_ARGS__);} while (0)
#define LOG_INFO(fmt,  ...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_INFO)  LOG("[INFO](%s):" fmt ,  __FUNCTION__,  __VA_ARGS__);} while (0)
#define LOG_DEBUG(fmt, ...) do { if (LOG_LEVEL_CUR >= LOG_LEVEL_DEBUG) LOG("[DEBUG](%s):" fmt,  __FUNCTION__ , __VA_ARGS__);} while (0)

#elif USE_OPENGL_ES
#define LOG(fmt, args...) printf(fmt, ##args)

#define LOG_ERROR(fmt, args...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_ERROR) LOG("[ERROR](%s):" fmt , __FUNCTION__, ##args);} while (0)
#define LOG_WARN(fmt,  args...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_WARN)  LOG("[WARN](%s):" fmt ,  __FUNCTION__, ##args);} while (0)
#define LOG_INFO(fmt,  args...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_INFO)  LOG("[INFO](%s):" fmt ,  __FUNCTION__, ##args);} while (0)
#define LOG_DEBUG(fmt, args...) do { if (LOG_LEVEL_CUR >= LOG_LEVEL_DEBUG) LOG("[DEBUG](%s):" fmt,  __FUNCTION__ ,##args);} while (0)
#endif

#endif