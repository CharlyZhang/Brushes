//
//  PaintingManager.h
//  HYDrawing
//
//  Created by CharlyZhang on 15/12/12.
//  Copyright © 2015年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface PaintingManager : NSObject

@property(nonatomic) NSInteger activePaintingIndex;
@property (nonatomic) BOOL hasInitialized;

+ (PaintingManager*) sharedInstance;

///初始化
- (BOOL) initializeWithWidth:(float)w height:(float)h scale:(float)s;

- (void)refreshData;

- (NSInteger) getPaintingNumber;
- (NSString*) getPaintingNameAt:(NSInteger)index;

/// load the painting at particular index
///
///     \note will change the 'activePaintingIndex'
- (BOOL) loadPaintingAt:(NSInteger)index;

- (BOOL) createNewPainting;

- (BOOL) deletePaintingAt:(NSInteger)index;

- (BOOL) saveActivePainting;

// 生成预览图
- (UIImage*) previewImageOfPaintingAt: (NSInteger)index;

/// get inital painting when lauching app
- (void*) getInitialPainting;

/// 对外接口-最新
-(void*) getInitialPaintingWithPath: (NSString*)path;
-(BOOL) loadPaintingWithPath: (NSString*)path;
-(BOOL) createNewPaintingWithPath: (NSString*)path;
-(BOOL) deletePaintingWithPath: (NSString*)path;
-(BOOL) saveActivePaintingWithPath: (NSString*)path;
-(UIImage*) previewImageOfPaintingWithPath: (NSString*)path;

@end
