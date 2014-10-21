#include <windows.h>	// Header File For Windows
#include "GL/glew.h"
#include "GL/glut.h"
#include "CZBrush.h"
#include "CZBrushPreview.h"
#include "CZTexture.h"
#include "CZFbo.h"
#include "CZSpiralGenerator.h"
#include "gl/GLAUX.H"			///< Ϊ������ͼƬ����
#include "CZUtil.h"				///< For checkPixels()
#include "CZFreehandTool.h"
#include "stdio.h"				///< for freopen

static HGLRC           hRC=NULL;                           // ������ɫ��������  
static HDC             hDC=NULL;                           // OpenGL��Ⱦ��������  
static HWND            hWnd=NULL;                          // �������ǵĴ��ھ��  
static HINSTANCE       hInstance;                          // ��������ʵ�� 

BOOL	keys[256];			// Array Used For The Keyboard Routine
bool    active = TRUE;      // ���ڵĻ��־��ȱʡΪTRUE  
bool    fullscreen = FALSE;  // ȫ����־ȱʡ��ȱʡ�趨��ȫ��ģʽ (ȫ����ʾ�ñ�֤�����Ϊ4:3)

#pragma comment(lib,"glaux.lib") 

//////////////////////////////////////////////////////////////////////////

#if PATH_TEX || BRUSH_TEX
CZBrush *brush = new CZBrush(new CZSpiralGenerator);
CZBrushPreview *priew;// = CZBrushPreview::getInstance();
CZTexture *brushTex = 0;
#endif
FILE *fp1 = freopen("../info.txt","w",stdout);
FILE *fp2 = freopen("../error.txt","w",stderr);

#if RENDER_FREEHAND
	CZFreehandTool *freeHand = NULL;		///! �����ȫ�ֱ��������ܵ���glew�ĳ�ʼ����gl��ʼ��֮ǰ
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

bool InitGL(GLsizei Width, GLsizei Height)	// This Will Be Called Right After The GL Window Is Created
{
	glClearColor(1.0,.95,.85,0.5);

	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);

	glewInit();

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
#endif
#if BRUSH_TEX
	brushTex = priew->getBrushTexture();
	glBindTexture(GL_TEXTURE_2D, brushTex->id);
#endif
#if PATH_TEX
	CZImage *img = priew->previewWithSize(CZSize(windowWidth,windowHeight));
	brushTex = CZTexture::produceFromImage(img);
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

#if RENDER_FULL_RECT || RENDER_FREEHAND
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

#if RENDER_FREEHAND
	freeHand = new CZFreehandTool;
#endif

	return true;
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	windowWidth = Width; windowHeight = Height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) windowWidth, 0.0f, 
		(GLfloat) windowHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, windowWidth, windowHeight);
}

GLvoid DrawGLScene(GLvoid)
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

#if RENDER_FREEHAND
	glBindTexture(GL_TEXTURE_2D,freeHand->texture->id);
#endif

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

	//glutSwapBuffers();

	checkForError("swap");
}

//////////////////////////////////////////////////////////////////////////
GLvoid KillGLWindow(GLvoid);
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag);

/// ��Ϣ������
LRESULT CALLBACK WndProc(	HWND	hWnd,
	UINT	message,
	WPARAM	wParam,
	LPARAM	lParam)
{
	POINT mousePos;

	switch (message)						// Tells Windows We Want To Check The Message
	{
	case WM_ACTIVATE:						// ���Ӵ��ڼ�����Ϣ
		if (!HIWORD(wParam))					// �����С��״̬
			active=TRUE;					// �����ڼ���״̬
		else
			active=FALSE;					// �����ټ���
		break;

	case WM_DESTROY:
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_QUIT:
		KillGLWindow();
		break;

	case WM_KEYDOWN:
		keys[wParam] = TRUE;
		break;

	case WM_KEYUP:
		keys[wParam] = FALSE;
		break;

	case WM_LBUTTONDOWN:
		GetCursorPos(&mousePos);
#if RENDER_FREEHAND
		freeHand->moveBegin(CZ2DPoint(mousePos.x,windowHeight-mousePos.y));
#endif
		break;

	case WM_MOUSEMOVE:
		GetCursorPos(&mousePos);
#if RENDER_FREEHAND
		if (wParam & MK_LBUTTON)
			freeHand->moving(CZ2DPoint(mousePos.x,windowHeight-mousePos.y),1.0);
#endif
		break;

	case WM_LBUTTONUP:
#if RENDER_FREEHAND
		GetCursorPos(&mousePos);
		freeHand->moveEnd(CZ2DPoint(mousePos.x,windowHeight-mousePos.y));
#endif
		break;

	case WM_SIZE:
		ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
		break;

	default:
		return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return (0);
};

/// �������ٴ���
GLvoid KillGLWindow(GLvoid)
{
	/// ȫ�������е�����
	if (fullscreen)
	{
		ChangeDisplaySettings(NULL,0);          // �ǵĻ����л�������  
		ShowCursor(TRUE);                       // ��ʾ���ָ��  
	}
	/// ����OpenGL��Ⱦ������
	if (hRC)									  
	{ 
		if (!wglMakeCurrent(NULL,NULL))         // �����ܷ��ͷ�DC��RC������?
			//wglMakeCurrent(hDC,NULL);
			MessageBox(NULL,"�ͷ�DC��RCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);

		if (!wglDeleteContext(hRC))             // �����ܷ�ɾ��RC?
			MessageBox(NULL,"�ͷ�RCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);  

		hRC=NULL;								// ��RC��Ϊ NULL
	}
	/// ����DC
	if (hDC && !ReleaseDC(hWnd,hDC))            
	{  
		MessageBox(NULL,"�ͷ�DCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);  
		hDC=NULL;								// �� DC ��Ϊ NULL  
	}   
	/// ���ٴ���
	if (hWnd && !DestroyWindow(hWnd))					
	{
		MessageBox(NULL,"�ͷŴ��ھ��ʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;								// �� hWnd ��Ϊ NULL
	}
	/// ע����
	if (!UnregisterClass("OpenGL WinClass",hInstance))				
	{
		MessageBox(NULL,"����ע�������ࡣ","�رմ���",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							// �� hInstance ��Ϊ NULL
	}
}

/// ��������
///		\param title - ���ڱ���
///		\param bits	 - ��ɫ���ݴ洢λ 32
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	fullscreen = fullscreenflag;								// ����ȫ��ȫ����־ 

	hInstance			= GetModuleHandle(NULL);				// ȡ�����Ǵ��ڵ�ʵ��

	/// �л���ȫ����ʾģʽ
	if (fullscreen)                               
	{  
		DEVMODE dmScreenSettings;								// �豸ģʽ
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// ȷ���ڴ����Ϊ��
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Devmode �ṹ�Ĵ�С
		dmScreenSettings.dmPelsWidth	= width;				// ��ѡ��Ļ���
		dmScreenSettings.dmPelsHeight	= height;				// ��ѡ��Ļ�߶�
		//dmScreenSettings.dmBitsPerPel	= bits;					// ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields		= /*DM_BITSPERPEL|*/DM_PELSWIDTH|DM_PELSHEIGHT;

		// ����������ʾģʽ�����ؽ����ע: CDS_FULLSCREEN ��ȥ��״̬����  
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)  
		{  
			// ��ģʽʧ�ܣ��ṩ����ѡ��˳����ڴ��������С�  
			if (MessageBox(NULL,"ȫ��ģʽ�ڵ�ǰ�Կ�������ʧ�ܣ�\nʹ�ô���ģʽ��","NeHe G",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)  
			{  
				fullscreen=FALSE;								// ѡ�񴰿�ģʽ(Fullscreen=FALSE)  
			}  
			else
			{  
				// ����һ���Ի��򣬸����û��������  
				MessageBox(NULL,"���򽫱��ر�","����",MB_OK|MB_ICONSTOP);  
				return FALSE;									//  �˳������� FALSE  
			}  
		}  
	}

	/// ���崰����
	WNDCLASS			wc;										// ������ṹ 
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// �ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc������Ϣ
	wc.cbClsExtra		= 0;									// �޶��ⴰ������
	wc.cbWndExtra		= 0;									// �޶��ⴰ������
	wc.hInstance		= hInstance;							// ����ʵ��
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// װ��ȱʡͼ��
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// װ�����ָ��
	wc.hbrBackground	= NULL;									// GL����Ҫ����
	wc.lpszMenuName		= NULL;									// ����Ҫ�˵�
	wc.lpszClassName	= "OpenGL WinClass";					// �趨������
	/// ����ע�ᴰ����
	if(!RegisterClass(&wc))
	{
		MessageBox(0,"Failed To Register The Window Class.","Error",MB_OK|MB_ICONERROR);
		return FALSE;
	}

	/// ��չ���
	DWORD       dwExStyle;							// ��չ���ڷ��  
	DWORD       dwStyle;							// ���ڷ�� 
	if (fullscreen)                               
	{  
		dwExStyle=WS_EX_APPWINDOW;								// ��չ�����񣨴�����ǰ��  
		dwStyle=WS_POPUP;										// ������û�б߿�  
		ShowCursor(FALSE);										// �������ָ��  
	}  
	else  
	{  
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // ��չ��������ǿ3D�У� 
		dwStyle=WS_OVERLAPPEDWINDOW;							// �����񣨴��������ͱ߿� 
	}  

	/// �������ڴﵽ����Ҫ��Ĵ�С
	RECT WindowRect;								// ȡ�þ��ε����ϽǺ����½ǵ�����ֵ  
	WindowRect.left=(long)0;                        // ��Left   ��Ϊ 0  
	WindowRect.right=(long)width;                   // ��Right  ��ΪҪ��Ŀ��  
	WindowRect.top=(long)0;                         // ��Top    ��Ϊ 0  
	WindowRect.bottom=(long)height;                 // ��Bottom ��ΪҪ��ĸ߶�

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle); 

	/// ��������
	hWnd = CreateWindowEx(
		dwExStyle,										// ��չ������
		"OpenGL WinClass",
		title,											// Title Appearing At The Top Of The Window

		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS |
		dwStyle,

		0, 0,											// The Position Of The Window On The Screen
		WindowRect.right-WindowRect.left,				// ��������õĴ��ڿ��  
		WindowRect.bottom-WindowRect.top,				// ��������õĴ��ڸ߶� 

		NULL,											// �޸�����  
		NULL,											// �޲˵�  
		hInstance,										// ʵ��  
		NULL);											// ����WM_CREATE�����κζ���;

	if(!hWnd)
	{
		KillGLWindow();									// ������ʾ��  
		MessageBox(0,"Window Creation Error.","Error",MB_OK|MB_ICONERROR);
		return FALSE;
	}

	/// ���ô��ڵĻ�������
	GLuint  PixelFormat;                    // �������ƥ��Ľ�� 
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),		// Size Of This Pixel Format Descriptor
		1,									// Version Number (?)
		PFD_DRAW_TO_WINDOW |				// Format Must Support Window
		PFD_SUPPORT_OPENGL |				// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,					// Must Support Double Buffering
		PFD_TYPE_RGBA,						// Request An RGBA Format
		bits,								// Select A 16Bit Color Depth
		0, 0, 0, 0, 0, 0,					// Color Bits Ignored (?)
		0,									// No Alpha Buffer
		0,									// Shift Bit Ignored (?)
		0,									// No Accumulation Buffer
		0, 0, 0, 0,							// Accumulation Bits Ignored (?)
		16,									// 16Bit Z-Buffer (Depth Buffer)  
		0,									// No Stencil Buffer
		0,									// No Auxiliary Buffer (?)
		PFD_MAIN_PLANE,						// Main Drawing Layer
		0,									// Reserved (?)
		0, 0, 0								// Layer Masks Ignored (?)
	};
	/// ȡ���豸������
	if (!(hDC=GetDC(hWnd)))							
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"���ܴ���һ����ƥ������ظ�ʽ","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
	/// Finds The Closest Match To The Pixel Format We Set Above
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(0,"Can't Find A Suitable PixelFormat.","Error",MB_OK|MB_ICONERROR);
		return FALSE;							// ���� FALSE
	}
	/// �������ظ�ʽ
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))				
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(0,"Can't Set The PixelFormat.","Error",MB_OK|MB_ICONERROR);
		return FALSE;							// ���� FALSE
	}
	/// ȡ����ɫ������
	if (!(hRC=wglCreateContext(hDC)))					
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(0,"Can't Create A GL Rendering Context.","Error",MB_OK|MB_ICONERROR);
		return FALSE;							// ���� FALSE
	}
	/// ���Լ�����ɫ������
	if(!wglMakeCurrent(hDC,hRC))						
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(0,"Can't activate GLRC.","Error",MB_OK|MB_ICONERROR);
		return FALSE;							// ���� FALSE
	}

	//GetClientRect(hWnd, &Screen);

	ShowWindow(hWnd,SW_SHOW);					// ��ʾ����
	UpdateWindow (hWnd) ;
	SetForegroundWindow(hWnd);					// ����������ȼ�
	SetFocus(hWnd);								// ���ü��̵Ľ������˴���
	ReSizeGLScene(width, height);				// ����͸�� GL ��Ļ

	if (!InitGL(width, height))					// ��ʼ���½���GL����
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

	return TRUE;
}

/// ������
int WINAPI WinMain(	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPSTR		lpCmdLine, 
	int			nCmdShow)
{
	MSG			msg;		// Windows Message Structure

	/// ��ʾ�û�ѡ������ģʽ
	//if (MessageBox(NULL,"������ȫ��ģʽ������ô��", "����ȫ��ģʽ",MB_YESNO|MB_ICONQUESTION)==IDNO) fullscreen=FALSE;						// FALSEΪ����ģʽ

	//glewInit();

	/// ����OpenGL����  
	if (!CreateGLWindow("Brushes",windowWidth,windowHeight,16,fullscreen))  return 0;

	while (1)
	{
		// Process All Messages
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return TRUE;
			}
		}

		if(active)
		{
			if (keys[VK_ESCAPE])
			{
				glDeleteBuffers(1,&mVertexBufferObject);
				glDeleteBuffers(1,&mTexCoordBufferObject);
#if RENDER_FREEHAND
				if(freeHand) {	delete freeHand; freeHand = NULL;}
#endif
				break;
			}
			else
			{
				DrawGLScene();
				SwapBuffers(hDC);
			}

			if (keys[VK_F1])                    // F1��������ô?  
			{  
				keys[VK_F1]=FALSE;              // ���ǣ�ʹ��Ӧ��Key�����е�ֵΪ FALSE  
				KillGLWindow();                 // ���ٵ�ǰ�Ĵ���  
				fullscreen =! fullscreen;             // �л� ȫ�� / ���� ģʽ  

				// �ؽ� OpenGL ����  
				if (!CreateGLWindow("Brushes",windowWidth,windowHeight,16,fullscreen))  return 0;               // �������δ�ܴ����������˳� 
			}  
		}

	}

	/// �رճ���  
	KillGLWindow();                             // ���ٴ���  
	return (msg.wParam);                        // �˳����� 
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

