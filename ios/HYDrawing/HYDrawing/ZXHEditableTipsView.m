//
//  ZXHEditableTipsView.m
//  HYDrawing
//
//  Created by macbook on 15/8/14.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ZXHEditableTipsView.h"

@implementation ZXHEditableTipsView

-(instancetype)initWithFrame:(CGRect)frame{
    if (self = [super initWithFrame:frame]) {
        
        self.alpha = 0;
        
        // 提示图片
        
        
        // 滑动手势
        UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(showTipsImage:)];
        [self addGestureRecognizer:pan];
        
    }
    
    return self;
}

#pragma mark 手势
-(void)showTipsImage:(UIPanGestureRecognizer*)pan{
    if (pan.state == UIGestureRecognizerStateBegan) {
        // 显示提示
        NSLog(@"Began");
    }
    
    if (pan.state == UIGestureRecognizerStateEnded) {
        // 移除提示
        NSLog(@"Ended");
    }
}

@end
