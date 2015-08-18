//
//  ZXHMockNavBar.h
//  HYDrawing
//
//  Created by macbook on 15/8/13.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ZXHMockNavBar : UIView

@property(nonatomic,assign)SEL leftBtnAction;
@property(nonatomic,assign)SEL rightBtnAction;
@property(nonatomic,retain)UIButton *leftBtn;
@property(nonatomic,retain)UIButton *rightBtn;

-(instancetype)initWithFrame:(CGRect)frame WithLeftBtnTitle:(NSString *)leftTitle title:(NSString *)title rightBtnTitle:(NSString *)rightTitle;

-(instancetype)initWithLeftBtnTitle:(NSString *)leftTitle title:(NSString *)title rightBtnTitle:(NSString *)rightTitle;

@end
