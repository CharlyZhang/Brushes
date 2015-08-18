//
//  Macro.h
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/17.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#ifndef HYDrawing_Macro_h
#define HYDrawing_Macro_h

// 系统版本
#define kSystemVersion [[[UIDevice currentDevice] systemVersion] integerValue]

// 屏幕宽高
#define kSysW [UIScreen mainScreen].bounds.size.width
#define kSysH [UIScreen mainScreen].bounds.size.height

#define kScreenW (kSystemVersion<=7.0 ? kSysH : kSysW)
#define kScreenH (kSystemVersion<=7.0 ? kSysW : kSysH)

// 屏幕比
#define kScreenScale 4.0/3.0

// 颜色工具
#define kColor(r,g,b,a) [UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a]
// 字体
#define kFontSize(size) [UIFont systemFontOfSize:size]
// 图片
#define kImage(name) [UIImage imageNamed:name]
// 图片颜色
#define kImageColor(name) [UIColor colorWithPatternImage:kImage(name)]

#define iOS(version) (([[[UIDevice currentDevice] systemVersion] intValue] >= version)?1:0)

#define UIPopoverBackgroundColor [UIColor colorWithRed:255/255.0 green:192/255.0 blue:99/255.0 alpha:1.0]
#define UIPopoverBorderColor [UIColor colorWithRed:147/255.0 green:106/255.0 blue:0/255.0 alpha:1.0]

// 当前视图宽、高
#define kSelfViewHeight self.view.frame.size.height
#define kSelfViewWidth self.view.frame.size.width

// 颜色
#define kCommenSkinColor [UIColor colorWithRed:253/255.0 green:248/255.0 blue:233/255.0 alpha:1.0]
#define kCommenCyanColor kColor(127,219,219,1)


#endif
