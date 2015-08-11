//
//  ImageEditViewController.m
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/21.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ImageEditViewController.h"


@implementation ImageEditViewController

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

    _imageView = [[UIImageView alloc]init];
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
        _imageView.gestureRecognizers = @[rotation,pinch];
    }
}

#pragma mark 图片的旋转、缩放、移动
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer{
    return YES;
}

-(void)pinchImageing:(UIPinchGestureRecognizer*)pinch{
    NSLog(@"缩放：%f",pinch.scale);
}

-(void)panImageing:(UIPanGestureRecognizer*)pan{
    NSLog(@"平移：%f",[pan translationInView:_imageView].x);
}

-(void)rotatingImage:(UIRotationGestureRecognizer*)rotation{
    NSLog(@"旋转：%f",rotation.rotation);
}

#pragma mark 编辑完成，传递参数，回到画图页面
-(void)toDrawingPage{
    [self.navigationController popToRootViewControllerAnimated:YES];
}
@end
