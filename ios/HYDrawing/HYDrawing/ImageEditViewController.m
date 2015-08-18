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



@implementation ImageEditViewController

{
    
    CGFloat imgW;
    
    CGFloat initialAngle_;
    
}



// 隐藏状态栏

-(BOOL)prefersStatusBarHidden{
    
    return YES;
    
}



- (NSArray *)constrainSubview:(UIView *)subview toMatchWithSuperview:(UIView *)superview

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
    
    
    
    return constraints;
    
}





-(void)viewDidLoad{
    
    
    
    _imageView = [[UIImageView alloc]init];
    
    //_imageView.frame = CGRectMake(0, 0, _originalImg.size.width/2, _originalImg.size.height/2);
    
    [self.view addSubview:_imageView];
    
    _imageView.contentMode = UIViewContentModeCenter;
    
    [self constrainSubview:_imageView toMatchWithSuperview:self.view];
    
    
    
    [super viewDidLoad];
    
    
    
    if (_originalImg) {
        
        _imageView.image = _originalImg;
        
        
        
        // 添加手势
        
        UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchImageing:)];
        
        pinch.delegate = self;
        
        UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(panImageing:)];
        
        pan.delegate = self;
        
        UIRotationGestureRecognizer *rotation = [[UIRotationGestureRecognizer alloc]initWithTarget:self action:@selector(rotatingImage:)];
        
        rotation.delegate = self;
        
        self.view.gestureRecognizers = @[pan,rotation,pinch];
        
    }
    
    
    
    self.imageTransform = CGAffineTransformIdentity;
    
    
    
    // 模拟导航栏
    
    ZXHMockNavBar *navBar = [[ZXHMockNavBar alloc]initWithLeftBtnTitle:@"Cancel" title:@"插入图片" rightBtnTitle:@"Accept"];
    
    [navBar.leftBtn addTarget:self action:@selector(backHome) forControlEvents:UIControlEventTouchUpInside];
    
    [navBar.rightBtn addTarget:self action:@selector(toDrawingPage) forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:navBar];
    
}



#pragma mark 图片的旋转、缩放、移动

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer{
    
    return YES;
    
}



-(void)panImageing:(UIPanGestureRecognizer*)pan{
    
    
    
    if (pan.state == UIGestureRecognizerStateChanged) {
        
        CGPoint loc = [pan translationInView:self.view];
        
        [pan setTranslation:CGPointZero inView:self.view];
        
        self.imageView.center = CGPointMake(self.imageView.center.x+loc.x, self.imageView.center.y+loc.y);
        
        
        
        CGAffineTransform tX = CGAffineTransformIdentity;
        
        tX = CGAffineTransformTranslate(tX, loc.x, -loc.y);
        
        self.imageTransform = CGAffineTransformConcat(self.imageTransform, tX);
        
    }
    
    
    
}



-(void)pinchImageing:(UIPinchGestureRecognizer*)pinch{
    
    
    
    if (pinch.state == UIGestureRecognizerStateBegan || pinch.state == UIGestureRecognizerStateChanged) {
        
        NSLog(@"%f",pinch.scale);
        
        self.imageView.transform = CGAffineTransformScale(self.imageView.transform, pinch.scale, pinch.scale);
        
        
        
        
        
        CGPoint pivot = self.imageView.center;
        
        pivot.y = self.view.bounds.size.height - pivot.y;
        
        CGAffineTransform tX = CGAffineTransformIdentity;
        
        tX = CGAffineTransformTranslate(tX, pivot.x, pivot.y);
        
        tX = CGAffineTransformScale(tX, pinch.scale, pinch.scale);
        
        tX = CGAffineTransformTranslate(tX, -pivot.x, -pivot.y);
        
        self.imageTransform = CGAffineTransformConcat(self.imageTransform, tX);
        
        
        
        pinch.scale = 1;
        
    }
    
}



-(void)rotatingImage:(UIRotationGestureRecognizer*)gesture{
    
    
    
    if (gesture.state == UIGestureRecognizerStateBegan) {
        
        initialAngle_ = [(UIRotationGestureRecognizer *)gesture rotation];
        
    }
    
    
    
    if (gesture.state == UIGestureRecognizerStateChanged) {
        
        float rotation  = [(UIRotationGestureRecognizer *)gesture rotation];
        
        float angle = rotation - initialAngle_;
        
        self.imageView.transform = CGAffineTransformRotate(self.imageView.transform , angle);
        
        
        
        CGPoint pivot = self.imageView.center;
        
        pivot.y = self.view.bounds.size.height - pivot.y;
        
        CGAffineTransform tX = CGAffineTransformIdentity;
        
        tX = CGAffineTransformTranslate(tX, pivot.x, pivot.y);
        
        tX = CGAffineTransformRotate(tX, M_PI-angle);
        
        tX = CGAffineTransformScale(tX, -1, -1);
        
        tX = CGAffineTransformTranslate(tX, -pivot.x, -pivot.y);
        
        self.imageTransform = CGAffineTransformConcat(self.imageTransform, tX);
        
        
        
        initialAngle_ = rotation;
        
    }
    
}



#pragma mark 编辑完成

-(void)toDrawingPage{
    
    // 显示导航栏
    
    UINavigationController *nav = (UINavigationController *)[UIApplication sharedApplication].keyWindow.rootViewController;
    
    nav.navigationBar.hidden = NO;
    
    [self.view removeFromSuperview];
    
    [[HYBrushCore sharedInstance]renderImage:_originalImg withTransform:self.imageTransform];
    
}



-(void)backHome{
    
    [self.view removeFromSuperview];
    
}



@end

