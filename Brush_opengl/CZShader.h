
///  \file CZShader.h
///  \brief This is the file declare the Class CZShader.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-17
///  \note

#ifndef __CZSHADER_H_
#define __CZSHADER_H_

#include <stdio.h>
#include "GL/glew.h"


class CZShader
{
public:
	CZShader();
	~CZShader();
	bool readVertextShader(char *_fn);
	bool readFragmentShader(char *_fn);
	void setShader();
	void begin();
	void end();
	GLuint getAttributeLocation(const char* atrrName);
private:
	bool textFileRead(char *_fn,GLchar *&_shader);
	/// 初始化OpenGL扩展
	///		\note 包含glew的初始化，应该在OpenGL和glut的初始化之后
	static bool initOpenGLExtensions();
	/// 是否支持GLSL
	static bool hasGLSLSupport();
	/// 编译程序
	bool compile();

	static bool extensionsInit;			///< 是否初始化GL扩展
	static bool useGLSL;				///< GLSL是否已经准备
	static bool bGeometryShader;		///< 是否支持G-Shader
	static bool bGPUShader4;			///< 是否支持Shader4

	GLchar *m_VertexShader;
	GLchar *m_FragmentShader;

	GLuint m_Program;
	GLuint m_Vert,m_Frag;

	bool isCompiled;					///< 是否编译
};

#endif