//
//  LayersCell.h
//  HYDrawing
//
//  Created by macbook on 15/8/10.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface LayersCell : UITableViewCell

@property (weak, nonatomic) IBOutlet UIButton *btnVisible;
@property (weak, nonatomic) IBOutlet UIButton *btnUnlock;
@property (weak, nonatomic) IBOutlet UIImageView *outlineView;
- (IBAction)setUnvisibleOr:(UIButton *)sender;
- (IBAction)setLockOr:(UIButton *)sender;

-(void)setOutlineViewBorderWithColor:(UIColor*)color;
@end
