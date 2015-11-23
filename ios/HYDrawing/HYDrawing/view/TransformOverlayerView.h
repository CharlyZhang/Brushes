//
//  TransformOverlayerView.h
//  HYDrawing
//
//  Created by CharlyZhang on 15/11/19.
//  Copyright © 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ImageEditViewController.h"

@interface TransformOverlayerView : UIView <UIGestureRecognizerDelegate>

@property (nonatomic, readonly) CGAffineTransform alignedTransform;
@property (nonatomic, weak) id<ImageEditViewControllerDelegate> delegate;

@end
