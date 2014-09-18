#include "GLShader.h"

CZShader::CZShader()
{
	this->m_VertexShader = NULL;
	this->m_FragmentShader = NULL;
	this->m_Program = NULL;
	this->m_Frag = NULL;
	this->m_Vert = NULL;
}

CZShader::~CZShader()
{
	if(NULL != m_VertexShader)
		delete[] m_VertexShader;
	if(NULL != m_FragmentShader)
		delete[] m_FragmentShader;

	//解除shader与program的绑定关系
	glDetachShader(m_Program,m_Vert);
	glDetachShader(m_Program,m_Frag);

	//删除shader和program
	glDeleteObjectARB(m_Vert);
	glDeleteObjectARB(m_Frag);
	glDeleteObjectARB(m_Program);
}

bool CZShader::TextFileRead(char *_fn, GLchar *&_shader)
{
	if(NULL == _fn)
		return false;

	FILE *fp;
	int count = 0;

	fp = fopen(_fn,"rt");
	if(NULL == fp)
		return false;

	// 将文件指针指向文件流末尾
	fseek(fp,0,SEEK_END);
	// 计算文件尾相对文件头的偏移量（文件长度）
	count = ftell(fp);
	// 把文件指针重新指向文件流头部
	rewind(fp);

	if(count<=0)
		return false;

	_shader = new GLchar[count];
	count = fread(_shader,sizeof(GLchar),count,fp);
	_shader[count] = '\0';
	fclose(fp);

	return true;
}

bool CZShader::ReadVertextShader(char *_fn)
{
	if(TextFileRead(_fn,m_VertexShader))
		return true;
	else
		return false;
}

bool CZShader::ReadFragmentShader(char *_fn)
{
	if(TextFileRead(_fn,m_FragmentShader))
		return true;
	else
		return false;
}

void CZShader::SetShader()
{
	//创建shader对象
	m_Vert = glCreateShader(GL_VERTEX_SHADER);
	m_Frag = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *vv = m_VertexShader;
	const GLchar *ff = m_FragmentShader;
	//添加shader
	glShaderSource(m_Vert,1,&vv,NULL);
	glShaderSource(m_Frag,1,&ff,NULL);

	//编译shader
	glCompileShader(m_Vert);
	glCompileShader(m_Frag);

	//创建程序对象
	m_Program = glCreateProgram();

	//绑定shader到程序对象
	glAttachShader(m_Program,m_Vert);
	glAttachShader(m_Program,m_Frag);

	//链接程序
	glLinkProgram(m_Program);
}

void CZShader::begin()
{
	//使用程序
	glUseProgram(m_Program);
}

void CZShader::end()
{
	//停止使用
	glUseProgram(0);
}