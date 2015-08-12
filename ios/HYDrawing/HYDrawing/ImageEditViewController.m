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
    CGAffineTransform _transform;
    CGFloat _lastRotation;
    CGFloat _lastScale;
    CGFloat initialScale_;
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
    self.navigationController.navigationBar.barTintColor = [UIColor whiteColor];
    self.navigationController.navigationBar.translucent = NO;
    self.navigationController.navigationBar.tintColor = [UIColor blackColor];
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc]initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(backHome)];
    
    // 变换初始值
    
    // 右侧navItem
    UIBarButtonItem *doneBtn = [[UIBarButtonItem alloc]initWithTitle:@"Accept" style:UIBarButtonItemStylePlain target:self action:@selector(toDrawingPage)];
    self.navigationItem.rightBarButtonItem = doneBtn;

    _imageView = [[UIImageView alloc]initWithFrame:self.view.frame];
//    _imageView.frame = CGRectMake(0, 0, _originalImg.size.width/2, _originalImg.size.height/2);
    [self.view addSubview:_imageView];
    _imageView.image = _originalImg;
//    _imageView.contentMode = UIViewContentModeCenter;
    [self constrainSubview:_imageView toMatchWithSuperview:self.view];
    
    self.view.backgroundColor = [UIColor whiteColor];
    _imageView.image = [UIImage imageNamed:@"new_canvas"];
    [super viewDidLoad];
    
    if (_originalImg) {
        _imageView.image = _originalImg;
        _imageView.userInteractionEnabled = YES;
        
        // 添加手势
//        UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchImageing:)];
//        UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(handlePanGesture:)];
//        pinch.delegate = self;
//        UIRotationGestureRecognizer *rotation = [[UIRotationGestureRecognizer alloc]initWithTarget:self action:@selector(rotatingImage:)];
//        _imageView.gestureRecognizers = @[pan,rotation,pinch];
    }
    
    
    
}

#pragma mark 图片的旋转、缩放、移动
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer{
    return YES;
}


#pragma mark 编辑完成，传递参数，回到画图页面
-(void)toDrawingPage{
    _passInfo(_imageView.transform);

    [self.navigationController popToRootViewControllerAnimated:NO];
}

-(void)backHome{
    [self.navigationController popToRootViewControllerAnimated:NO];
}

@end
