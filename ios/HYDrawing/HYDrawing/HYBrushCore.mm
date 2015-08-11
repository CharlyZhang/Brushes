//
//  HYBrushCore.m
//  HYDrawing
//
//  Created by CharlyZhang on 15/8/10.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "HYBrushCore.h"

#include "BrushesCore.h"

@interface HYBrushCore()
{
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
    canvas = new CZCanvas(CZRect(0,0,w,h));
    painting = new CZPainting(CZSize(w,h));
    canvas->setPaiting(painting);
    CZActiveState::getInstance()->setEraseMode(false);
    CZActiveState::getInstance()->setActiveBrush(kPencil);
    
    self.hasInitialized = YES;
    return YES;
}

///获得绘制视图
- (UIView*) getPaintingView
{
    return (__bridge UIView*)canvas->getView();
}


///激活橡皮
- (void) activeEraser
{
    CZActiveState::getInstance()->setEraseMode(true);
    CZActiveState::getInstance()->setActiveBrush(kEraser);
}
///激活铅笔
- (void) activePencil
{
    CZActiveState::getInstance()->setEraseMode(false);
    CZActiveState::getInstance()->setActiveBrush(kPencil);
}
///激活蜡笔
- (void) activeCrayon
{
    CZActiveState::getInstance()->setEraseMode(false);
    CZActiveState::getInstance()->setActiveBrush(kCrayon);
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
- (void) setActiveStateSwatchColor:(WDColor*)color atIndex:(NSUInteger)index
{
    if(color){
        CZColor *c = new CZColor(color.red,color.green,color.blue,color.alpha);
        CZActiveState::getInstance()->setSwatch(c, (int)index);
    }
    else {
        CZActiveState::getInstance()->setSwatch(NULL, (int)index);
    }
}
- (void) setActiveStatePaintColorAtIndex:(NSUInteger)index
{
    CZActiveState::getInstance()->setPaintColorAsSwatch(int(index));
}

- (WDColor*) getColorFromActiveStateSwatchAtIndex:(NSUInteger)index
{
    CZColor *c = CZActiveState::getInstance()->getSwatch((int)index);
    if (c)  return [WDColor colorWithRed:c->red green:c->green blue:c->blue alpha:c->alpha];
    else    return nil;
}

///绘制图片
- (void)renderImage:(UIImage*)image withTranslate:(CGPoint)vectT rotate:(float)angel scale:(float)s;
{
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
    
//    s = 1;
    angel = 0;
    vectT = CGPointMake(0, 0);
    // 先上下翻转，再变换
    CZAffineTransform trans_scale = CZAffineTransform::makeFromScale(s, s);
    CZAffineTransform trans_rotate = CZAffineTransform::makeFromRotate(angel);
    CZAffineTransform trans_translate = CZAffineTransform::makeFromTranslation(vectT.x, vectT.y);
    
    CZAffineTransform trans_flip = CZAffineTransform::makeFromScale(1, -1);
    CZAffineTransform trans_adjust = CZAffineTransform::makeFromTranslation(-(width/2.0), -(height/2.0));
    CZAffineTransform trans_recover = CZAffineTransform::makeFromTranslation((width/2.0), (height/2.0));
    
    CZAffineTransform trans = trans_adjust * trans_flip * trans_rotate * trans_translate * trans_scale * trans_recover;
    painting->getActiveLayer()->renderImage(brushImg, trans);
    canvas->drawView();
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
