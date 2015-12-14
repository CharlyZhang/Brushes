//
//  ZXHPaintingListController.h
//  HYDrawing
//
//  Created by macbook on 15/8/24.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ZXHPaintingListController;

@protocol PaintingListControllerDelegate <NSObject>

- (void) paintingListController:(ZXHPaintingListController*)paintingListCtrl didSelectAt:(NSInteger)index;

@end

@interface ZXHPaintingListController : UIViewController

@property (nonatomic,weak) id<PaintingListControllerDelegate> delegate;

- (void)refreshData;

@end
