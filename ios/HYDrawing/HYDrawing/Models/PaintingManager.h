//
//  PaintingManager.h
//  HYDrawing
//
//  Created by CharlyZhang on 15/12/12.
//  Copyright © 2015年 Founder. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PaintingManager : NSObject

@property(nonatomic) NSInteger activePaintingIndex;
@property (nonatomic) BOOL hasInitialized;

+ (PaintingManager*) sharedInstance;

///初始化
- (BOOL) initializeWithWidth:(float)w height:(float)h;

- (void)refreshData;

- (NSInteger) getPaintingNumber;
- (NSString*) getPaintingNameAt:(NSInteger)index;

/// load the painting at particular index
///
///     \note will change the 'activePaintingIndex'
- (BOOL) loadPaintingAt:(NSInteger)index;

- (BOOL) createNewPainting;

/// get inital painting when lauching app
- (void*) getInitialPainting;

@end
