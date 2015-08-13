//
//  ImageEditViewController.h
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/21.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ImageEditViewController : UIViewController<UIGestureRecognizerDelegate>

@property(nonatomic, strong)UIImage *originalImg;
@property(nonatomic, retain)UIImageView *imageView;
// 上次旋转的角度
@property CGFloat lastRotation;

/* 旋转 缩放 位置 */
@property CGFloat angle;
@property CGFloat scale;
@property CGPoint position;

// block
@property(nonatomic,copy)void (^passInfo)(NSArray *arr);

@property(nonatomic,assign) CGAffineTransform imageTransform;

@end
