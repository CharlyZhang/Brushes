//
//  CanvasView.m
//  HYDrawing
//
//  Created by CharlyZhang on 15/8/15.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "CanvasView.h"
#include "painting/CZPainting.h"
#include "basic/CZMat4.h"

#include "CZActiveState.h"
#include "tool/CZFreehandTool.h"
#include "graphic/glDef.h"
#include "CZUtil.h"
#import "WDColor.h"

NSString *CZActivePaintColorDidChange = @"CZActivePaintColorDidChange";

////////////////////CanvasView////////////////////
@interface CanvasView()
{
    CZMat4 projMat;
    EAGLContext *context;               ///
    BOOL isBarVisible;
    
    /// canvas transform related
    CGPoint                 userSpacePivot;
    CGPoint                 deviceSpacePivot;
    float                   canvasScale;
    CZAffineTransform       canvasTransform;  /// from Painting Space to Device Space
}

@property (nonatomic, assign) CZFbo* fbo;
@property (nonatomic, assign) CZPainting* ptrPainting;

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
    
    // configure the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    eaglLayer.opaque = NO;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8,
                                    kEAGLDrawablePropertyColorFormat, nil];
    
    CGSize size = frame.size;
    projMat.SetOrtho(0,size.width, 0, size.height, -1.0f, 1.0f);
    
    glEnable(GL_TEXTURE_2D);        /// will emit gl error, but cause showing nothing if be deleted
    CZCheckGLError();
    glActiveTexture(GL_TEXTURE0);
    
    [self configureGestrues];
    
    isBarVisible = YES;
    
    canvasTransform = CZAffineTransform::makeIdentity();
    canvasScale = 1.f;
    deviceSpacePivot = CGPointMake(size.width / 2.f, size.height / 2.f);

    
    return self;
}

- (void)dealloc
{
    [EAGLContext setCurrentContext:context];
    delete _fbo;
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void) layoutSubviews
{
    CGSize size = self.bounds.size;
    projMat.SetOrtho(0,size.width, 0, size.height, -1.0f, 1.0f);
    [EAGLContext setCurrentContext:context];
    if(self.fbo)
        self.fbo->setRenderBufferWithContext((__bridge void*)context, (__bridge void*)self.layer);
    [self drawView];
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
    if (ptrPainting) {
        context = (__bridge EAGLContext*) ptrPainting->getGLContext()->getRealContext();
        
        CZSize size = ptrPainting->getDimensions();
        userSpacePivot = CGPointMake(size.width / 2.0f, size.height / 2.0f);
        
        [self rebuildViewTransform];
    }
    else {
        context = nil;
    }
    
    _ptrPainting = ptrPainting;
    
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
    
    UITapGestureRecognizer *doubleTapGesture = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(handleDoubleTapGesture:)];
    doubleTapGesture.numberOfTapsRequired = 2;
    doubleTapGesture.numberOfTouchesRequired = 1;
    [self addGestureRecognizer:doubleTapGesture];
    
    [tapGesture requireGestureRecognizerToFail:doubleTapGesture];

}


- (void)handlePanGesture:(UIPanGestureRecognizer*)sender
{
    LOG_DEBUG("pan\n");
    CZActiveState *activeState = CZActiveState::getInstance();
    
    if (activeState->colorFillMode || activeState->colorPickMode) return;
    
    if (self.ptrPainting->shouldPreventPaint()) {
        CZLayer *layer = self.ptrPainting->getActiveLayer();
        if (sender.state == UIGestureRecognizerStateBegan || sender.state == UIGestureRecognizerStateChanged) {
            if (!layer->isLocked())          [self.delegate showMessageView:kInvisible];
            else if(layer->isVisible())      [self.delegate showMessageView:kLocked];
            else                             [self.delegate showMessageView:kInvisibleAndLocked];
        }
        else {
            [self.delegate dismissMessageView];
        }

        return;
    }
    
    CGPoint p = [self transformToPainting:[sender locationInView:sender.view]];
    
    
    if (sender.state == UIGestureRecognizerStateBegan) {
        activeState->getActiveTool()->moveBegin(p.x,p.y);
    }
    else if (sender.state == UIGestureRecognizerStateChanged){
        CGPoint velocity = [sender velocityInView:sender.view];
        CZ2DPoint zeroPoint; CZ2DPoint v(velocity.x,velocity.y);
        float   speed = zeroPoint.distanceTo2DPoint(v) / 1000.0f; // pixels/millisecond
        //LOG_DEBUG("speed is %f\n", speed);
        activeState->getActiveTool()->moving(p.x, p.y, speed);
    }
    else if (sender.state == UIGestureRecognizerStateEnded){
        activeState->getActiveTool()->moveEnd(p.x, p.y);
    }
    else if (sender.state == UIGestureRecognizerStateCancelled) {
        LOG_DEBUG("gesture canceled!\n");
    }
}

- (void)handleTapGesture:(UITapGestureRecognizer*)sender
{
    LOG_DEBUG("tap\n");
    
    if (self.ptrPainting->shouldPreventPaint()) {
        CZLayer *layer = self.ptrPainting->getActiveLayer();
        if (!layer->isLocked())          [self.delegate showMessageView:kInvisible];
        else if(layer->isVisible())      [self.delegate showMessageView:kLocked];
        else                             [self.delegate showMessageView:kInvisibleAndLocked];
        if (sender.state == UIGestureRecognizerStateEnded) [self.delegate dismissMessageView];
        return;
    }
    
    CGPoint p = [self transformToPainting:[sender locationInView:sender.view]];
    
    CZActiveState *activeState = CZActiveState::getInstance();
    
    if (activeState->colorFillMode) {
        CZLayer *layer = self.ptrPainting->getActiveLayer();
        CZColor color = activeState->getPaintColor();
        CZ2DPoint location = CZ2DPoint(p.x,p.y);
        layer->fill(color, location);
        [self drawView];
    }
    else if (activeState->colorPickMode) {
        CZColor pickedColor = self.ptrPainting->pickColor(p.x, p.y);
        
        WDColor *pColor = [WDColor colorWithRed:pickedColor.red green:pickedColor.green blue:pickedColor.blue alpha:pickedColor.alpha];
        activeState->setPaintColor(pickedColor);
        NSDictionary *userInfo = @{@"pickedColor": pColor};
        [[NSNotificationCenter defaultCenter] postNotificationName:CZActivePaintColorDidChange object:nil userInfo:userInfo];
    }
    else {
        activeState->getActiveTool()->moveBegin(p.x, p.y);
        activeState->getActiveTool()->moveEnd(p.x, p.y);
    }
    
}

- (void)handleDoubleTapGesture:(UITapGestureRecognizer*)sender
{
    LOG_DEBUG("double tap\n");
    isBarVisible = !isBarVisible;
    [self.delegate displayToolView:isBarVisible];
}


- (void)setPainting:(void*)painting
{
    self.ptrPainting = (CZPainting*)painting;
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
    
    //glClearColor(1, 1, 1, 1);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT );
    
//    // setup projection matrix (orthographic)
//    mat4f_LoadOrtho(0, mainRegion.width / scale, 0, mainRegion.height / scale, -1.0f, 1.0f, proj);
//    mat4f_LoadCGAffineTransform(effectiveProj, canvasTransform_);
//
    CZMat4 effectiveProj;
    effectiveProj.LoadFromAffineTransform(canvasTransform);
    CZMat4 finalMat = projMat * effectiveProj;
    
    [self drawWhiteBackground: &finalMat];
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    self.ptrPainting->blit(finalMat);
    
    glBindRenderbuffer(GL_RENDERBUFFER, self.fbo->getRenderBufferId());
    [context presentRenderbuffer:GL_RENDERBUFFER];
    LOG_DEBUG("drawView\n");
}

- (void) drawWhiteBackground:(CZMat4*)proj
{
    if (!self.ptrPainting) {
        LOG_ERROR("ptrPainting is NULL!]\n");
        return ;
    }
    
    CZShader *shader = self.ptrPainting->getShader("simple");
    
    shader->begin();
    
    glUniformMatrix4fv(shader->getUniformLocation("mvpMat"), 1, GL_FALSE, *proj);
    glUniform4f(shader->getUniformLocation("color"), 1, 1, 1, 1);
    
    GL_BIND_VERTEXARRAY(self.ptrPainting->getQuadVAO());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    GL_BIND_VERTEXARRAY(0);
}

#pragma mark Private Method

- (void) rebuildViewTransform
{
    float screenScale = [UIScreen mainScreen].scale;
    
    CGAffineTransform transform = CGAffineTransformIdentity;
    transform = CGAffineTransformTranslate(transform, deviceSpacePivot.x, deviceSpacePivot.y);
    transform = CGAffineTransformScale(transform, canvasScale / screenScale, -canvasScale / screenScale);
    transform = CGAffineTransformTranslate(transform, -userSpacePivot.x, -userSpacePivot.y);
    
    transform.tx = roundf(transform.tx);
    transform.ty = roundf(transform.ty);
    
    canvasTransform = CZAffineTransform(transform.a, transform.b, transform.c, transform.d, transform.tx, transform.ty);
}

- (CGPoint) transformToPainting:(CGPoint)pt
{
    pt.y = self.bounds.size.height - pt.y;
    
    CGAffineTransform iTx = CGAffineTransformMake(canvasTransform.a, canvasTransform.b, canvasTransform.c, canvasTransform.d, canvasTransform.tx, canvasTransform.ty);
    iTx = CGAffineTransformInvert(iTx);
    CGPoint ret = CGPointApplyAffineTransform(pt, iTx);
    return ret;
}

@end

