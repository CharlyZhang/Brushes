//
//  TransformOverlayerView.m
//  HYDrawing
//
//  Created by CharlyZhang on 15/11/19.
//  Copyright © 2015年 Founder. All rights reserved.
//

#import "TransformOverlayerView.h"
#import "HYBrushCore.h"
#import "ZXHMockNavBar.h"


NSString *CZActiveLayerTransformChange = @"CZActiveLayerTransformChange";

@implementation TransformOverlayerView
{
    UIView *targetView;
    CGAffineTransform alignedTransform;
}

@synthesize alignedTransform;

- (instancetype) initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        
        self.opaque = NO;
        self.backgroundColor = [UIColor clearColor];
        
        targetView = [[UIView alloc]initWithFrame:frame];
        targetView.backgroundColor = [UIColor clearColor];
        [self addSubview:targetView];
        
        // Add Gestures
        UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(handlePanGesture:)];
        pan.delegate = self;
        [self addGestureRecognizer:pan];
        
        UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(handlePinchGesture:)];
        pinch.delegate = self;
        [self addGestureRecognizer:pinch];
        
        UIRotationGestureRecognizer *rotation = [[UIRotationGestureRecognizer alloc]initWithTarget:self action:@selector(handleRotationGesture:)];
        rotation.delegate = self;
        [self addGestureRecognizer:rotation];
        
        alignedTransform = CGAffineTransformIdentity;
        
        
        ZXHMockNavBar *navBar = [[ZXHMockNavBar alloc]initWithLeftBtnTitle:@"Cancel" title:@"编辑图层" rightBtnTitle:@"Accept"];
        
        [navBar.leftBtn addTarget:self action:@selector(cancelTransform) forControlEvents:UIControlEventTouchUpInside];
        [navBar.rightBtn addTarget:self action:@selector(acceptTransform) forControlEvents:UIControlEventTouchUpInside];
        
        [self addSubview:navBar];
        
    }
    
    return self;
}

#pragma mark 编辑完成

-(void)acceptTransform {
    [self removeFromSuperview];
    
    [[HYBrushCore sharedInstance] renderActiveLayerWithTransform:alignedTransform];
    
    [[HYBrushCore sharedInstance] setActiveLayerLinearInterprolation:NO];
    
    // 更新图层
    [self.delegate updateLayersView];
}

-(void)cancelTransform {
    [self removeFromSuperview];
    
    [[HYBrushCore sharedInstance] setActiveLayerTransform:CGAffineTransformIdentity];
    
    [[HYBrushCore sharedInstance] setActiveLayerLinearInterprolation:NO];
    // 更新图层
    [self.delegate updateLayersView];
}

#pragma mark - Gestures

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    return YES;
}

-(void)handlePanGesture:(UIPanGestureRecognizer*)pan
{
    if (pan.state == UIGestureRecognizerStateChanged) {
        CGPoint loc = [pan translationInView:self];
        [pan setTranslation:CGPointZero inView:self];
        targetView.center = CGPointMake(targetView.center.x+loc.x, targetView.center.y+loc.y);
        
        CGAffineTransform tX = CGAffineTransformIdentity;
        tX = CGAffineTransformTranslate(tX, loc.x, -loc.y);
        alignedTransform = CGAffineTransformConcat(alignedTransform, tX);
        
        NSDictionary *userInfo = @{@"a":@(alignedTransform.a),@"b":@(alignedTransform.b),@"c":@(alignedTransform.c),
                                   @"d":@(alignedTransform.d),@"tx":@(alignedTransform.tx),@"ty":@(alignedTransform.ty)};
        
        [[NSNotificationCenter defaultCenter]postNotificationName:CZActiveLayerTransformChange object:nil userInfo:userInfo];
    }
    
    if (pan.state == UIGestureRecognizerStateEnded) {
    }
    
}


-(void)handlePinchGesture:(UIPinchGestureRecognizer*)pinch {
    if (pinch.state == UIGestureRecognizerStateBegan) {
        
    }
    
    if (pinch.state == UIGestureRecognizerStateChanged) {
        targetView.transform = CGAffineTransformScale(targetView.transform, pinch.scale, pinch.scale);
        
        CGPoint pivot = targetView.center;
        
        CGAffineTransform tX = CGAffineTransformIdentity;
        
        pivot.y = self.bounds.size.height - pivot.y;
        tX = CGAffineTransformIdentity;
        tX = CGAffineTransformTranslate(tX, pivot.x, pivot.y);
        tX = CGAffineTransformScale(tX, pinch.scale, pinch.scale);
        tX = CGAffineTransformTranslate(tX, -pivot.x, -pivot.y);
        
        alignedTransform = CGAffineTransformConcat(alignedTransform, tX);
        
        pinch.scale = 1;
        
        NSDictionary *userInfo = @{@"a":@(alignedTransform.a),@"b":@(alignedTransform.b),@"c":@(alignedTransform.c),
                                   @"d":@(alignedTransform.d),@"tx":@(alignedTransform.tx),@"ty":@(alignedTransform.ty)};
        
        [[NSNotificationCenter defaultCenter]postNotificationName:CZActiveLayerTransformChange object:nil userInfo:userInfo];
    }
    
    if (pinch.state == UIGestureRecognizerStateEnded) {
    }
    
}



-(void)handleRotationGesture:(UIRotationGestureRecognizer*)rotation {
    
    if (rotation.state == UIGestureRecognizerStateBegan) {
        
    }
    
    if (rotation.state == UIGestureRecognizerStateChanged) {
        
        CGFloat angle = rotation.rotation;
        targetView.transform = CGAffineTransformRotate(targetView.transform, angle);
        
        CGPoint pivot = targetView.center;
        pivot.y = self.bounds.size.height - pivot.y;
        CGAffineTransform tX = CGAffineTransformIdentity;
        tX = CGAffineTransformTranslate(tX, pivot.x, pivot.y);
        tX = CGAffineTransformRotate(tX, M_PI-angle);
        tX = CGAffineTransformScale(tX, -1, -1);
        tX = CGAffineTransformTranslate(tX, -pivot.x, -pivot.y);
        alignedTransform = CGAffineTransformConcat(alignedTransform, tX);
        
        // 重置旋转角度
        rotation.rotation = 0;
        
        NSDictionary *userInfo = @{@"a":@(alignedTransform.a),@"b":@(alignedTransform.b),@"c":@(alignedTransform.c),
                                   @"d":@(alignedTransform.d),@"tx":@(alignedTransform.tx),@"ty":@(alignedTransform.ty)};
        
        [[NSNotificationCenter defaultCenter]postNotificationName:CZActiveLayerTransformChange object:nil userInfo:userInfo];
        
    }
    
    if (rotation.state == UIGestureRecognizerStateEnded) {
        
    }
}


@end
