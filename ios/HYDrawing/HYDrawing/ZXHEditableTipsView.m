//
//  ZXHEditableTipsView.m
//  HYDrawing
//
//  Created by macbook on 15/8/14.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ZXHEditableTipsView.h"

@implementation ZXHEditableTipsView

static ZXHEditableTipsView *tipsView;

+(id)defaultTipsView{
    if (!tipsView) {
        tipsView = [ZXHEditableTipsView new];
        tipsView.backgroundColor = [UIColor clearColor];
        // 提示图片
        tipsView.visibleView.alpha = 0;
        tipsView.lockedView.alpha = 0;
        [tipsView addGestureRecognizer:[[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(handlePanGesture:)]];
    }
    
    return tipsView;
}

-(void)handlePanGesture:(UIPanGestureRecognizer*)pan{
    if (pan.state == UIGestureRecognizerStateBegan) {
        // 显示提示
        NSLog(@"Began");
        if (!_visible || !_locked) {
            [self showTips];
        }else{
            [tipsView removeFromSuperview];
        }
    }
    
    if (pan.state == UIGestureRecognizerStateEnded) {
         [self dismissTips];
    }

}

#pragma mark 手势
-(void)showTips{
    [UIView animateWithDuration:0.01 animations:^{
        self.visibleView.alpha = 1;
        self.lockedView.alpha = 1;
    }];
}

-(void)dismissTips{
    [UIView animateWithDuration:2 animations:^{
        self.visibleView.alpha = 0;
        self.lockedView.alpha = 0;
    }];
}

@end
