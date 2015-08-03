//
//  ColorWheelView.h
//  HYDrawing
//
//  Created by macbook on 15/8/3.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ColorWheelView;

@protocol ColorWheelViewDelegate <NSObject>

@required
- (void)colorWheelView:(ColorWheelView*)colorWheelView setColor:(UIColor*)color;

@end

@interface ColorWheelView : UIView

@property (nonatomic, weak)id<ColorWheelViewDelegate> delegate;


@end
