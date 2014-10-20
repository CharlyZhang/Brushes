
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
#include <vector>
#include <string>
#include <map>
#include "GL/glew.h"


class CZShader
{
public:
	CZShader();
	CZShader(const char* vertFileName, const char* fragFileName, \
		std::vector<std::string>& atrributes, std::vector<std::string>& uniforms);
	~CZShader();
	void begin();
	void end();
	GLuint getAttributeLocation(const char* atrrName);
	GLuint getUniformLocation(const std::string& str);
private:
	/// ������ɫ��
	void destroyShaders(GLuint vertShader,GLuint fragShader, GLuint prog);
	/// ��ȡ��ɫ������
	bool textFileRead(const char *_fn,GLchar *&_shader);
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
	std::map<std::string,GLuint> m_uniforms;
};

#endif