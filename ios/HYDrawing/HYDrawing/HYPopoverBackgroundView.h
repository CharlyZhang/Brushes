//
//  HYPopoverBackgroundView.h
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/17.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HYPopoverBackgroundView : UIPopoverBackgroundView

@property(nonatomic, retain)UIImageView *backgroundImageView;
-(void)setType:(NSInteger)type;
@end
