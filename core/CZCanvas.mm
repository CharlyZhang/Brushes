
///  \file CZCanvas.cpp
///  \brief This is the file implements the class CZCanvas.
///
///	
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-15
///  \note

#include "CZCanvas.h"
#include "CZDefine.h"
#include "painting/CZPainting.h"
#include "CZActiveState.h"
#include "tool/CZFreehandTool.h"
#include "basic/CZMat4.h"
#include "graphic/glDef.h"

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

// const
GLfloat gVertexData[20] =
{
    // positionX, positionY, positionZ,     u,v
    0.0f, 50.0f, 0,        0.0f, 1.0f,
    0.0f, 0.0f, 0,         0.0f, 0.0f,
    50.0f, 0.0f, 0,        1.0f, 0.0f,
    50.0f, 50.0f, 0,       1.0f, 1.0f
};

///
@interface CanvasView : UIView<UIGestureRecognizerDelegate>
{
    CZFbo *fbo_;
    CZShader *shader;
    GLuint textures[10];
    CZMat4 mat;
    GLuint _vertexArray;
    GLuint _vertexBuffer;
}

@property (nonatomic, assign) EAGLContext *context;
@property (nonatomic, assign) CZFbo* fbo;

- (void)drawView;

@end

@implementation CanvasView

@synthesize fbo = fbo_;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (!self) {
        return nil;
    }
    
    self.multipleTouchEnabled = YES;
    self.contentMode = UIViewContentModeCenter;
    self.contentScaleFactor = [UIScreen mainScreen].scale;
    self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    self.exclusiveTouch = YES;
    self.opaque = YES;
    self.backgroundColor = [UIColor colorWithWhite:0.95f alpha:1];
    
    // configure the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8,
                                    kEAGLDrawablePropertyColorFormat, nil];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    //////////
    if (!self.context || ![EAGLContext setCurrentContext:self.context]) {
        [self release];
        return nil;
    }
    
    [self loadTexture];
    
    std::vector<std::string> attrs, uniforms;
    attrs.push_back("position");
    attrs.push_back("texcoord");
    uniforms.push_back("modelViewProjectionMatrix");
    uniforms.push_back("texture");
    shader = new CZShader("Shader","Shader",attrs,uniforms);
    
    CGSize size = self.bounds.size;
    mat.SetOrtho(0, size.width,0, size.height, -1, 1);
    
    glGenVertexArraysOES(1, &_vertexArray);
    glBindVertexArrayOES(_vertexArray);
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexData), gVertexData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5*sizeof(GLfloat), (void*)12);
    glBindVertexArrayOES(0);
    
    glEnable(GL_TEXTURE_2D);        /// will emit gl error, but cause showing nothing if be deleted
    CZCheckGLError();
    glActiveTexture(GL_TEXTURE0);
    
    
///////////
    
    [self configureGestrues];
    
    return self;
}

- (void)loadTexture {
    CGImageRef textureImage = [UIImage imageNamed:@"checkerplate.png"].CGImage;
    if (textureImage == nil) {
        NSLog(@"Failed to load texture image");
        return;
    }
    
    size_t texWidth = CGImageGetWidth(textureImage);
    size_t texHeight = CGImageGetHeight(textureImage);
    
    GLubyte *textureData = (GLubyte *)malloc(texWidth * texHeight * 4);
    
    CGContextRef textureContext = CGBitmapContextCreate(textureData,
                                                        texWidth, texHeight,
                                                        8, texWidth * 4,
                                                        CGImageGetColorSpace(textureImage),
                                                        kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(textureContext, CGRectMake(0.0, 0.0, (float)texWidth, (float)texHeight), textureImage);
    CGContextRelease(textureContext);
    
    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)texWidth, (GLsizei)texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    
    free(textureData);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    
 //   [self checkGLError:NO];
    
}

- (void)dealloc
{
    [EAGLContext setCurrentContext:self.context];
    delete self.fbo;
    delete shader;
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    [self.context release];
    
    [super dealloc];
}

#pragma mark - Properties

- (CZFbo*)fbo
{
    if (!fbo_ && self.context) {
        [EAGLContext setCurrentContext:self.context];
        fbo_ = new CZFbo;
    }
    
    return fbo_;
}

#pragma mark - Geusture
- (void)configureGestrues
{
    UIPanGestureRecognizer *panGesutre = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(handlePanGesture:)];
    panGesutre.maximumNumberOfTouches = 1;
    panGesutre.delegate = self;
    [self addGestureRecognizer:panGesutre];
}


- (void)handlePanGesture:(UIPanGestureRecognizer*)sender
{
    CGPoint p = [sender locationInView:sender.view];
    p.y = self.bounds.size.height - p.y;
    
    if (sender.state == UIGestureRecognizerStateBegan) {
        CZActiveState::getInstance()->getActiveTool()->moveBegin(p.x,p.y);
    }
    else if (sender.state == UIGestureRecognizerStateChanged){
        CZActiveState::getInstance()->getActiveTool()->moving(p.x, p.y, 1.0f);
    }
    else if (sender.state == UIGestureRecognizerStateEnded){
        CZActiveState::getInstance()->getActiveTool()->moveEnd(p.x, p.y);
    }
    else if (sender.state == UIGestureRecognizerStateCancelled) {
        LOG_DEBUG("gesture canceled!\n");
    }
    
}

#pragma mark - Draw
- (void) drawView
{
    [EAGLContext setCurrentContext:self.context];
    self.fbo->setRenderBufferWithContext((void*)self.context, (void*)self.layer);
    self.fbo->begin();
    
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT );
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    GLfloat data[20];
    CGSize size = self.bounds.size;
    GLfloat quad[8] =
    {
        0.0f, (GLfloat)size.height,
        0.0f, 0.0f,
        (GLfloat)size.width, 0.0f,
        (GLfloat)size.width, (GLfloat)size.height
    };
    
    // update the data
    for (int i=0; i<4; i++) {
        data[5*i+0] = quad[i*2+0];
        data[5*i+1] = quad[i*2+1];
        data[5*i+2] = gVertexData[5*i+2];
        data[5*i+3] = gVertexData[5*i+3];
        data[5*i+4] = gVertexData[5*i+4];
    }
    
#ifndef SHOW_FREE_DRAW
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glBindVertexArrayOES(_vertexArray);
    
    // Render the object again with ES2
    shader->begin();
    
    glUniformMatrix4fv(shader->getUniformLocation("modelViewProjectionMatrix"), 1, GL_FALSE, mat);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(shader->getUniformLocation("texture"), 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#else
    CZMat4 proj;
    proj.SetOrtho(0,size.width, 0, size.height, -1.0f, 1.0f);
    painting->blit(proj);
#endif
    
    glBindRenderbuffer(GL_RENDERBUFFER, self.fbo->getRenderBufferId());
    [self.context presentRenderbuffer:GL_RENDERBUFFER];
    
    return;
    [EAGLContext setCurrentContext:self.context];
    
    self.fbo->setColorRenderBuffer(self.bounds.size.width, self.bounds.size.height);
    
    
}

- (void)layoutSubviews {
    [EAGLContext setCurrentContext:self.context];
    [self drawView];
}

@end

/// CZView, 平台相关的视图
class CZViewImpl : public CZView
{
public:
    CanvasView *realView;
    
    CZViewImpl(const CZRect rect)
    {
        realView = [[CanvasView alloc]initWithFrame:CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height)];
    }
    
    ~CZViewImpl() { [realView release];}
    void setContext(void* ctx) { realView.context = (EAGLContext*)ctx;}
    void draw() { [realView drawView];}
};

/// implemention of CZCanvas
CZCanvas::CZCanvas(const CZRect rect)
{
    ptrPainting = NULL;
	view = new CZViewImpl(rect);
}

CZCanvas::~CZCanvas()
{
	delete view;
}

/// set painting
bool CZCanvas::setPaiting(CZPainting *p)
{
    if (!p) {
        LOG_ERROR("painting is NULL\n");
        return false;
    }
    
    ptrPainting = p;
    CZActiveState::getInstance()->getActiveTool()->ptrPainting = p;
    view->setContext(p->getGLContext()->getRealContext());
    return true;
}

/// 绘制视图
void CZCanvas::drawView()
{
//    if (!ptrPainting) {
//        LOG_ERROR("painting is NULL\n");
//        return;
//    }

    view->draw();
	//drawViewInRect(visibleRect);
}

void* CZCanvas::getView()
{
#if USE_OPENGL_ES
    CZViewImpl *thisView = (CZViewImpl*)view;
    return (void*)thisView->realView;
#endif
    return NULL;
}

/// 在一定区域绘制视图
void CZCanvas::drawViewInRect(CZRect &rect)
{

}

