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

	//���shader��program�İ󶨹�ϵ
	glDetachShader(m_Program,m_Vert);
	glDetachShader(m_Program,m_Frag);

	//ɾ��shader��program
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

	// ���ļ�ָ��ָ���ļ���ĩβ
	fseek(fp,0,SEEK_END);
	// �����ļ�β����ļ�ͷ��ƫ�������ļ����ȣ�
	count = ftell(fp);
	// ���ļ�ָ������ָ���ļ���ͷ��
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
	//����shader����
	m_Vert = glCreateShader(GL_VERTEX_SHADER);
	m_Frag = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *vv = m_VertexShader;
	const GLchar *ff = m_FragmentShader;
	//���shader
	glShaderSource(m_Vert,1,&vv,NULL);
	glShaderSource(m_Frag,1,&ff,NULL);

	//����shader
	glCompileShader(m_Vert);
	glCompileShader(m_Frag);

	//�����������
	m_Program = glCreateProgram();

	//��shader���������
	glAttachShader(m_Program,m_Vert);
	glAttachShader(m_Program,m_Frag);

	//���ӳ���
	glLinkProgram(m_Program);
}

void CZShader::begin()
{
	//ʹ�ó���
	glUseProgram(m_Program);
}

void CZShader::end()
{
	//ֹͣʹ��
	glUseProgram(0);
}