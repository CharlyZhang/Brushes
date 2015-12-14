//
//  PaintingManager.m
//  HYDrawing
//
//  Created by CharlyZhang on 15/12/12.
//  Copyright © 2015年 Founder. All rights reserved.
//

#import "PaintingManager.h"
#include "BrushesCore.h"
#include "CZFileManager.h"

#define DOCUMENT_DIR            [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"]
#define PAINTING_FILE_EXTENTION @"painting"
#define PAINTING_DEFAULT_NAME   @"Painting"

@interface PaintingManager()

@property (nonatomic,strong)NSString* defaultFileName;

@end

@implementation PaintingManager
{
    NSArray *paintingNames;
    
    NSString *activePaintingName;
    
    CZPainting* ptrActivePainting;
    
    float width,height;
}

+ (PaintingManager *) sharedInstance
{
    static PaintingManager *paintingManager_ = nil;
    
    if (!paintingManager_) {
        paintingManager_ = [[PaintingManager alloc] init];
    }
    
    return paintingManager_;
}


/// 初始化
- (BOOL) initializeWithWidth:(float)w height:(float)h
{
    width = w;
    height = h;
    self.hasInitialized = YES;
    ptrActivePainting = nullptr;
    
    return YES;
}

- (NSInteger) activePaintingIndex
{
    return [paintingNames indexOfObject:activePaintingName];
}

- (NSInteger) getPaintingNumber
{
    return paintingNames.count;
}

- (NSString*) getPaintingNameAt:(NSInteger)index
{
    return [[paintingNames objectAtIndex:index] stringByDeletingPathExtension];
}

/// 以下函数改变当前激活的Painting
- (BOOL) loadPaintingAt:(NSInteger)index
{
    if (!self.hasInitialized) {
        LOG_ERROR("Painting manager has not been initialized!\n");
        return NO;
    }
    
    if (index < 0 || index >= paintingNames.count) {
        LOG_ERROR("index is out of range\n");
        return NO;
    }
    
    [self saveActivePainting];
    
    activePaintingName = [paintingNames objectAtIndex:index];
    CZFileManager::getInstance()->loadPainting([[self filePathofName:activePaintingName]UTF8String], ptrActivePainting);
    
    return YES;
}

/// get inital painting when lauching app
- (void*) getInitialPainting
{
    if (!self.hasInitialized) {
        LOG_ERROR("Painting manager has not been initialized!\n");
        return nil;
    }
    
    [self refreshData];
    
    void *ret = nullptr;
    
    NSString *URLString = [[NSUserDefaults standardUserDefaults] objectForKey:@"lauchPaintingUrl"];
    
    if (URLString) {
        URLString = [URLString substringFromIndex:7];       // strip the "file://"
        ptrActivePainting = CZFileManager::getInstance()->createPainting([URLString UTF8String],width,height);
        ret = ptrActivePainting;
        activePaintingName = self.defaultFileName;
        [self saveActivePainting];
        [self refreshData];
        [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"lauchPaintingUrl"];
    }
    else if ([self getPaintingNumber] <= 0) {
        ptrActivePainting = new CZPainting(CZSize(width,height));
        ret = ptrActivePainting;
    }
    else {
        activePaintingName = [paintingNames objectAtIndex:0];
        ptrActivePainting = CZFileManager::getInstance()->createPainting([[self filePathofName:activePaintingName] UTF8String],width,height);
        ret = ptrActivePainting;
    }
    
    return ret;
}

- (BOOL) createNewPainting
{
    if (!self.hasInitialized) {
        LOG_ERROR("Painting manager has not been initialized!\n");
        return NO;
    }
    
    if(!ptrActivePainting->restore()) return NO;
    activePaintingName = self.defaultFileName;
    [self saveActivePainting];
    [self refreshData];
    
    return YES;
}

- (void)refreshData
{
    paintingNames = [[[NSFileManager defaultManager] contentsOfDirectoryAtPath:DOCUMENT_DIR error:nil] pathsMatchingExtensions:[NSArray arrayWithObjects:PAINTING_FILE_EXTENTION,nil]];
    // TO DO: 排序
}

#pragma mark Private

- (NSString*) defaultFileName
{
    NSFileManager *fm = [NSFileManager defaultManager];
    int defaultFileNo = 0;
    _defaultFileName = [NSString stringWithFormat:@"%@ %d.%@",PAINTING_DEFAULT_NAME, defaultFileNo, PAINTING_FILE_EXTENTION];
    BOOL isDir;
    while ([fm fileExistsAtPath: [self filePathofName:_defaultFileName] isDirectory:&isDir]) {
        defaultFileNo ++;
         _defaultFileName = [NSString stringWithFormat:@"%@ %d.%@",PAINTING_DEFAULT_NAME, defaultFileNo, PAINTING_FILE_EXTENTION];
    }
            
    return _defaultFileName;
}

- (NSString*) filePathofName:(NSString*)filename
{
    return [DOCUMENT_DIR stringByAppendingPathComponent:filename];
}

- (BOOL) saveActivePainting
{
    if (ptrActivePainting) {
        return CZFileManager::getInstance()->savePainting(ptrActivePainting, [[self filePathofName:activePaintingName] UTF8String]);
    }
    return NO;
}

@end
