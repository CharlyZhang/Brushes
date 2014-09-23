#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include "CZBrush.h"
#include "CZBrushPreview.h"
#include "CZShader.h"
#include "CFbo.h"
#include "CZSpiralGenerator.h"
#include "gl/GLAUX.H"			///< 为了载入图片纹理

#pragma comment(lib,"glaux.lib") 

CZShader *shader;
CFbo *fbo;
CZSpiralGenerator gen;

#if RENDER_PATH
CZBrush *brush = new CZBrush;
CZBrushPreview *priew = CZBrushPreview::getInstance();
#endif

int windowWidth = 600, windowHeight = 600;

GLuint mVertexBufferObject, mTexCoordBufferObject;

GLenum checkForError(char *loc);

GLuint		textureID=0;							// 存储一个纹理
AUX_RGBImageRec  *LoadBMP(char *Filename)					// 载入位图图象
{
	FILE *File=NULL;							// 文件句柄

	if (!Filename)								// 确保文件名已提供
	{
		return NULL;							// 如果没提供，返回 NULL
	}
	File=fopen(Filename,"r");
	if(File)								// 文件存在么?
	{
		fclose(File);							// 关闭句柄
		return auxDIBImageLoad(Filename); 				// 载入位图并返回指针
	}
	return NULL;
}
int LoadGLTextures(char *filename)					// 载入位图(调用上面的代码)并转换成纹理
{
	// 状态指示器
	AUX_RGBImageRec *TextureImage[1];					// 创建纹理的存储空间
	memset(TextureImage,0,sizeof(void *)*1);				// 将指针设为 NULL
	// 载入位图，检查有无错误，如果位图没找到则退出
	if (TextureImage[0]=LoadBMP(filename))
	{
		textureID++;						// 将 Status 设为 TRUE
		glGenTextures(1, &textureID);					// 创建纹理

		// 使用来自位图数据生成 的典型纹理
		glBindTexture(GL_TEXTURE_2D, textureID);
		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// 线形滤波
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// 线形滤波
	}
	return textureID;								// 返回 Status
}

void ShowTextureToScreen(int x,int y,int width,int height,GLuint texID)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(x,y,width,height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D,texID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  0.0f);
	glEnd();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}
//////////////////////////////////////////////////////////////////////////

void display(void)
{
	glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0,1.0,1.0,0.5);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	fbo->begin();
	shader->begin();
	
#if RENDER_RECT	
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,0,0);

	/// 绘制
	glDrawArrays(GL_TRIANGLE_STRIP,0,10);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#endif		/// < 绘制矩形纹理

#if RENDER_PATH
	priew->path->paint();	///< 绘制曲线轨迹
#endif

	shader->end();

	fbo->end();

	
	fbo->showTextureOnScreen(0,0,windowWidth,windowHeight);
	
    glutSwapBuffers();
    
    checkForError("swap");
}

void reshape(int width, int height)
{
  windowWidth = width; windowHeight = height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, (GLfloat) windowWidth, 0.0f, 
	  (GLfloat) windowHeight, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glViewport(0, 0, windowWidth, windowHeight);
}

void key(unsigned char key, int x, int y) 
{

	if (key == 27) 
	{
		delete shader;
		delete fbo;
		exit(0);
	}
}

void initGL()
{
	glClearColor(1.0,.95,.85,0.5);

	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
#if PIC_TEX
	int stru=LoadGLTextures("tex.bmp");
	if (stru==-1)
	{
		printf("spaceshiptexture.bmp not found: \n  press the Enter key to exit");
		getchar();
		exit (0);	
	}
	glEnable(GL_TEXTURE_2D);
#endif
#if BRUSH_TEX
	glClearColor(0,0,0,0);
	CFbo *f = gen.getStamp();
	glBindTexture(GL_TEXTURE_2D,f->getTexID());
#endif

#if RENDER_PATH
	priew->setBrush(brush);
	priew->previewWithSize(CZSize(windowWidth,windowHeight));	///< 初始化数据
	
	priew->path->shader = shader;
#endif

	glDisable(GL_TEXTURE_2D);
}

void initShader()
{
	 shader = new CZShader;

	if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
	{
		shader->readVertextShader("temp.vert");
		shader->readFragmentShader("temp.frag");
		shader->setShader();
	}

#if RENDER_RECT
	float vertices[] = {100,100,	200,100,	100,200,	200,200,
		200,200,	150,150,
		150,150,	300,150,	150,300,	300,300};
	float texcoord[] = {0,0,	1,0,	0,1,	1,1,
		1,1,	0,0,
		0,0,	1,0,	0,1,	1,1};

	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vertices, GL_STREAM_DRAW);

	glGenBuffers(1, &mTexCoordBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), texcoord, GL_STREAM_DRAW);
#endif

}

void initFBO()
{
	fbo = new CFbo(windowWidth, windowHeight,2);
	fbo->init();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  //  glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("Brushes");
  
  initShader();

  initFBO();

  initGL();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);

  glutMainLoop();

return 0;
}

GLenum checkForError(char *loc)
{
  GLenum errCode;
  const GLubyte *errString;

  if ((errCode = glGetError()) != GL_NO_ERROR)
    {
      errString = gluErrorString(errCode);
      printf("OpenGL error: %s",errString);

      if (loc != NULL)
	printf("(%s)",loc);

      printf("\n");
    }

  return errCode;
}

