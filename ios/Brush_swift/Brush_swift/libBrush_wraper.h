//
//  libBrush_wraper.h
//  Brush_swift
//
//  Created by CharlyZhang on 15-7-2.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface libBrush_wraper : NSObject


@property (nonatomic) CGSize size;

- (id)initWithFrame:(CGRect)frame;

- (EAGLContext*)getGLContext;

- (void)draw;
- (void)draw_test;

- (void)moveBeginFrom:(CGPoint)p;
- (void)movingAt:(CGPoint)p;
- (void)moveEndAt:(CGPoint)p;

@end
