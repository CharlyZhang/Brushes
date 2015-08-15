//
//  ZXHEditableTipsView.m
//  HYDrawing
//
//  Created by macbook on 15/8/14.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ZXHEditableTipsView.h"
#import "Macro.h"

@implementation ZXHEditableTipsView

static ZXHEditableTipsView *tipsView;

+(id)defaultTipsView{
    if (!tipsView) {
        CGFloat bottomBarH = 119;
        
        tipsView = [[NSBundle mainBundle]loadNibNamed:@"ZXHEditableTipsView" owner:self options:nil][0];
        tipsView.frame = CGRectMake(0, 0, kScreenH, kScreenW-bottomBarH);
        tipsView.backgroundColor = [UIColor clearColor];
        // 提示图片
        tipsView.visibleView.alpha = 0;
        tipsView.lockedView.alpha = 0;
    }
    
    return tipsView;
}


#pragma mark 手势
-(void)showTips{
    // 显示提示
    NSLog(@"show");
    
    if (!_visible || _locked) {
        [UIView animateWithDuration:1 animations:^{
            if (!_visible) {
                self.visibleView.alpha = 1;
            }
            if (_locked) {
                self.lockedView.alpha = 1;
            }
        }];
    }else if(_visible && !_locked){
        [tipsView removeFromSuperview];
    }
}

-(void)dismissTips{
    [UIView animateWithDuration:2 animations:^{
        self.visibleView.alpha = 0;
        self.lockedView.alpha = 0;
    }];
}

@end
