//
//  Actions.m
//  HYDrawing
//
//  Created by CharlyZhang on 16/3/30.
//  Copyright © 2016年 Founder. All rights reserved.
//

#import "Actions.h"

@implementation Actions

- (instancetype) initWithType:(ActionType)type {
    if (self = [super init]) {
        _type = type;
    }
    return self;
}

+ (Actions*) createCanvasChangingAction:(NSInteger) layerIdx {
    Actions *newAction = [[Actions alloc]initWithType:kCanvasChanging];
    newAction.activeLayerIdx = layerIdx;
    return newAction;
}

+ (Actions*) createAddingLayerAction {
    return nil;
}

@end
