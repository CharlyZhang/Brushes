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
    self.view.backgroundColor = [UIColor clearColor];
    _imageView.image = [UIImage imageNamed:@"new_canvas"];
    [super viewDidLoad];
    
    if (_originalImg) {
        _imageView.image = _originalImg;
        _imageView.userInteractionEnabled = YES;
        
        // 添加手势
        UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchImageing:)];
        UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(panImageing:)];
        pinch.delegate = self;
        UIRotationGestureRecognizer *rotation = [[UIRotationGestureRecognizer alloc]initWithTarget:self action:@selector(rotatingImage:)];
        _imageView.gestureRecognizers = @[pan,rotation,pinch];
    }
    
    // 记住原图大小
    imgW = _originalImg.size.width;
    
    // 变换默认值
    _scale = 1;
    _position = CGPointMake(0, 0);
    _angle = 0;
    
}

#pragma mark 图片的旋转、缩放、移动
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer{
    return YES;
}

-(void)pinchImageing:(UIPinchGestureRecognizer*)pinch{
    CGPoint location = [pinch locationInView:self.view];
    pinch.view.center = CGPointMake(location.x, location.y);
    pinch.view.transform = CGAffineTransformScale(pinch.view.transform, pinch.scale, pinch.scale);
    self.scale = pinch.scale;
//    NSLog(@"缩放：%f",pinch.scale);
    pinch.scale = 1;
}

-(void)panImageing:(UIPanGestureRecognizer*)pan{
    CGPoint location = [pan locationInView:self.view];
    pan.view.center = CGPointMake(location.x,  location.y);
//    NSLog(@"平移：%f",pan.view.frame.origin.x);
}

-(void)rotatingImage:(UIRotationGestureRecognizer*)gesture{
    CGPoint location = [gesture locationInView:self.view];
    gesture.view.center = CGPointMake(location.x, location.y);
    
    if (gesture.state == UIGestureRecognizerStateEnded) {
        self.angle = gesture.rotation;
        self.lastRotation = 0;
        return;
    }
    
    CGAffineTransform currentTransform = self.imageView.transform;
    CGFloat rotation = 0.0 - (self.lastRotation - gesture.rotation);
    CGAffineTransform newTransform = CGAffineTransformRotate(currentTransform, rotation);
    self.imageView.transform = newTransform;
    self.lastRotation = gesture.rotation;
}

#pragma mark 编辑完成，传递参数，回到画图页面
-(void)toDrawingPage{

    CGFloat w = _imageView.frame.size.width;
    CGFloat h = _imageView.bounds.size.height;
    
    self.position = CGPointMake(_imageView.frame.origin.x, _imageView.frame.origin.y);

    NSLog(@"scale: %f, %f",imgW,w);
    
    NSArray *info = @[[NSValue valueWithCGPoint:_position], @(_scale), @(_angle)];
    _passInfo(info);
    
    NSLog(@"旋转：%f 位置：%@  缩放：%f",self.angle, NSStringFromCGPoint(self.position),self.scale);

    [self.navigationController popToRootViewControllerAnimated:YES];
}
@end
