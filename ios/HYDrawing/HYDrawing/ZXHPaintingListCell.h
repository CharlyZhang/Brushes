//
//  ZXHPaintingListCell.h
//  HYDrawing
//
//  Created by macbook on 15/8/24.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ZXHPaintingListCell : UITableViewCell

@property (weak, nonatomic) IBOutlet UIImageView *imgView;
@property (weak, nonatomic) IBOutlet UIButton *btnDelete;
@property (weak, nonatomic) IBOutlet UIButton *btnRename;
@property (weak, nonatomic) IBOutlet UILabel *nameLabel;

// 约束
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *imgViewRightCons;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *nameLabelRightCons;

-(void)setBorderStyleWithColor:(UIColor*)color;

@end
