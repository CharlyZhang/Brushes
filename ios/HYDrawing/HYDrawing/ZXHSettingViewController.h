//
//  ZXHSettingViewController.h
//  HYDrawing
//
//  Created by macbook on 15/8/24.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol SettingViewControllerDelegate <NSObject>

@required
-(void)settingForCreateNewCanvas;
-(void)settingForSavePainting;
-(void)settingForClearCanvas;
-(void)settingForTransformCanvas;

@end

@interface ZXHSettingViewController : UIViewController

@property(nonatomic,assign)id delegate;

@end
