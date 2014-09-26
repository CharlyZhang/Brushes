#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include "CZBrush.h"
#include "CZBrushPreview.h"
#include "CZTexture.h"
#include "CZFbo.h"
#include "CZSpiralGenerator.h"
#include "gl/GLAUX.H"			///< Ϊ������ͼƬ����
#include "CZUtil.h"				///< For checkPixels()

#pragma comment(lib,"glaux.lib") 

#if PATH_TEX || BRUSH_TEX
CZBrush *brush = new CZBrush(new CZSpiralGenerator);
CZBrushPreview *priew;// = CZBrushPreview::getInstance();
CZTexture *brushTex = 0;
#endif

int windowWidth = 600, windowHeight = 600;

GLuint mVertexBufferObject, mTexCoordBufferObject;
int verNum;
GLenum checkForError(char *loc);

GLuint		textureID=0;							// �洢һ������
AUX_RGBImageRec  *LoadBMP(char *Filename)					// ����λͼͼ��
{
	FILE *File=NULL;							// �ļ����

	if (!Filename)								// ȷ���ļ������ṩ
	{
		return NULL;							// ���û�ṩ������ NULL
	}
	File=fopen(Filename,"r");
	if(File)								// �ļ�����ô?
	{
		fclose(File);							// �رվ��
		return auxDIBImageLoad(Filename); 				// ����λͼ������ָ��
	}
	return NULL;
}
int LoadGLTextures(char *filename)					// ����λͼ(��������Ĵ���)��ת��������
{
	// ״ָ̬ʾ��
	AUX_RGBImageRec *TextureImage[1];					// ��������Ĵ洢�ռ�
	memset(TextureImage,0,sizeof(void *)*1);				// ��ָ����Ϊ NULL
	// ����λͼ��������޴������λͼû�ҵ����˳�
	if (TextureImage[0]=LoadBMP(filename))
	{
		textureID++;						// �� Status ��Ϊ TRUE
		glGenTextures(1, &textureID);					// ��������

		// ʹ������λͼ�������� �ĵ�������
		glBindTexture(GL_TEXTURE_2D, textureID);
		// ��������
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// �����˲�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// �����˲�
	}
	return textureID;								// ���� Status
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
#if BRUSH_TEX
	glClearColor(0.0,0.0,0.0,1.0);
#else
	glClearColor(1.0,1.0,1.0,1.0);
#endif
    glClear(GL_COLOR_BUFFER_BIT);

	glColor4f(1.0,1.0,1.0,1.0);	///< ����Ƭ����ɫ�Ļ��
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,0,0);

	/// ����
	glDrawArrays(GL_TRIANGLE_STRIP,0,verNum);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
		exit(0);
	}
}

void initGL()
{
	glClearColor(1.0,.95,.85,0.5);

	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);

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
#if PATH_TEX || BRUSH_TEX
	priew = CZBrushPreview::getInstance();
	priew->setBrush(brush);
	brushTex = priew->previewWithSize(CZSize(windowWidth,windowHeight));
#endif
#if PATH_TEX
	glBindTexture(GL_TEXTURE_2D, brushTex->id);
#endif

#if RENDER_TWO_RECT
	float vertices[] = {100,100,	200,100,	100,200,	200,200,
		200,200,	150,150,
		150,150,	300,150,	150,300,	300,300};
	float texcoord[] = {0,0,	1,0,	0,1,	1,1,
		1,1,	0,0,
		0,0,	1,0,	0,1,	1,1};

	verNum = 10;
#endif

#if RENDER_FULL_RECT
	float vertices[] = {0,0,	windowWidth,0,	0,windowHeight,	windowWidth,windowHeight};
	float texcoord[] = {0,0,	1,0,	0,1,	1,1};

	verNum = 4;
#endif

	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 2*verNum * sizeof(float), vertices, GL_STREAM_DRAW);

	glGenBuffers(1, &mTexCoordBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 2*verNum * sizeof(float), texcoord, GL_STREAM_DRAW);

	glDisable(GL_TEXTURE_2D);
}


int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  //  glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("Brushes");

  glewInit();
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

