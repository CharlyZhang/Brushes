//
//  LayersCell.m
//  HYDrawing
//
//  Created by macbook on 15/8/10.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "LayersCell.h"
#import "Macro.h"

@implementation LayersCell



-(id)initWithCoder:(NSCoder *)aDecoder{
    if (self = [super initWithCoder:aDecoder]) {
        // 设置默认值
        _isVisible = YES;
        _isUnlocked = YES;
    }
    
    return self;
}

-(void)awakeFromNib{
    self.backgroundColor = kCommenSkinColor;
    [self setOutlineViewBorderWithColor:kCommenCyanColor];
}

// 轮廓样式
-(void)setOutlineViewBorderWithColor:(UIColor *)color{
    _outlineView.layer.borderWidth = 2;
    _outlineView.layer.borderColor = color.CGColor;
    _outlineView.layer.cornerRadius = 4;
    _outlineView.clipsToBounds = YES;
    _outlineView.layer.masksToBounds = YES;
}

// 选中cell的背景图片
-(UIView *)selectedBackgroundView{
    UIView *view = (UIView *)[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"layer_cell_selected_bg"]];
    return view;
}


- (IBAction)setUnvisibleOr:(UIButton *)sender {
    if (_isVisible) {
        [sender setImage:[UIImage imageNamed:@"layer_invisible"] forState:0];
    }else{
        [sender setImage:[UIImage imageNamed:@"layer_visible"] forState:0];
    }
    
    _isVisible = !_isVisible;
    NSLog(@"unvisible");
}

- (IBAction)setLockOr:(UIButton *)sender {
    if (_isUnlocked) {
        [sender setImage:[UIImage imageNamed:@"layer_lock"] forState:0];
    }else{
        [sender setImage:[UIImage imageNamed:@"layer_unlock"] forState:0];
    }
    
    if (self.selected) {
        // 发送是否可编辑消息
        [[NSNotificationCenter defaultCenter]postNotificationName:@"LayerLockNotification" object:@(!_isUnlocked)];
    }
    
    _isUnlocked = !_isUnlocked;
    NSLog(@"lock");
}
@end
