//
//  ImageEditViewController.m
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/21.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ImageEditViewController.h"
#import "HYBrushCore.h"
#import "ZXHMockNavBar.h"
#import "Macro.h"

@implementation ImageEditViewController
{
    CGFloat imgW;
    CGFloat initialAngle_;
    UIView *testView;
    CGAffineTransform translateTransform;
}

// 隐藏状态栏
-(BOOL)prefersStatusBarHidden{
    return YES;
}

- (void)constrainSubview:(UIView *)subview toMatchWithSuperview:(UIView *)superview
{
    
    subview.translatesAutoresizingMaskIntoConstraints = NO;
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(subview);
    
    NSArray *constraints = [NSLayoutConstraint
                            constraintsWithVisualFormat:@"H:|[subview]|"
                            options:0
                            metrics:nil
                            views:viewsDictionary];
    constraints = [constraints arrayByAddingObjectsFromArray:
                   [NSLayoutConstraint
                    constraintsWithVisualFormat:@"V:|[subview]|"
                    options:0
                    metrics:nil
                    views:viewsDictionary]];
    [superview addConstraints:constraints];
    
    
}


-(void)viewDidLoad{
    [super viewDidLoad];
    
    _imageView = [[UIImageView alloc]init];
//    _imageView.backgroundColor = [UIColor greenColor];
    _imageView.frame = CGRectMake(0, 0, kScreenW, kScreenH);
    [self.view addSubview:_imageView];
    _imageView.contentMode = UIViewContentModeCenter;
//    [self constrainSubview:_imageView toMatchWithSuperview:self.view];
    
    self.view.backgroundColor = [UIColor whiteColor];
    if (_originalImg) {
        _imageView.image = _originalImg;
       // [_imageView setFrame:CGRectMake(512, 768/2, _originalImg.size.width, _originalImg.size.height)];
        
        // 添加手势
        UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchImageing:)];
        UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(panImageing:)];
        UIRotationGestureRecognizer *rotation = [[UIRotationGestureRecognizer alloc]initWithTarget:self action:@selector(rotatingImage:)];
        self.view.gestureRecognizers = @[pan,rotation,pinch];
    }

    self.imageTransform = CGAffineTransformIdentity;
    
    translateTransform = CGAffineTransformIdentity;

    ZXHMockNavBar *navBar = [[ZXHMockNavBar alloc]initWithFrame:CGRectMake(0, 0, kScreenW , 64) WithLeftBtnTitle:@"Cancel" title:@"插入图片" rightBtnTitle:@"Accept"];
    [navBar.leftBtn addTarget:self action:@selector(backHome) forControlEvents:UIControlEventTouchUpInside];
    [navBar.rightBtn addTarget:self action:@selector(toDrawingPage) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:navBar];
}

//
//-(void)viewWillLayoutSubviews{
//    NSLog(@"viewWillLayoutSubviews");
//}
#pragma mark 图片的旋转、缩放、移动
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer{
    if (([gestureRecognizer isMemberOfClass:[UIRotationGestureRecognizer class]] && [otherGestureRecognizer isMemberOfClass:[UIPinchGestureRecognizer class]]) ||
        ([gestureRecognizer isMemberOfClass:[UIPinchGestureRecognizer class]] && [otherGestureRecognizer isMemberOfClass:[UIRotationGestureRecognizer class]])) {
        return YES;
    }
    return NO;
}

-(void)panImageing:(UIPanGestureRecognizer*)pan{
    
    if (pan.state == UIGestureRecognizerStateBegan) {
//        CGPoint loc = CGPointMake(30, 50);
//        [pan setTranslation:CGPointZero inView:self.view];
//        
//        testView.center = CGPointMake(testView.center.x+loc.x, testView.center.y+loc.y);
//        self.imageView.center = CGPointMake(self.imageView.center.x+loc.x, self.imageView.center.y+loc.y);
//        
//        CGAffineTransform tX = CGAffineTransformIdentity;
//        tX = CGAffineTransformTranslate(tX, loc.x, -loc.y);
//        self.imageTransform = CGAffineTransformConcat(self.imageTransform, tX);

    }
    
    if (pan.state == UIGestureRecognizerStateChanged) {
        CGPoint loc = [pan translationInView:self.view];
        [pan setTranslation:CGPointZero inView:self.view];
        self.imageView.center = CGPointMake(self.imageView.center.x+loc.x, self.imageView.center.y+loc.y);
        
        CGAffineTransform tX = CGAffineTransformIdentity;
        tX = CGAffineTransformTranslate(tX, loc.x, -loc.y);
        self.imageTransform = CGAffineTransformConcat(self.imageTransform, tX);
    }
    
    if (pan.state == UIGestureRecognizerStateEnded) {
//        NSLog(@"pan imageView: %@",self.imageView);
//        NSLog(@"imageTransform: %@",NSStringFromCGAffineTransform(self.imageTransform));
    }
    
}

-(void)pinchImageing:(UIPinchGestureRecognizer*)pinch{
    if (pinch.state == UIGestureRecognizerStateBegan) {
//        CGFloat scale = 0.5;
//        
//        NSLog(@"pinch begin before transform: %@",self.imageView);
//        self.imageView.transform = CGAffineTransformScale(self.imageView.transform, scale, scale);
////        
////        UIView *newView = [[UIView alloc]initWithFrame:testView.frame];
////        newView.backgroundColor = [UIColor yellowColor];
////        newView.alpha = 0.5;
////        [self.view addSubview:newView];
////        testView.transform = CGAffineTransformScale(testView.transform, scale, scale);
//        
//        NSLog(@"pinch begin after transform: %@",self.imageView);
////        
////        float x = -self.imageView.bounds.size.width / 2.0f + self.imageView.center.x;
////        float y = -self.imageView.bounds.size.height / 2.0f + self.imageView.center.y;
////        float w = self.imageView.frame.size.width;
////        float h = self.imageView.frame.size.height;
////        self.imageView.frame = CGRectMake(x, y, w, h);
//        
//        CGPoint pivot = self.imageView.center;
//        NSLog(@"new center after pinch:%@",NSStringFromCGPoint(pivot));
//        pivot.y = self.view.bounds.size.height - pivot.y;
//        CGAffineTransform tX = CGAffineTransformIdentity;
//        tX = CGAffineTransformTranslate(tX, pivot.x, pivot.y);
//        tX = CGAffineTransformScale(tX, scale, scale);
//        tX = CGAffineTransformTranslate(tX, -pivot.x, -pivot.y);
//        self.imageTransform = CGAffineTransformConcat(self.imageTransform, tX);
//        pinch.scale = 1.0f;
    }
    
    if (pinch.state == UIGestureRecognizerStateChanged) {
        self.imageView.transform = CGAffineTransformScale(self.imageView.transform, pinch.scale, pinch.scale);
    
        CGPoint pivot = self.imageView.center;
        
        CGAffineTransform tX = CGAffineTransformIdentity;
        /// get the imageTransform for OpenGL
        pivot.y = self.view.bounds.size.height - pivot.y;
        tX = CGAffineTransformIdentity;
        tX = CGAffineTransformTranslate(tX, pivot.x, pivot.y);
        tX = CGAffineTransformScale(tX, pinch.scale, pinch.scale);
        tX = CGAffineTransformTranslate(tX, -pivot.x, -pivot.y);
        self.imageTransform = CGAffineTransformConcat(self.imageTransform, tX);
        
        pinch.scale = 1;
    }
    
    if (pinch.state == UIGestureRecognizerStateEnded) {
//        NSLog(@"pinch imageView: %@",self.imageView);
//        NSLog(@"imageTransform: %@",NSStringFromCGAffineTransform(self.imageTransform));
    }
}

-(void)rotatingImage:(UIRotationGestureRecognizer*)gesture{
    
//    if (gesture.state == UIGestureRecognizerStateBegan) {
//        initialAngle_ = [(UIRotationGestureRecognizer *)gesture rotation];
//    }
//    
//    if (gesture.state == UIGestureRecognizerStateChanged) {
//        float rotation  = [(UIRotationGestureRecognizer *)gesture rotation];
//        float angle = rotation - initialAngle_;
//        self.imageView.transform = CGAffineTransformRotate(self.imageView.transform , angle);
//        
//        CGPoint pivot = self.imageView.center;
//        pivot.y = self.view.bounds.size.height - pivot.y;
//        CGAffineTransform tX = CGAffineTransformIdentity;
//        tX = CGAffineTransformTranslate(tX, pivot.x, pivot.y);
//        tX = CGAffineTransformRotate(tX, M_PI-angle);
//        tX = CGAffineTransformScale(tX, -1, -1);
//        tX = CGAffineTransformTranslate(tX, -pivot.x, -pivot.y);
//        self.imageTransform = CGAffineTransformConcat(self.imageTransform, tX);
//        
//        initialAngle_ = rotation;
//    }
    
    if (gesture.state == UIGestureRecognizerStateBegan) {
//        CGFloat angle = 0.5;
//        self.imageView.transform = CGAffineTransformRotate(self.imageView.transform, angle);
//        
//        
//        CGPoint pivot = self.imageView.center;
//        pivot.y = self.view.bounds.size.height - pivot.y;
//        CGAffineTransform tX = CGAffineTransformIdentity;
//        tX = CGAffineTransformTranslate(tX, pivot.x, pivot.y);
//        tX = CGAffineTransformRotate(tX, M_PI-angle);
//        tX = CGAffineTransformScale(tX, -1, -1);
//        tX = CGAffineTransformTranslate(tX, -pivot.x, -pivot.y);
//        self.imageTransform = CGAffineTransformConcat(self.imageTransform, tX);
//        
//        
//        // 重置旋转角度
//        gesture.rotation = 0;

    }

    
    if (gesture.state == UIGestureRecognizerStateChanged) {
        CGFloat angle = gesture.rotation;
        self.imageView.transform = CGAffineTransformRotate(self.imageView.transform, angle);
        
        
        CGPoint pivot = self.imageView.center;
        pivot.y = self.view.bounds.size.height - pivot.y;
        CGAffineTransform tX = CGAffineTransformIdentity;
        tX = CGAffineTransformTranslate(tX, pivot.x, pivot.y);
        tX = CGAffineTransformRotate(tX, M_PI-angle);
        tX = CGAffineTransformScale(tX, -1, -1);
        tX = CGAffineTransformTranslate(tX, -pivot.x, -pivot.y);
        self.imageTransform = CGAffineTransformConcat(self.imageTransform, tX);
        
        
        // 重置旋转角度
        gesture.rotation = 0;
    }
    
    if (gesture.state == UIGestureRecognizerStateEnded) {
        //NSLog(@"rotate imageView: %@",self.imageView);
//        NSLog(@"imageTransform: %@",NSStringFromCGAffineTransform(self.imageTransform));
    }
}

#pragma mark 编辑完成
-(void)toDrawingPage{
    
    [self dismissAnimation];
    
    //[[HYBrushCore sharedInstance]renderImage:_originalImg withTransform:self.imageTransform newLayer:YES];
    [[HYBrushCore sharedInstance]renderBackground:_originalImg];
}

-(void)dismissAnimation{
    [UIView animateWithDuration:0.1 animations:^{
        self.view.alpha = 0;
    } completion:^(BOOL finished) {
        [self.view removeFromSuperview];
        // 显示导航栏
        UINavigationController *nav = (UINavigationController *)[UIApplication sharedApplication].keyWindow.rootViewController;
        nav.navigationBar.hidden = NO;
        nav.navigationBar.translucent = YES;
    }];
}

-(void)backHome{
    [self dismissAnimation];
}

@end
