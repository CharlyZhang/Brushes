//
//  WDColorPickerController.h
//  Brushes
//
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
//  Copyright (c) 2011-2013 Steve Sprang
//

#import <UIKit/UIKit.h>
#import "WDSwatches.h"

@class WDColor;
@class WDColorComparator;
@class WDColorSquare;
@class WDColorWheel;
@class WDColorSlider;
@class WDMatrix;

@class WDColorPickerController;

@protocol WDColorPickerControllerDelegate <NSObject>

- (void) dismissViewController:(UIViewController *)viewController;
- (void) setActiveStateColor:(UIColor*)color from:(WDColorPickerController*) colorPickerController;
- (void) setActiveStateSwatchColor:(UIColor*)color atIndex:(NSUInteger)index from:(WDColorPickerController*) colorPickerController;
- (void) setActiveStatePaintColorAtIndex:(NSUInteger)index from:(WDColorPickerController*) colorPickerController;
- (UIColor*) getColorFromActiveStateSwatchAtIndex:(NSUInteger)index from:(WDColorPickerController*) colorPickerController;

@end
@interface WDColorPickerController : UIViewController <WDSwatchesDelegate>

@property (nonatomic) WDColor *color;
@property (nonatomic) IBOutlet WDColorComparator *colorComparator;
@property (nonatomic) IBOutlet WDColorWheel *colorWheel;
@property (nonatomic) IBOutlet WDColorSquare *colorSquare;
@property (nonatomic) IBOutlet WDSwatches *swatches;
@property (nonatomic) IBOutlet WDColorSlider *alphaSlider;

@property (nonatomic, weak) id<WDColorPickerControllerDelegate> delegate;

- (IBAction)dismiss:(id)sender;
- (void) setInitialColorWithRed:(float)r green:(float)g blue:(float)b alpha:(float)a;

@end
