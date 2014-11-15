
///  \file CZShader.cpp
///  \brief This is the file implement the Class CZShader.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-17
///  \note

#include "CZShader.h"
#include <iostream>

#if USE_OPENGL_ES
#include <OpenGLES/EAGL.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

using namespace std;

// Error, Warning and Info Strings
char* aGLSLStrings[] = {
	"[e00] GLSL is not available!",
	"[e01] Not a valid program object!",
	"[e02] Not a valid object!",
	"[e03] Out of memory!",
	"[e04] Unknown compiler error!",
	"[e05] Linker log is not available!",
	"[e06] Compiler log is not available!",
	"[Empty]"
};

//////////////////////////////////////////////////////////////////////////

// GL ERROR CHECK
int CheckGLError(const string& file, int line)
{
	GLenum glErr;
	int    retCode = 0;
	glErr = glGetError();
	while (glErr != GL_NO_ERROR) 
	{
#if USE_OPENGL
		const GLubyte* sError = gluErrorString(glErr);

		if (sError)
			cout << "GL Error #" << glErr << "(" << gluErrorString(glErr) << ") " << " in File " << file.c_str() << " at line: " << line << endl;
		else
			cout << "GL Error #" << glErr << " (no message available)" << " in File " << file.c_str() << " at line: " << line << endl;

#elif USE_OPENGL_ES
        switch (glErr) {
            case GL_INVALID_ENUM:
                NSLog(@"GL Error: Enum argument is out of range");
                break;
            case GL_INVALID_VALUE:
                NSLog(@"GL Error: Numeric value is out of range");
                break;
            case GL_INVALID_OPERATION:
                NSLog(@"GL Error: Operation illegal in current state");
                break;
                //        case GL_STACK_OVERFLOW:
                //            NSLog(@"GL Error: Command would cause a stack overflow");
                //            break;
                //        case GL_STACK_UNDERFLOW:
                //            NSLog(@"GL Error: Command would cause a stack underflow");
                //            break;
            case GL_OUT_OF_MEMORY:
                NSLog(@"GL Error: Not enough memory to execute command");
                break;
            case GL_NO_ERROR:
                if (1) {
                    NSLog(@"No GL Error");
                }
                break;
            default:
                NSLog(@"Unknown GL Error");
                break;
        }
#endif
		retCode = 1;
		glErr = glGetError();
	}

	return retCode;
}

#define CHECK_GL_ERROR() CheckGLError(__FILE__, __LINE__)

//////////////////////////////////////////////////////////////////////////

bool CZShader::extensionsInit = true;
bool CZShader::useGLSL = true;
bool CZShader::bGeometryShader = false;
bool CZShader::bGPUShader4 = false;

void printShaderInfoLog(GLuint obj);
void printProgramInfoLog(GLuint obj);

CZShader::CZShader(const char* vertFileName, const char* fragFileName)
{
	initOpenGLExtensions();

	this->m_VertexShader = NULL;
	this->m_FragmentShader = NULL;
	this->m_Program = NULL;
	this->m_Frag = NULL;
	this->m_Vert = NULL;

	if(!textFileRead(vertFileName,m_VertexShader))	return;
	if(!textFileRead(fragFileName,m_FragmentShader))return;


	//创建shader对象
	m_Vert = glCreateShader(GL_VERTEX_SHADER);
	m_Frag = glCreateShader(GL_FRAGMENT_SHADER);

	//创建程序对象
	m_Program = glCreateProgram();

	if(!compile()) 
	{
		printShaderInfoLog(m_Vert);
		printShaderInfoLog(m_Frag);
		destroyShaders(m_Vert,m_Frag,m_Program);
		return;
	}

	//绑定shader到程序对象
	glAttachShader(m_Program,m_Vert);
	glAttachShader(m_Program,m_Frag);
    
	//链接程序
	glLinkProgram(m_Program);
	printProgramInfoLog(m_Program);

	// release vertex and fragment shaders
	if (m_Vert) 
	{
		glDeleteShader(m_Vert);
		m_Vert = NULL;
	}
	if (m_Frag) 
	{
		glDeleteShader(m_Frag);
		m_Frag = NULL;
	}
}

CZShader::CZShader(const char* vertFileName, const char* fragFileName, \
	vector<string>& attributes, vector<string>& uniforms)
{
	initOpenGLExtensions();

	this->m_VertexShader = NULL;
	this->m_FragmentShader = NULL;
	this->m_Program = NULL;
	this->m_Frag = NULL;
	this->m_Vert = NULL;

#if USE_OPENGL
    
	char fileName[128];
	char *pathName = "../src/shader/";
	strcpy(fileName,pathName);
	strcat(fileName,vertFileName);
	if(!textFileRead(fileName,m_VertexShader))	return;

	strcpy(fileName,pathName);
	strcat(fileName,fragFileName);
	if(!textFileRead(fileName,m_FragmentShader))return;

#elif USE_OPENGL_ES
    NSString *vertShaderPathname, *fragShaderPathname;
    NSString *_filename;
    const GLchar *source;
    int len = 0;
    
    // Create and compile vertex shader.
    _filename = [NSString stringWithCString: vertFileName encoding: NSASCIIStringEncoding ];
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:_filename ofType:@"vert"];
    
    source = (GLchar *)[[NSString stringWithContentsOfFile:vertShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source) {
        NSLog(@"Failed to load vertex shader");
    }
    
    len = strlen(source);
    m_VertexShader = new GLchar[len+1];
    strcpy(m_VertexShader, source);
    m_VertexShader[len] = '\0';
    
    // Create and compile fragment shader.
    _filename = [NSString stringWithCString: fragFileName encoding: NSASCIIStringEncoding ];
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:_filename ofType:@"frag"];
    
    source = (GLchar *)[[NSString stringWithContentsOfFile:fragShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source) {
        NSLog(@"Failed to load vertex shader");
    }
    
    len = strlen(source);
    m_FragmentShader = new GLchar[len+1];
    strcpy(m_FragmentShader, source);
    m_FragmentShader[len] = '\0';
  
#endif
    
    //创建shader对象
	m_Vert = glCreateShader(GL_VERTEX_SHADER);
	m_Frag = glCreateShader(GL_FRAGMENT_SHADER);
    
	//创建程序对象
	m_Program = glCreateProgram();
    
	if(!compile())
	{
		printShaderInfoLog(m_Vert);
		printShaderInfoLog(m_Frag);
		destroyShaders(m_Vert,m_Frag,m_Program);
		return;
	}
    
	//绑定shader到程序对象
	glAttachShader(m_Program,m_Vert);
	glAttachShader(m_Program,m_Frag);
    
	//绑定属性名
	for(unsigned int i=0; i<attributes.size(); i++)
	{
		glBindAttribLocation(m_Program, (GLuint) i, (const GLchar*) attributes[i].c_str());
		CHECK_GL_ERROR();
	}

	//链接程序
	glLinkProgram(m_Program);
	printProgramInfoLog(m_Program);
    
	//绑定uniform对象
	for(unsigned int i=0; i<uniforms.size(); i++)
	{
		GLuint location = glGetUniformLocation(m_Program, uniforms[i].c_str());
		CHECK_GL_ERROR();
		m_uniforms[uniforms[i]] = location;
	}

	// release vertex and fragment shaders
	if (m_Vert) 
	{
		glDeleteShader(m_Vert);
		m_Vert = NULL;
	}
	if (m_Frag) 
	{
		glDeleteShader(m_Frag);
		m_Frag = NULL;
	}
	
}

CZShader::~CZShader()
{
	if(NULL != m_VertexShader)
		delete[] m_VertexShader;
	if(NULL != m_FragmentShader)
		delete[] m_FragmentShader;

	//删除program
	glDeleteProgram(m_Program);
}

/// 销毁着色器
void CZShader::destroyShaders(GLuint vertShader,GLuint fragShader, GLuint prog)
{
	if(vertShader) glDeleteShader(vertShader);
	if(fragShader) glDeleteShader(fragShader);
	if(prog) glDeleteProgram(prog);
}

bool CZShader::textFileRead(const char *_fn, GLchar *&_shader)
{
#if USE_OPENGL
	if(NULL == _fn)
		return false;

	FILE *fp;
	int count = 0;

	fopen_s(&fp,_fn,"rt");
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

	_shader = new GLchar[count+1];
	count = fread(_shader,sizeof(GLchar),count,fp);
	_shader[count] = '\0';
	fclose(fp);
#endif
    
	return true;
}

/// 初始化OpenGL扩展
bool CZShader::initOpenGLExtensions()
{
#if USE_OPENGL
	if (extensionsInit) return true;
	extensionsInit = true;

	GLenum err = glewInit();
	CHECK_GL_ERROR();  

	if (GLEW_OK != err)
	{
		std::cout << "Error:" << glewGetErrorString(err) << std::endl;
		extensionsInit = false;
		return false;
	}

	hasGLSLSupport();
#endif
	return true;
}

//// 是否支持GLSL
bool CZShader::hasGLSLSupport()
{
#if USE_OPENGL
	if (useGLSL) return true;							///< already initialized and GLSL is available
	useGLSL = true;

	initOpenGLExtensions();								///< extensions were not yet initialized!!


	if(GLEW_VERSION_3_2)
	{
		cout << "OpenGL 3.2 (or higher) is available!" << endl;
		useGLSL = true;
		bGeometryShader = true;
		bGPUShader4 = true;
		cout << "[OK] OpenGL Shading Language is available!\n\n";
		return true;
	}
	else if(GLEW_VERSION_3_1)
	{
		cout << "OpenGL 3.1 core functions are available!" << endl;
	}
	else if(GLEW_VERSION_3_0)
	{
		cout << "OpenGL 3.0 core functions are available!" << endl;
	}
	else if(GLEW_VERSION_2_1)
	{
		cout << "OpenGL 2.1 core functions are available!" << endl;
	}
	else if (GLEW_VERSION_2_0)
	{
		cout << "OpenGL 2.0 core functions are available!" << endl;
	}
	else if (GLEW_VERSION_1_5)
	{
		cout << "OpenGL 1.5 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_4)
	{
		cout << "OpenGL 1.4 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_3)
	{
		cout << "OpenGL 1.3 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_2)
	{
		cout << "OpenGL 1.2 core functions are available" << endl;
	}

	/// if we have opengl ver < 3.2, need to load extensions.
	cout<<"Checking for Extensions: "<<endl;

	//Extensions supported
	if (!glewIsSupported("GL_ARB_fragment_shader"))
	{
		cout << "[WARNING] GL_ARB_fragment_shader extension is not available!\n";
		useGLSL = false;
	}else{
		cout << "[OK] GL_ARB_fragment_shader extension is available!\n";

	}

	if (!glewIsSupported("GL_ARB_vertex_shader"))
	{
		cout << "[WARNING] GL_ARB_vertex_shader extension is not available!\n";
		useGLSL = false;
	}else{
		cout << "[OK] GL_ARB_vertex_shader extension is available!\n";
	}

	if (!glewIsSupported("GL_ARB_shader_objects"))
	{
		cout << "[WARNING] GL_ARB_shader_objects extension is not available!\n";
		useGLSL = false;
	}else{
		cout << "[OK] GL_ARB_shader_objects extension is available!\n";
	}


	if (!glewIsSupported("GL_ARB_geometry_shader4"))
	{
		cout << "[WARNING] GL_ARB_geometry_shader4 extension is not available!\n";
		bGeometryShader = false;
	}else{
		cout << "[OK] GL_ARB_geometry_shader4 extension is available!\n";
		bGeometryShader = true;
	}

	if(!glewIsSupported("GL_EXT_gpu_shader4")){
		cout << "[WARNING] GL_EXT_gpu_shader4 extension is not available!\n";
		bGPUShader4 = false;
	}else{
		cout << "[OK] GL_EXT_gpu_shader4 extension is available!\n";
		bGPUShader4 = true;
	}

	///end detecting extensions
	if (useGLSL)
	{
		cout << "[OK] OpenGL Shading Language is available!\n\n";
	}
	else
	{
		cout << "[FAILED] OpenGL Shading Language is not available...\n\n";
	}   
#endif
	return useGLSL;
}

void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		//printf("%s\n",infoLog);
		std::cout << infoLog << std::endl;
		free(infoLog);
	}
}
void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		//printf("%s\n",infoLog);
		cout << infoLog <<endl;
		free(infoLog);
	}
}


/// 编译程序
bool CZShader::compile()
{
	if (!useGLSL) return false;

	isCompiled = false;

	GLint compiled = 0;

	const GLchar *vv = m_VertexShader;
	const GLchar *ff = m_FragmentShader;
	GLint	vertLen = (GLint) strlen((const char*)vv);
	GLint	fragLen = (GLint) strlen((const char*)ff);

	if (vv == NULL || ff == NULL) return false;

	//添加shader
	glShaderSource(m_Vert,1,&vv, &vertLen);
	CHECK_GL_ERROR();
	glShaderSource(m_Frag,1,&ff, &fragLen);
	CHECK_GL_ERROR();
	
	//编译shader
	glCompileShader(m_Vert);
	CHECK_GL_ERROR();
	glGetShaderiv(m_Vert, GL_COMPILE_STATUS, &compiled);
	CHECK_GL_ERROR();
	if (compiled) isCompiled = true;

    printShaderInfoLog(m_Vert);
	
    glCompileShader(m_Frag);
	CHECK_GL_ERROR();
	glGetShaderiv(m_Vert, GL_COMPILE_STATUS, &compiled);
	CHECK_GL_ERROR();
	if (!compiled) isCompiled = false;
    
    printShaderInfoLog(m_Frag);

	if(NULL != m_VertexShader) {	delete[] m_VertexShader;	m_VertexShader = NULL;}
	if(NULL != m_FragmentShader){	delete[] m_FragmentShader;	m_FragmentShader = NULL;}

	return isCompiled;
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

GLuint CZShader::getAttributeLocation(const char* atrrName)
{
	return glGetAttribLocation(m_Program, atrrName);
}

GLuint CZShader::getUniformLocation(const std::string& str)
{
	return m_uniforms[str];
}