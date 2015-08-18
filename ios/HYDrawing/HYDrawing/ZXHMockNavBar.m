//
//  ZXHMockselfBar.m
//  HYDrawing
//
//  Created by macbook on 15/8/13.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ZXHMockNavBar.h"
#import "Macro.h"

@implementation ZXHMockNavBar

-(instancetype)initWithFrame:(CGRect)frame WithLeftBtnTitle:(NSString *)leftTitle title:(NSString *)title rightBtnTitle:(NSString *)rightTitle{
    
    if (self = [super initWithFrame:frame]) {
        CGFloat selfW = CGRectGetWidth(frame);
        CGFloat selfH = 64;
        CGFloat btnW = 60;
        if (leftTitle) {
            UIButton *leftBtn = [[UIButton alloc]initWithFrame:CGRectMake(20, (selfH-30)/2+10, btnW, 30)];
            leftBtn.backgroundColor = [UIColor whiteColor];
            leftBtn.titleLabel.font = kFontSize(14);
            [leftBtn setTitle:leftTitle forState:0];
            [leftBtn setTitleColor:UIPopoverBorderColor forState:0];
            leftBtn.layer.borderColor = UIPopoverBorderColor.CGColor;
            leftBtn.layer.cornerRadius = 4;
            leftBtn.layer.borderWidth = 2;
            _leftBtn = leftBtn;
            [self addSubview:_leftBtn];
        }
        
        if (title) {
            UILabel *titleLabel = [[UILabel alloc]initWithFrame:CGRectMake((selfW-200)/2, (selfH-30)/2+10, 200, 30)];
            titleLabel.text = title;
            titleLabel.textColor = UIPopoverBorderColor;
            titleLabel.font = kFontSize(18);
            titleLabel.textAlignment = NSTextAlignmentCenter;
            [self addSubview:titleLabel];
        }
        
        if (rightTitle) {
            UIButton *rightBtn = [[UIButton alloc]initWithFrame:CGRectMake(selfW-20-btnW, (selfH-30)/2+10, btnW, 30)];
            rightBtn.titleLabel.font = kFontSize(14);
            [rightBtn setTitle:rightTitle forState:0];
            rightBtn.backgroundColor = UIPopoverBackgroundColor;
            [rightBtn setTitleColor:UIPopoverBorderColor forState:0];
            rightBtn.layer.borderColor = UIPopoverBorderColor.CGColor;
            rightBtn.layer.borderWidth = 2;
            rightBtn.layer.cornerRadius = 4;
            _rightBtn = rightBtn;
            [self addSubview:_rightBtn];
        }
        
        // 分割线
        UIView *lineView = [[UIView alloc]initWithFrame:CGRectMake(0, selfH-2, selfW, 2)];
        lineView.backgroundColor = UIPopoverBorderColor;
        
        self.backgroundColor = kCommenSkinColor;
        self.alpha = 0.5;
        
        [self addSubview:lineView];
    }
    
    return self;
}

-(instancetype)initWithLeftBtnTitle:(NSString *)leftTitle title:(NSString *)title rightBtnTitle:(NSString *)rightTitle{


    
    if (self = [super init]) {
        
   
        UIButton *leftBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        leftBtn.backgroundColor = [UIColor whiteColor];
        leftBtn.titleLabel.font = kFontSize(14);
     
        [leftBtn setTitleColor:UIPopoverBorderColor forState:0];
        leftBtn.layer.borderColor = UIPopoverBorderColor.CGColor;
        leftBtn.layer.cornerRadius = 4;
        leftBtn.layer.borderWidth = 2;
        _leftBtn = leftBtn;
        [self addSubview:_leftBtn];
        
        UIButton *rightBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        rightBtn.titleLabel.font = kFontSize(14);
        rightBtn.backgroundColor = UIPopoverBackgroundColor;
        [rightBtn setTitleColor:UIPopoverBorderColor forState:0];
        rightBtn.layer.borderColor = UIPopoverBorderColor.CGColor;
        rightBtn.layer.borderWidth = 2;
        rightBtn.layer.cornerRadius = 4;
        _rightBtn = rightBtn;
        [self addSubview:_rightBtn];
        
        
        
//        if (title) {
//            UILabel *titleLabel = [[UILabel alloc]initWithFrame:CGRectMake((selfW-200)/2, (selfH-30)/2, 200, 30)];
//            titleLabel.text = title;
//            titleLabel.textColor = UIPopoverBorderColor;
//            titleLabel.font = kFontSize(18);
//            titleLabel.textAlignment = NSTextAlignmentCenter;
//            [self addSubview:titleLabel];
//        }
        if (leftTitle) {
               [leftBtn setTitle:leftTitle forState:UIControlStateNormal];
        }
        
        if (rightTitle) {
            [rightBtn setTitle:leftTitle forState:UIControlStateNormal];
        }
        
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(leftBtn,rightBtn);
        leftBtn.translatesAutoresizingMaskIntoConstraints = NO;
        rightBtn.translatesAutoresizingMaskIntoConstraints = NO;
        
        // |- 默认和垂直约束相等 = 20
        // |-(== 100)- 水平左约束100
        
        // subview(100) 水平约束 - 宽
        // subview(50) 垂直约束 - 高
        
        NSArray *constraints = [NSLayoutConstraint
                                constraintsWithVisualFormat:@"H:|-[leftBtn(80)]"
                                options:0
                                metrics:nil
                                views:viewsDictionary];
        constraints = [constraints arrayByAddingObjectsFromArray:
                       [NSLayoutConstraint
                        constraintsWithVisualFormat:@"H:[rightBtn(44)]-|"
                        options:0
                        metrics:nil
                        views:viewsDictionary]];
        
        constraints = [constraints arrayByAddingObjectsFromArray:
                       [NSLayoutConstraint
                        constraintsWithVisualFormat:@"V:[leftBtn(80)]-(==40)-|"
                        options:0
                        metrics:nil
                        views:viewsDictionary]];
        
        constraints = [constraints arrayByAddingObjectsFromArray:
                       [NSLayoutConstraint
                        constraintsWithVisualFormat:@"V:|-[rightBtn(32)]"
                        options:0
                        metrics:nil
                        views:viewsDictionary]];
        
        [self addConstraints:constraints];
        
        
        
        // 分割线
//        UIView *lineView = [[UIView alloc]initWithFrame:CGRectMake(0, selfH-2, selfW, 2)];
//        lineView.backgroundColor = UIPopoverBorderColor;
//        
//        self.backgroundColor = kCommenSkinColor;
//        self.alpha = 0.5;
//        
//        [self addSubview:lineView];
    }
    
    return self;
}


@end
