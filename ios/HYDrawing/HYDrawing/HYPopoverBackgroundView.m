//
//  HYPopoverBackgroundView.m
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/17.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "HYPopoverBackgroundView.h"
#define kArrowBase 30.0f
#define kArrowHeight 20.0f
#define kBorderInset 2.0f

@interface HYPopoverBackgroundView()

@property(nonatomic, copy)UIImage *image;
@property(nonatomic, assign)NSInteger type;
@end


@implementation HYPopoverBackgroundView
@synthesize arrowDirection  = _arrowDirection;
@synthesize arrowOffset     = _arrowOffset;
- (NSArray *)constrainSubview:(UIView *)subview toMatchWithSuperview:(UIView *)superview
{
    subview.translatesAutoresizingMaskIntoConstraints = NO;
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(subview);
    
    NSArray *constraints = [NSLayoutConstraint
                            constraintsWithVisualFormat:@"H:|[subview]|"
                            options:0
                            metrics:nil
                            views:viewsDictionary];
    constraints = [constraints arrayByAddingObjectsFromArray:
                   [NSLayoutConstraint
                    constraintsWithVisualFormat:@"V:|[subview]|"
                    options:0
                    metrics:nil
                    views:viewsDictionary]];
    [superview addConstraints:constraints];
    
    return constraints;
}


+ (CGFloat)arrowBase
{
    return kArrowBase;
}

+ (CGFloat)arrowHeight
{
    return kArrowHeight;
}

+ (UIEdgeInsets)contentViewInsets
{
//    return UIEdgeInsetsZero;
        return UIEdgeInsetsMake(0, kBorderInset, kBorderInset, kBorderInset);
    
}

+ (BOOL)wantsDefaultContentAppearance
{
    return YES;
}
- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        _backgroundImageView = [[UIImageView alloc]init];
        _backgroundImageView.image= _image;
        [self addSubview:_backgroundImageView];
        [self constrainSubview:_backgroundImageView toMatchWithSuperview:self];

    }
    return self;
}

-(void)setType:(NSInteger)type{
    self.type = type;
}
@end
