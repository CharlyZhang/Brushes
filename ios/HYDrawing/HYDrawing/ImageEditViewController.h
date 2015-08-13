//
//  ImageEditViewController.h
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/21.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ImageEditViewController : UIViewController<UIGestureRecognizerDelegate,UINavigationBarDelegate>

@property(nonatomic, strong)UIImage *originalImg;
@property(nonatomic, retain)UIImageView *imageView;


/* 旋转 缩放 位置 */
@property CGFloat angle;
@property CGFloat scale;
@property CGPoint position;

// block
@property(nonatomic,copy)void (^passInfo)(CGAffineTransform trans);

@property(nonatomic,assign) CGAffineTransform imageTransform;

@end
