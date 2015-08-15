//
//  CZViewImpl.h
//  HYDrawing
//
//  Created by CharlyZhang on 15/8/12.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#ifndef __HYDrawing__CZViewImpl__
#define __HYDrawing__CZViewImpl__

#import <UIKit/UIKit.h>

#include "CZCanvas.h"
#include "painting/CZPainting.h"
#include "basic/CZMat4.h"

/// 显示的遮罩消息类型
typedef enum ShowingMessageType {
    kInvisible = 0,
    kLocked
} ShowingMessageType;

@protocol CanvasViewDelegate <NSObject>

@required
- (void) showMessageView:(ShowingMessageType)msgType;

@end

@interface CanvasView : UIView<UIGestureRecognizerDelegate>

@property (nonatomic, assign) CZPainting* ptrPainting;
@property (nonatomic, weak) id<CanvasViewDelegate> delegate;

- (void)drawView;

@end

/// CZView, ∆ΩÃ®œ‡πÿµƒ ”Õº
class CZViewImpl : public CZView
{
public:
    CanvasView *realView;
    
    CZViewImpl(const CZRect rect);
    
    ~CZViewImpl();
    void setPaiting(CZPainting* p);
    void draw();
};

#endif /* defined(__HYDrawing__CZViewImpl__) */
