//
//  HYBrushCore.h
//  HYDrawing
//
//  Created by CharlyZhang on 15/8/10.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WDColor.h"


@interface HYBrushCore : NSObject

@property (nonatomic) BOOL hasInitialized;

+ (HYBrushCore*) sharedInstance;

///初始化
- (BOOL) initializeWithWidth:(float)w height:(float)h;

///获得绘制视图
- (UIView*) getPaintingView;

///激活橡皮
- (void) activeEraser;
///激活铅笔
- (void) activePencil;
///激活蜡笔
- (void) activeCrayon;

///获取当前绘制颜色
- (WDColor*) getActiveStatePaintColor;
///设置当前绘制颜色
- (void) setActiveStateColor:(UIColor*)color;

///设置挑选颜色
- (void) setActiveStateSwatchColor:(WDColor*)color atIndex:(NSUInteger)index;
- (void) setActiveStatePaintColorAtIndex:(NSUInteger)index;
//获取挑选颜色
- (WDColor*) getColorFromActiveStateSwatchAtIndex:(NSUInteger)index;

///绘制图片
- (void)renderImage:(UIImage*)image withTranslate:(CGPoint)vectT rotate:(float)angel scale:(float)s;

- (void)testRenderImage:(UIImage*)image withTransform:(CGAffineTransform)transform;

@end