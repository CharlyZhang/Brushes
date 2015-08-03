//
//  ColorWheelView.m
//  HYDrawing
//
//  Created by macbook on 15/8/3.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ColorWheelView.h"

@interface ColorWheelView()
{
    CGFloat red,green,blue,alpha;
}

@property (weak, nonatomic) IBOutlet UIView *colorDisplay;
@property (strong,nonatomic) UIColor* color;

@end

@implementation ColorWheelView

- (UIColor*)color {
    if (!_color) {
        _color = [UIColor colorWithRed:0 green:0 blue:0 alpha:1.0];
    }
    return _color;
}
- (IBAction)valueVhanged:(UISlider *)sender {
    switch (sender.tag) {
        case 0:
            [self.color getRed:&red green:&green blue:&blue alpha:&alpha];
            self.color = [UIColor colorWithRed:sender.value green:green  blue:blue alpha:alpha];
            break;
        case 1:
            [self.color getRed:&red green:&green blue:&blue alpha:&alpha];
            self.color = [UIColor colorWithRed:red green:sender.value  blue:blue alpha:alpha];
            break;
        case 2:
            [self.color getRed:&red green:&green blue:&blue alpha:&alpha];
            self.color = [UIColor colorWithRed:red green:green  blue:sender.value alpha:alpha];
            break;
        case 3:
            [self.color getRed:&red green:&green blue:&blue alpha:&alpha];
            self.color = [UIColor colorWithRed:red green:green  blue:blue alpha:sender.value];
            break;
        default:
            break;
    }
    [self.colorDisplay setBackgroundColor:self.color];
    [self.delegate colorWheelView:self setColor:self.color];
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
