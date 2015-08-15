//
//  HYBrushCore.m
//  HYDrawing
//
//  Created by CharlyZhang on 15/8/10.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "HYBrushCore.h"
#import "CZViewImpl.h"
#include "BrushesCore.h"
#import <QuartzCore/QuartzCore.h>

@interface HYBrushCore()
{
    CZViewImpl *viewImpl;
    CZCanvas *canvas;
    CZPainting *painting;
}
@end

@implementation HYBrushCore

+ (HYBrushCore *) sharedInstance
{
    static HYBrushCore *brushCore_ = nil;
    
    if (!brushCore_) {
        brushCore_ = [[HYBrushCore alloc] init];
    }
    
    return brushCore_;
}

/// 初始化
- (BOOL) initializeWithWidth:(float)w height:(float)h
{
    viewImpl = new CZViewImpl(CZRect(0,0,w,h));
    canvas = new CZCanvas(viewImpl);
    painting = new CZPainting(CZSize(w,h));
    canvas->setPaiting(painting);
    CZActiveState::getInstance()->setEraseMode(false);
    CZActiveState::getInstance()->setActiveBrush(kPencil);
    CZActiveState::getInstance()->mainScreenScale = [UIScreen mainScreen].scale;
    
    self.hasInitialized = YES;
    return YES;
}

///获得绘制视图
- (UIView*) getPaintingView
{
    if (!viewImpl) return nil;
    return viewImpl->realView;
}


///激活橡皮
- (void) activeEraser
{
    CZActiveState::getInstance()->colorFillMode = false;
    CZActiveState::getInstance()->setEraseMode(true);
    CZActiveState::getInstance()->setActiveBrush(kEraser);
}
///激活铅笔
- (void) activePencil
{
    CZActiveState::getInstance()->colorFillMode = false;
    CZActiveState::getInstance()->setEraseMode(false);
    CZActiveState::getInstance()->setActiveBrush(kPencil);
}
///激活蜡笔
- (void) activeCrayon
{
    CZActiveState::getInstance()->colorFillMode = false;
    CZActiveState::getInstance()->setEraseMode(false);
    CZActiveState::getInstance()->setActiveBrush(kCrayon);
}
///激活倒色桶
- (void) activeBucket
{
    CZActiveState::getInstance()->colorFillMode = true;
}

///获取当前绘制颜色
- (WDColor*)getActiveStatePaintColor
{
    CZColor myColor = CZActiveState::getInstance()->getPaintColor();
    WDColor *ret = [WDColor colorWithRed: myColor.red green:myColor.green blue:myColor.blue alpha:myColor.alpha];
    return ret;
}
///设置当前绘制颜色
- (void) setActiveStateColor:(UIColor*)color
{
    CGFloat r,g,b,a;
    [color getRed:&r green:&g blue:&b alpha:&a];
    CZColor pc(r,g,b,a);
    CZActiveState::getInstance()->setPaintColor(pc);
}

///设置挑选颜色
- (void) setActiveStateSwatchColor:(WDColor*)color atIndex:(NSInteger)index
{
    if(color){
        CZColor *c = new CZColor(color.red,color.green,color.blue,color.alpha);
        CZActiveState::getInstance()->setSwatch(c, (int)index);
    }
    else {
        CZActiveState::getInstance()->setSwatch(NULL, (int)index);
    }
}
- (void) setActiveStatePaintColorAtIndex:(NSInteger)index
{
    CZActiveState::getInstance()->setPaintColorAsSwatch(int(index));
}

- (WDColor*) getColorFromActiveStateSwatchAtIndex:(NSInteger)index
{
    CZColor *c = CZActiveState::getInstance()->getSwatch((int)index);
    if (c)  return [WDColor colorWithRed:c->red green:c->green blue:c->blue alpha:c->alpha];
    else    return nil;
}

///绘制图片
- (NSInteger)renderImage:(UIImage*)image withTransform:(CGAffineTransform)transform;

{
    int ret = painting->addNewLayer();
    if (ret < 0) return (NSInteger)ret;
    
    CGImageRef img = image.CGImage;
    
    //数据源提供者
    CGDataProviderRef inProvider = CGImageGetDataProvider(img);
    // provider’s data.
    CFDataRef inBitmapData = CGDataProviderCopyData(inProvider);
    
    //宽，高，data
    size_t width = CGImageGetWidth(img);
    size_t height = CGImageGetHeight(img);
    
    CZImage *brushImg = new CZImage(width,height,RGBA_BYTE,CFDataGetBytePtr(inBitmapData));
    
    CGImageAlphaInfo alphaInfo = CGImageGetAlphaInfo(img);
    
    if (alphaInfo == kCGImageAlphaNone ||
        alphaInfo == kCGImageAlphaNoneSkipLast ||
        alphaInfo == kCGImageAlphaNoneSkipFirst){
        brushImg->hasAlpha = false;
    }
    else {
        brushImg->hasAlpha = true;
    }


//    
    //////
    CZSize paintingSize = painting->getDimensions();

    
    CZAffineTransform trans_flip = CZAffineTransform::makeFromScale(1, -1);
    CZAffineTransform trans_adjust = CZAffineTransform::makeFromTranslation(-(width/2.0), -(height/2.0));
    CZAffineTransform trans_center = CZAffineTransform::makeFromTranslation((paintingSize.width/2.0), (paintingSize.height/2.0));
    
    CZAffineTransform trans = CZAffineTransform(transform.a,transform.b,transform.c,transform.d,transform.tx,transform.ty);
    

    trans = (trans_adjust * trans_flip * trans_center) * trans;
    
    painting->getActiveLayer()->renderImage(brushImg, trans);
    canvas->drawView();
    
    return (NSInteger)ret;
}


- (NSInteger) getLayersNumber
{
    return NSInteger(painting->getLayersNumber());
}

- (UIImage*) getLayerThumbnailOfIndex:(NSInteger)index
{
    int layersNum = painting->getLayersNumber();
    
    CZLayer *layer = painting->getLayer(int(layersNum - 1 - index));
    if (!layer)         return nil;
    CZImage *thumbImage = layer->getThumbnailImage();
    if (!thumbImage)    return nil;

    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGContextRef ctx = CGBitmapContextCreate(thumbImage->data, thumbImage->width, thumbImage->height, 8, thumbImage->width*4,
                                             colorSpaceRef, kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast);
    CGImageRef imageRef = CGBitmapContextCreateImage(ctx);
    
    UIImage *ret = [[UIImage alloc] initWithCGImage:imageRef scale:[UIScreen mainScreen].scale orientation:UIImageOrientationDownMirrored];
    
    CGImageRelease(imageRef);
    CGContextRelease(ctx);
    CGColorSpaceRelease(colorSpaceRef);
    
    return ret;
}

- (NSInteger) addNewLayer
{
    int layersNum = painting->getLayersNumber();
    return NSInteger(layersNum - 1 - painting->addNewLayer());
}

- (NSInteger) duplicateActiveLayer
{
    int layersNum = painting->getLayersNumber();
    NSInteger ret = NSInteger(layersNum - 1 - painting->duplicateActiveLayer());
    canvas->drawView();
    return ret;
}

- (NSInteger) setActiveLayer:(NSInteger)idx
{
    int layersNum = painting->getLayersNumber();
    return NSInteger(layersNum - 1 - painting->setActiveLayer(int(layersNum - 1 -idx)));
}

- (NSInteger) getActiveLayerIndex
{
    int layersNum = painting->getLayersNumber();
    return NSInteger(layersNum - 1 - painting->getActiveLayerIndex());
}

- (BOOL) moveLayerFrom:(NSInteger)fromIdx to:(NSInteger)toIdx
{
    int layersNum = painting->getLayersNumber();
    
    BOOL ret = painting->moveLayer(int(layersNum - 1 - fromIdx), int(layersNum - 1 - toIdx));
    canvas->drawView();
    return ret;
}

- (BOOL) deleteActiveLayer
{
    BOOL ret = painting->deleteActiveLayer();
    canvas->drawView();
    return ret;
};

- (void) setVisibility:(BOOL)visible ofLayer:(NSInteger) index
{
    int layersNum = painting->getLayersNumber();

    CZLayer *layer = painting->getLayer(layersNum - 1 - int(index));
    layer->setVisiblility(visible);
    canvas->drawView();
}
- (BOOL) isVisibleOfLayer:(NSInteger)index
{
    int layersNum = painting->getLayersNumber();
    
    CZLayer *layer = painting->getLayer(layersNum - 1 - int(index));
    return layer->isVisible();
}
- (void) setLocked:(BOOL)locked ofLayer:(NSInteger) index
{
    int layersNum = painting->getLayersNumber();
    
    CZLayer *layer = painting->getLayer(layersNum - 1 - int(index));
    layer->setLocked(locked);
}
- (BOOL) isLockedofLayer:(NSInteger)index
{
    int layersNum = painting->getLayersNumber();
    
    CZLayer *layer = painting->getLayer(layersNum - 1 - int(index));
    return layer->isLocked();
}
- (void) setActiveLayerOpacity:(float)opacity
{
    CZLayer *layer = painting->getActiveLayer();
    layer->setOpacity(opacity);
    canvas->drawView();
}
- (float) getActiveLayerOpacity
{
    CZLayer *layer = painting->getActiveLayer();
    return layer->getOpacity();
}

/// 析构
- (void) dealloc {
    if (canvas) {
        delete canvas;
        canvas = NULL;
    }
    
    if (painting) {
        delete painting;
        painting = NULL;
    }
}
@end
