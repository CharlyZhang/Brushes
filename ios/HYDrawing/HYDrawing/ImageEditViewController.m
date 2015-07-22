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
    

    _imageView = [[UIImageView alloc]init];
    [self.view addSubview:_imageView];
    _imageView.contentMode = UIViewContentModeCenter;
    [self constrainSubview:_imageView toMatchWithSuperview:self.view];
    
    _imageView.backgroundColor = [UIColor clearColor];
    self.view.backgroundColor = [UIColor clearColor];
    _imageView.image = [UIImage imageNamed:@"new_canvas"];
    [super viewDidLoad];
    
}
@end
