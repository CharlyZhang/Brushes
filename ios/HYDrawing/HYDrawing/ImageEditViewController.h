//
//  ImageEditViewController.h
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/21.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol ImageEditViewControllerDelegate <NSObject>

@required
- (void)updateLayersView;

@end

@interface ImageEditViewController : UIViewController<UIGestureRecognizerDelegate>

@property(nonatomic, strong)UIImage *originalImg;
@property(nonatomic, retain)UIImageView *imageView;
@property(nonatomic, weak)id<ImageEditViewControllerDelegate> delegate;


/* 旋转 缩放 位置 */
//@property CGFloat angle;
//@property CGFloat scale;
//@property CGPoint position;

@property(nonatomic,assign) CGAffineTransform imageTransform;

@end
