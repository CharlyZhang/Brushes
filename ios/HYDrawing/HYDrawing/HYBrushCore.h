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

///工具激活
- (void) activeEraser;
- (void) activePencil;
- (void) activeCrayon;
- (void) activeBucket;

///当前绘制颜色
- (WDColor*) getActiveStatePaintColor;
- (void) setActiveStateColor:(UIColor*)color;

///快速颜色
- (void) setActiveStateSwatchColor:(WDColor*)color atIndex:(NSUInteger)index;
- (void) setActiveStatePaintColorAtIndex:(NSUInteger)index;
- (WDColor*) getColorFromActiveStateSwatchAtIndex:(NSUInteger)index;

///绘制图片
- (void) renderImage:(UIImage*)image withTransform:(CGAffineTransform)trans;

///图层
- (NSUInteger) getLayersNumber;
- (UIImage*) getLayerThumbnailOfIndex:(NSUInteger)index;
- (NSUInteger) addNewLayer;                                             ///<如果成功，会将新图层设置为当前图层，返回其序号
- (NSUInteger) getActiveLayerIndex;
- (BOOL) moveLayerFrom:(NSUInteger)fromIdx to:(NSUInteger)toIdx;        ///<如果成功，会将toIdx设置为当前图层
- (BOOL) deleteActiveLayer;
- (BOOL) toggleVisibilityOfLayerIndex:(NSUInteger) index;
- (BOOL) toggleAlphaLockedOfLayerIndex:(NSUInteger) index;




- (void)testRenderImage:(UIImage*)image withTransform:(CGAffineTransform)transform;

@end
