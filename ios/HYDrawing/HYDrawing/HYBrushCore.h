//
//  HYBrushCore.h
//  HYDrawing
//
//  Created by CharlyZhang on 15/8/10.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WDColor.h"

@class CanvasView;

@interface HYBrushCore : NSObject

@property (nonatomic) BOOL hasInitialized;

+ (HYBrushCore*) sharedInstance;

///初始化
- (BOOL) initializeWithWidth:(float)w height:(float)h scale:(float)s;

///设置glsl所在文件夹
- (void) setGLSLDirectory:(const char*) glslDir;

///获得绘制视图
- (CanvasView*) getPaintingView;

/// 绘制
- (void) draw;

///工具激活
- (void) activeEraser;
- (void) activePencil;
- (void) activeMarker;
- (void) activeCrayon;
- (void) activeWaterColorPen;
- (void) activeBucket;
- (void) activeColorPicker;

///当前绘制颜色
- (WDColor*) getActiveStatePaintColor;
- (void) setActiveStateColor:(UIColor*)color;

///快速颜色
- (void) setActiveStateSwatchColor:(WDColor*)color atIndex:(NSInteger)index;
- (void) setActiveStatePaintColorAtIndex:(NSInteger)index;
- (WDColor*) getColorFromActiveStateSwatchAtIndex:(NSInteger)index;

///图片编辑
- (void) beginPhotoPlacement:(UIImage*) photo withTransform:(CGAffineTransform)trans;
- (BOOL) setPhotoTransform:(CGAffineTransform)transform;
- (void) endPhotoPlacement:(BOOL) renderToLayer;

- (NSInteger) renderImage:(UIImage*)image withTransform:(CGAffineTransform)trans newLayer:(BOOL)flag;
                                                                        ///<如果成功，返回插入图片所在图层的序号，否则返回负数
///操作图层
- (BOOL) setActiveLayerTransform:(CGAffineTransform)transform;
- (BOOL) renderActiveLayerWithTransform:(CGAffineTransform)transform;
- (BOOL) setActiveLayerLinearInterprolation:(BOOL)flag;
///绘制背景
- (void) renderBackground:(UIImage*)image;

///图层
- (NSInteger) getLayersNumber;
- (UIImage*) getLayerThumbnailOfIndex:(NSInteger)index;
- (NSInteger) addNewLayer;                                             ///<如果成功，会将新图层设置为当前图层，返回其序号
- (NSInteger) duplicateActiveLayer;                                    ///<如果成功，会将新图层设置为当前图层，返回其序号
- (NSInteger) setActiveLayer:(NSInteger)idx;
- (NSInteger) getActiveLayerIndex;
- (BOOL) moveLayerFrom:(NSInteger)fromIdx to:(NSInteger)toIdx;         ///<如果成功，会将toIdx设置为当前图层
- (BOOL) deleteActiveLayer;
- (BOOL) restoreDeletedLayer;
- (void) setVisibility:(BOOL)visible ofLayer:(NSInteger)index;
- (BOOL) isVisibleOfLayer:(NSInteger)index;
- (void) setLocked:(BOOL)locked ofLayer:(NSInteger)index;
- (BOOL) isLockedofLayer:(NSInteger)index;
- (void) setActiveLayerOpacity:(float)opacity;
- (float) getOpacityOfLayer:(NSInteger)index;
- (BOOL) clearLayer:(NSInteger)index;

///笔触大小
- (void) setActiveBrushSize:(float) value;
- (float) getActiveBrushSize;

///画板
- (float) getCanvasScale;
- (CGPoint) convertToPainting:(CGPoint)pt;
- (CGSize) getPaintingSize;

/// undo & redo
- (BOOL) undoPaintingOfLayer:(NSInteger)idx;
- (BOOL) redoPaintingOfLayer:(NSInteger)idx;

@end
