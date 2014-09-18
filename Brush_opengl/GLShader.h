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
private:
	bool textFileRead(char *_fn,GLchar *&_shader);
	GLchar *m_VertexShader;
	GLchar *m_FragmentShader;

	GLuint m_Program;
	GLuint m_Vert,m_Frag;
};

#endif