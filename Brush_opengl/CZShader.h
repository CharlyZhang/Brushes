
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
	/// ��ʼ��OpenGL��չ
	///		\note ����glew�ĳ�ʼ����Ӧ����OpenGL��glut�ĳ�ʼ��֮��
	static bool initOpenGLExtensions();
	/// �Ƿ�֧��GLSL
	static bool hasGLSLSupport();
	/// �������
	bool compile();

	static bool extensionsInit;			///< �Ƿ��ʼ��GL��չ
	static bool useGLSL;				///< GLSL�Ƿ��Ѿ�׼��
	static bool bGeometryShader;		///< �Ƿ�֧��G-Shader
	static bool bGPUShader4;			///< �Ƿ�֧��Shader4

	GLchar *m_VertexShader;
	GLchar *m_FragmentShader;

	GLuint m_Program;
	GLuint m_Vert,m_Frag;

	bool isCompiled;					///< �Ƿ����
};

#endif