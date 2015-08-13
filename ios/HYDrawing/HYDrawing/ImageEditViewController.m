//
//  ImageEditViewController.m
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/21.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ImageEditViewController.h"

@implementation ImageEditViewController
{
    CGFloat imgW;
    float initialScale_;
    float initialAngle_;
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
    // 右侧navItem
    UIBarButtonItem *doneBtn = [[UIBarButtonItem alloc]initWithTitle:@"完成" style:UIBarButtonItemStylePlain target:self action:@selector(toDrawingPage)];
    self.navigationItem.rightBarButtonItem = doneBtn;

    _imageView = [[UIImageView alloc]initWithImage:_originalImg];
    [self.view addSubview:_imageView];
    _imageView.contentMode = UIViewContentModeCenter;
    [self constrainSubview:_imageView toMatchWithSuperview:self.view];
    
    _imageView.backgroundColor = [UIColor clearColor];
    self.view.backgroundColor = [UIColor redColor];
    _imageView.image = [UIImage imageNamed:@"new_canvas"];
    [super viewDidLoad];
    
    if (_originalImg) {
        _imageView.image = _originalImg;
        //_imageView.userInteractionEnabled = YES;
        
        // 添加手势
        UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchImageing:)];
        UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(panImageing:)];
        pinch.delegate = self;
        UIRotationGestureRecognizer *rotation = [[UIRotationGestureRecognizer alloc]initWithTarget:self action:@selector(rotatingImage:)];
        self.view.gestureRecognizers = @[pan,rotation,pinch];
    }
    
    // 记住原图大小
    imgW = _originalImg.size.width;
    
    // 变换默认值
    _scale = 1;
    _position = CGPointMake(0, 0);
    _angle = 0;
    
    self.imageTransform = CGAffineTransformIdentity;
    
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

#pragma mark 编辑完成，传递参数，回到画图页面
-(void)toDrawingPage{

    CGFloat w = _imageView.frame.size.width;
    CGFloat h = _imageView.bounds.size.height;
    
    self.position = CGPointMake(_imageView.frame.origin.x, _imageView.frame.origin.y);

    NSLog(@"scale: %f, %f",imgW,w);
    
    NSArray *info = @[[NSValue valueWithCGPoint:_position], @(_scale), @(_angle),_imageView];
    _passInfo(info);
    
    NSLog(@"旋转：%f 位置：%@  缩放：%f",self.angle, NSStringFromCGPoint(self.position),self.scale);

    [self.navigationController popToRootViewControllerAnimated:NO];
}
@end
