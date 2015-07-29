
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

///
@interface CanvasView : UIView<UIGestureRecognizerDelegate>
{
    CZMat4 projMat;
    EAGLContext *context;               ///
}

@property (nonatomic, assign) CZPainting* ptrPainting;
@property (nonatomic, assign) CZFbo* fbo;

- (void)drawView;

@end

@implementation CanvasView

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
    self.autoresizingMask = UIViewAutoresizingNone;
    self.exclusiveTouch = YES;
    self.opaque = YES;
    self.backgroundColor = [UIColor colorWithWhite:0.95f alpha:1];
    
    // configure the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8,
                                    kEAGLDrawablePropertyColorFormat, nil];
    
    CGSize size = frame.size;
    projMat.SetOrtho(0,size.width, 0, size.height, -1.0f, 1.0f);
    
    glEnable(GL_TEXTURE_2D);        /// will emit gl error, but cause showing nothing if be deleted
    CZCheckGLError();
    glActiveTexture(GL_TEXTURE0);
    
    
    [self configureGestrues];
    
    return self;
}

- (void)dealloc
{
    [EAGLContext setCurrentContext:context];
    delete _fbo;
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [super dealloc];
}

#pragma mark - Properties

- (CZFbo*)fbo
{
    if (!_fbo && context) {
        [EAGLContext setCurrentContext:context];
        _fbo = new CZFbo;
    }
    
    return _fbo;
}

- (void)setPtrPainting:(CZPainting *)ptrPainting
{
    _ptrPainting = ptrPainting;
    if (_ptrPainting) {
        context = (EAGLContext*) ptrPainting->getGLContext()->getRealContext();
    }
    else
        context = nil;
    
}

#pragma mark - Geusture
- (void)configureGestrues
{
    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(handlePanGesture:)];
    panGesture.maximumNumberOfTouches = 1;
    panGesture.delegate = self;
    [self addGestureRecognizer:panGesture];
    
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(handleTapGesture:)];
    tapGesture.numberOfTapsRequired = 1;
    tapGesture.numberOfTouchesRequired = 1;
    [self addGestureRecognizer:tapGesture];
    
    [panGesture release];
    [tapGesture release];
}


- (void)handlePanGesture:(UIPanGestureRecognizer*)sender
{
    LOG_DEBUG("pan\t");
    
    CGPoint p = [sender locationInView:sender.view];
    p.y = self.bounds.size.height - p.y;
    
    if (sender.state == UIGestureRecognizerStateBegan) {
        CZActiveState::getInstance()->getActiveTool()->moveBegin(p.x,p.y);
    }
    else if (sender.state == UIGestureRecognizerStateChanged){
        CGPoint velocity = [sender velocityInView:sender.view];
        CZ2DPoint zeroPoint; CZ2DPoint v(velocity.x,velocity.y);
        float   speed = zeroPoint.distanceTo2DPoint(v) / 1000.0f; // pixels/millisecond
        LOG_DEBUG("speed is %f\n", speed);
        CZActiveState::getInstance()->getActiveTool()->moving(p.x, p.y, speed);
    }
    else if (sender.state == UIGestureRecognizerStateEnded){
        CZActiveState::getInstance()->getActiveTool()->moveEnd(p.x, p.y);
    }
    else if (sender.state == UIGestureRecognizerStateCancelled) {
        LOG_DEBUG("gesture canceled!\n");
    }
}

- (void)handleTapGesture:(UITapGestureRecognizer*)sender
{
    LOG_DEBUG("tap\n");
    
    CGPoint p = [sender locationInView:sender.view];
    p.y = self.bounds.size.height - p.y;
    
    if (CZActiveState::getInstance()->colorFillMode) {
        CZLayer *layer = self.ptrPainting->getActiveLayer();
        CZColor color = CZActiveState::getInstance()->getPaintColor();
        CZ2DPoint location = CZ2DPoint(p.x,p.y);
        layer->fill(color, location);
        [self drawView];
    }

}

#pragma mark - Draw
- (void) drawView
{
    if (!self.ptrPainting) {
        LOG_ERROR("ptrPainting is NULL!]\n");
        return ;
    }
    
    [EAGLContext setCurrentContext:context];
    self.fbo->begin();
    
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT );
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    self.ptrPainting->blit(projMat);
    
    glBindRenderbuffer(GL_RENDERBUFFER, self.fbo->getRenderBufferId());
    [context presentRenderbuffer:GL_RENDERBUFFER];
    LOG_DEBUG("drawView\n");
}

- (void)layoutSubviews
{
    CGSize size = self.bounds.size;
    projMat.SetOrtho(0,size.width, 0, size.height, -1.0f, 1.0f);
    [EAGLContext setCurrentContext:context];
    if(self.fbo)
        self.fbo->setRenderBufferWithContext((void*)context, (void*)self.layer);
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
    void setPaiting(CZPainting* p) { realView.ptrPainting = p;}
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
    p->setCanvas(this);
    CZActiveState::getInstance()->setPainting(p);
    view->setPaiting(p);
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

