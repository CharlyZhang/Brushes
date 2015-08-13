//
//  ZXHLayerTopBar.m
//  HYDrawing
//
//  Created by macbook on 15/8/13.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ZXHLayerTopBar.h"
#import "Macro.h"

@implementation ZXHLayerTopBar

-(id)initWithCoder:(NSCoder *)aDecoder{
    if (self = [super initWithCoder:aDecoder]) {
        NSLog(@"initWithCoder");
        self.backgroundColor = UIPopoverBackgroundColor;
    }
    
    return self;
}

-(void)awakeFromNib{
    NSLog(@"awakeFromNib");
    self.backgroundColor = UIPopoverBackgroundColor;
}
@end
