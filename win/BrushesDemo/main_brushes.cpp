#include <windows.h>	// Header File For Windows
#include "glew.h"
#include "glut.h"
#include "brush/CZBrush.h"
#include "graphic/CZTexture.h"
#include "graphic/CZFbo.h"
#include "stamp/CZStampGenerator.h"
#include "CZUtil.h"				///< For checkPixels()
#include "tool/CZTool.h"
#include "CZActiveState.h"
#include "painting/CZPainting.h"
#include "CZCanvas.h"
#include "basic/CZRect.h"
#include "CZDefine.h"
#include "stdio.h"				///< for freopen

#if RENDER_IMGAGE
static char* BmpImageName = "tex.bmp";
GLuint textureID;
#endif

static HGLRC           hRC=NULL;                           // 窗口着色描述表句柄  
static HDC             hDC=NULL;                           // OpenGL渲染描述表句柄  
static HWND            hWnd=NULL;                          // 保存我们的窗口句柄  
static HINSTANCE       hInstance;                          // 保存程序的实例 

BOOL	keys[256];			// Array Used For The Keyboard Routine
bool    active = TRUE;      // 窗口的活动标志，缺省为TRUE  
bool    fullscreen = FALSE;  // 全屏标志缺省，缺省设定成全屏模式 (全屏显示得保证长宽比为4:3)

//////////////////////////////////////////////////////////////////////////

CZTexture *showTex = NULL;
CZImage *screenImg = NULL;

#if STAMP_TEX
CZStampGenerator *stampGen =  CZActiveState::getInstance()->getGenerator();
#endif

#if STROKE_TEX
CZBrush *brush = CZActiveState::getInstance()->getActiveBrush();
#endif

FILE *fp1 = freopen("../info.txt","w",stdout);
FILE *fp2 = freopen("../error.txt","w",stderr);

#if RENDER_FREEHAND
	CZPainting *painting  = NULL;
	CZCanvas *canvas = NULL;
#endif

int windowWidth = 600, windowHeight = 600;

GLuint mVertexBufferObject, mTexCoordBufferObject;
int verNum;
GLenum checkForError(char *loc);

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
	//glClearColor(0.0, 0.0, 1.0, 0.0);  
	//glClearStencil(0);  
	//glEnable(GL_STENCIL_TEST);  
	//return true;

	glClearColor(1.0,.95,.85,0.5);

	glDisable(GL_DITHER);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glewInit();

#if STAMP_TEX
	showTex = CZTexture::produceFromImage(stampGen->getStamp());
	glBindTexture(GL_TEXTURE_2D, showTex->texId);
#endif

#if STROKE_TEX
	CZImage *img = brush->previewImageWithSize(CZSize(windowWidth,windowHeight));
	showTex = CZTexture::produceFromImage(img);
	glBindTexture(GL_TEXTURE_2D, showTex->texId);
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
	painting = new CZPainting(CZSize(windowWidth,windowHeight));
	canvas = new CZCanvas(CZRect(0,0,windowHeight,windowHeight));
	canvas->setPaiting(painting);
#endif

#if RENDER_IMGAGE
	
	long width, height;
	width = height = 128;
	unsigned char *buf = NULL;
	CZUtil::loadBMP(BmpImageName,buf,width,height);

	CZImage *img = new CZImage((int)width,(int)height,RGB_BYTE,(void*)buf);
	painting->getActiveLayer()->renderImage(img,CZAffineTransform::makeFromTranslation(100,100));
	//showTex = new CZTexture(width,height,RGB_BYTE,(void*)buf);
	//glBindTexture(GL_TEXTURE_2D, showTex->texId);
	delete img;
	free(buf);

	painting->addNewLayer();
#endif
	return true;
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	//int w = Width; int h = Height;
	//glViewport(0, 0, w, h);  
	//float aspect = (w * 1.0) / h;  

	//glMatrixMode(GL_PROJECTION);  
	//glLoadIdentity();  
	//gluPerspective(60, aspect, 1, 100);  

	//glMatrixMode(GL_MODELVIEW);  
	//glLoadIdentity();  
	//return;

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
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  

	//glLoadIdentity();  
	//glTranslatef(0.0, 0.0, -20.0);  

	////glStencilFunc(GL_ALWAYS, 0, 0x00);  
	//glStencilFunc(GL_NEVER, 0x0, 0x0);  
	//glStencilOp(GL_INCR, GL_INCR, GL_INCR);//  

	//glColor3f(1.0f, 1.0f, 1.0f);  

	//float dRadius = 5.0 * (sqrt(2.0) / 2.0);  
	//glBegin(GL_LINE_STRIP);  
	//for (float dAngel = 0; dAngel < 380.0; dAngel += 0.1)  
	//{  
	//	glVertex2d(dRadius * cos(dAngel), dRadius * sin(dAngel));  
	//	dRadius *= 1.003;  
	//}  
	//glEnd();  

	//glStencilFunc(GL_NOTEQUAL,0x1,0x1);  
	//glStencilOp(GL_INCR,GL_INCR,GL_INCR);//  

	//glColor3f(1.0f, 0.0f, 0.0f);  
	//glRectf(-5, -5, 5, 5); 
	//return ;

#if RENDER_FREEHAND &&0
	//painting->getRender()->drawViewInRect();	///!!! 这个绘制应该由tool调用paintPath时发notification调用
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	/// 为了模拟填充白色底板
	glClear(GL_COLOR_BUFFER_BIT);
	CZMat4 proj;
	proj.SetOrtho(0,windowWidth,0,windowHeight,-1.0f,1.0f);
	painting->blit(proj);

	// restore blending functions
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	canvas->isDirty = false;
	LOG_INFO("draw\n");
	return;
#endif
	delete screenImg;
	delete showTex;
	 screenImg = painting->imageWithSize(CZSize(windowWidth,windowHeight),&CZColor::whiteColor());
	showTex = CZTexture::produceFromImage(screenImg);
	glBindTexture(GL_TEXTURE_2D, showTex->texId);

#if STAMP_TEX
	glClearColor(0.0,0.0,0.0,1.0);
#else
	glClearColor(1.0,1.0,1.0,1.0);
#endif
	glClear(GL_COLOR_BUFFER_BIT);

	glColor4f(1.0,1.0,1.0,1.0);	///< 参与片段颜色的混合
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferObject);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,0,0);

	/// 绘制
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

/// 消息处理函数
LRESULT CALLBACK WndProc(	HWND	hWnd,
	UINT	message,
	WPARAM	wParam,
	LPARAM	lParam)
{
	POINT mousePos;

	switch (message)						// Tells Windows We Want To Check The Message
	{
	case WM_ACTIVATE:						// 监视窗口激活消息
		if (!HIWORD(wParam))					// 检查最小化状态
			active=TRUE;					// 程序处于激活状态
		else
			active=FALSE;					// 程序不再激活
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
		CZActiveState::getInstance()->getActiveTool()->moveBegin(CZ2DPoint(mousePos.x,windowHeight-mousePos.y));
		LOG_INFO("button down\n");
#endif
		break;

	case WM_MOUSEMOVE:
		GetCursorPos(&mousePos);
#if RENDER_FREEHAND
		if (wParam & MK_LBUTTON)
		{
			CZActiveState::getInstance()->getActiveTool()->moving(CZ2DPoint(mousePos.x,windowHeight-mousePos.y),1.0);
			LOG_INFO("move\n");
		}
#endif
		break;

	case WM_LBUTTONUP:
#if RENDER_FREEHAND
		GetCursorPos(&mousePos);
		CZActiveState::getInstance()->getActiveTool()->moveEnd(CZ2DPoint(mousePos.x,windowHeight-mousePos.y));
		LOG_INFO("button up\n");
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

/// 正常销毁窗口
GLvoid KillGLWindow(GLvoid)
{
	delete showTex;
#if RENDER_IMGAGE
	glDeleteTextures(1,&textureID);
#endif

	/// 全屏下先切到窗口
	if (fullscreen)
	{
		ChangeDisplaySettings(NULL,0);          // 是的话，切换回桌面  
		ShowCursor(TRUE);                       // 显示鼠标指针  
	}
	/// 销毁OpenGL渲染描述表
	if (hRC)									  
	{ 
		if (!wglMakeCurrent(NULL,NULL))         // 我们能否释放DC和RC描述表?
			//wglMakeCurrent(hDC,NULL);
			MessageBox(NULL,"释放DC或RC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);

		if (!wglDeleteContext(hRC))             // 我们能否删除RC?
			MessageBox(NULL,"释放RC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);  

		hRC=NULL;								// 将RC设为 NULL
	}
	/// 销毁DC
	if (hDC && !ReleaseDC(hWnd,hDC))            
	{  
		MessageBox(NULL,"释放DC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);  
		hDC=NULL;								// 将 DC 设为 NULL  
	}   
	/// 销毁窗口
	if (hWnd && !DestroyWindow(hWnd))					
	{
		MessageBox(NULL,"释放窗口句柄失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;								// 将 hWnd 设为 NULL
	}
	/// 注销类
	if (!UnregisterClass("OpenGL WinClass",hInstance))				
	{
		MessageBox(NULL,"不能注销窗口类。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							// 将 hInstance 设为 NULL
	}
}

/// 创建窗口
///		\param title - 窗口标题
///		\param bits	 - 颜色数据存储位 32
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	fullscreen = fullscreenflag;								// 设置全局全屏标志 

	hInstance			= GetModuleHandle(NULL);				// 取得我们窗口的实例

	/// 切换到全屏显示模式
	if (fullscreen)                               
	{  
		DEVMODE dmScreenSettings;								// 设备模式
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// 确保内存清空为零
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Devmode 结构的大小
		dmScreenSettings.dmPelsWidth	= width;				// 所选屏幕宽度
		dmScreenSettings.dmPelsHeight	= height;				// 所选屏幕高度
		//dmScreenSettings.dmBitsPerPel	= bits;					// 每象素所选的色彩深度
		dmScreenSettings.dmFields		= /*DM_BITSPERPEL|*/DM_PELSWIDTH|DM_PELSHEIGHT;

		// 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条。  
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)  
		{  
			// 若模式失败，提供两个选项：退出或在窗口内运行。  
			if (MessageBox(NULL,"全屏模式在当前显卡上设置失败！\n使用窗口模式？","NeHe G",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)  
			{  
				fullscreen=FALSE;								// 选择窗口模式(Fullscreen=FALSE)  
			}  
			else
			{  
				// 弹出一个对话框，告诉用户程序结束  
				MessageBox(NULL,"程序将被关闭","错误",MB_OK|MB_ICONSTOP);  
				return FALSE;									//  退出并返回 FALSE  
			}  
		}  
	}

	/// 定义窗口类
	WNDCLASS			wc;										// 窗口类结构 
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// 移动时重画，并为窗口取得DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc处理消息
	wc.cbClsExtra		= 0;									// 无额外窗口数据
	wc.cbWndExtra		= 0;									// 无额外窗口数据
	wc.hInstance		= hInstance;							// 设置实例
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// 装入缺省图标
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// 装入鼠标指针
	wc.hbrBackground	= NULL;									// GL不需要背景
	wc.lpszMenuName		= NULL;									// 不需要菜单
	wc.lpszClassName	= "OpenGL WinClass";					// 设定类名字
	/// 尝试注册窗口类
	if(!RegisterClass(&wc))
	{
		MessageBox(0,"Failed To Register The Window Class.","Error",MB_OK|MB_ICONERROR);
		return FALSE;
	}

	/// 扩展风格
	DWORD       dwExStyle;							// 扩展窗口风格  
	DWORD       dwStyle;							// 窗口风格 
	if (fullscreen)                               
	{  
		dwExStyle=WS_EX_APPWINDOW;								// 扩展窗体风格（窗口最前）  
		dwStyle=WS_POPUP;										// 窗体风格（没有边框）  
		ShowCursor(FALSE);										// 隐藏鼠标指针  
	}  
	else  
	{  
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // 扩展窗体风格（增强3D感） 
		dwStyle=WS_OVERLAPPEDWINDOW;							// 窗体风格（带标题栏和边框） 
	}  

	/// 调整窗口达到真正要求的大小
	RECT WindowRect;								// 取得矩形的左上角和右下角的坐标值  
	WindowRect.left=(long)0;                        // 将Left   设为 0  
	WindowRect.right=(long)width;                   // 将Right  设为要求的宽度  
	WindowRect.top=(long)0;                         // 将Top    设为 0  
	WindowRect.bottom=(long)height;                 // 将Bottom 设为要求的高度

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle); 

	/// 创建窗口
	hWnd = CreateWindowEx(
		dwExStyle,										// 扩展窗体风格
		"OpenGL WinClass",
		title,											// Title Appearing At The Top Of The Window

		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS |
		dwStyle,

		0, 0,											// The Position Of The Window On The Screen
		WindowRect.right-WindowRect.left,				// 计算调整好的窗口宽度  
		WindowRect.bottom-WindowRect.top,				// 计算调整好的窗口高度 

		NULL,											// 无父窗口  
		NULL,											// 无菜单  
		hInstance,										// 实例  
		NULL);											// 不向WM_CREATE传递任何东东;

	if(!hWnd)
	{
		KillGLWindow();									// 重置显示区  
		MessageBox(0,"Window Creation Error.","Error",MB_OK|MB_ICONERROR);
		return FALSE;
	}

	/// 设置窗口的绘制属性
	GLuint  PixelFormat;                    // 保存查找匹配的结果 
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
		8,									// No Stencil Buffer
		0,									// No Auxiliary Buffer (?)
		PFD_MAIN_PLANE,						// Main Drawing Layer
		0,									// Reserved (?)
		0, 0, 0								// Layer Masks Ignored (?)
	};
	/// 取得设备描述表
	if (!(hDC=GetDC(hWnd)))							
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能创建一种相匹配的像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
	/// Finds The Closest Match To The Pixel Format We Set Above
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				
	{
		KillGLWindow();							// 重置显示区
		MessageBox(0,"Can't Find A Suitable PixelFormat.","Error",MB_OK|MB_ICONERROR);
		return FALSE;							// 返回 FALSE
	}
	/// 设置象素格式
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))				
	{
		KillGLWindow();							// 重置显示区
		MessageBox(0,"Can't Set The PixelFormat.","Error",MB_OK|MB_ICONERROR);
		return FALSE;							// 返回 FALSE
	}
	/// 取得着色描述表
	if (!(hRC=wglCreateContext(hDC)))					
	{
		KillGLWindow();							// 重置显示区
		MessageBox(0,"Can't Create A GL Rendering Context.","Error",MB_OK|MB_ICONERROR);
		return FALSE;							// 返回 FALSE
	}
	/// 尝试激活着色描述表
	if(!wglMakeCurrent(hDC,hRC))						
	{
		KillGLWindow();							// 重置显示区
		MessageBox(0,"Can't activate GLRC.","Error",MB_OK|MB_ICONERROR);
		return FALSE;							// 返回 FALSE
	}

	//GetClientRect(hWnd, &Screen);

	ShowWindow(hWnd,SW_SHOW);					// 显示窗口
	UpdateWindow (hWnd) ;
	SetForegroundWindow(hWnd);					// 略略提高优先级
	SetFocus(hWnd);								// 设置键盘的焦点至此窗口
	ReSizeGLScene(width, height);				// 设置透视 GL 屏幕

	if (!InitGL(width, height))					// 初始化新建的GL窗口
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	return TRUE;
}

/// 主函数
int WINAPI WinMain(	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPSTR		lpCmdLine, 
	int			nCmdShow)
{
	MSG			msg;		// Windows Message Structure

	/// 提示用户选择运行模式
	//if (MessageBox(NULL,"你想在全屏模式下运行么？", "设置全屏模式",MB_YESNO|MB_ICONQUESTION)==IDNO) fullscreen=FALSE;						// FALSE为窗口模式

	//glewInit();

	/// 创建OpenGL窗口  
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
				if(canvas) { delete canvas; canvas = NULL; }
				if(painting) {  delete painting; painting = NULL;}
#endif
				CZActiveState::destroy();
				break;
			}
			else 
#if RENDER_FREEHAND
				if(canvas->isDirty)
#endif
			{
				DrawGLScene();
				SwapBuffers(hDC);
			}

			if (keys[VK_F1])                    // F1键按下了么?  
			{  
				keys[VK_F1]=FALSE;              // 若是，使对应的Key数组中的值为 FALSE  
				KillGLWindow();                 // 销毁当前的窗口  
				fullscreen =! fullscreen;             // 切换 全屏 / 窗口 模式  

				// 重建 OpenGL 窗口  
				if (!CreateGLWindow("Brushes",windowWidth,windowHeight,16,fullscreen))  return 0;               // 如果窗口未能创建，程序退出 
			} 

			if (keys['R'])
			{
				CZActiveState::getInstance()->setPaintColor(1,0,0,1);
				keys['R'] = false;
			}
			if (keys['G'])
			{
				CZActiveState::getInstance()->setPaintColor(0,1,0,1);
				keys['G'] = false;
			}
			if (keys['B'])
			{
				CZActiveState::getInstance()->setPaintColor(0,0,1,1);
				keys['B'] = false;
			}
			static int brushIdx = 0;
			if (keys['C'])
			{
				CZActiveState::getInstance()->setActiveBrush(brushIdx);
				brushIdx = (brushIdx+1)%2;
				keys['C'] = false;
			}
			static bool erase = false;
			if (keys['E'])
			{
				erase = !erase;
				CZActiveState::getInstance()->setEraseMode(erase);
				keys['E'] = false;
			}
			if (keys['T'])
			{
				CZLayer *layer = painting->getActiveLayer();
				layer->fill(CZColor(1,0,0,1),CZ2DPoint(300,300));
				canvas->isDirty = true;
				keys['T'] = false;
			}
#if RENDER_FREEHAND
			if (keys['S'])
			{
				painting->moveLayer(0,1);
				keys['S'] = false;
			}
#endif
		}

	}

	/// 关闭程序  
	KillGLWindow();                             // 销毁窗口  
	return (msg.wParam);                        // 退出程序 
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

