//
//  HYDrawingViewController.m
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/16.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "HYDrawingViewController.h"
#import "HYBrushCore.h"
#import "BottomBarView.h"
#import "HYMenuViewController.h"
#import "DDPopoverBackgroundView.h"
#import "Macro.h"
#import "WDColorPickerController.h"
#import "ZXHLayersViewController.h"
#import "ZXHEditableTipsView.h"
#import "CanvasView.h"
#import "CliperView.h"
#import "ZXHShapeBoxController.h"
#import "ZXHCanvasBackgroundController.h"
#import "ZXHPaintingListController.h"
#import "TransformOverlayer.h"
#import "SettingViewController.h"
#import "BrushSizePannelView.h"
#import "MBProgressHUD.h"
#import "PaintingManager.h"
#import "UIImage+Resize.h"

extern NSString *CZActivePaintColorDidChange;
extern NSString* LayersCountChange;

@interface HYDrawingViewController ()<
BottomBarViewDelegate,UIPopoverControllerDelegate,WDColorPickerControllerDelegate,CanvasViewDelegate, BrushSizePannelViewDelegate, PaintingListControllerDelegate,
SettingViewControllerDelegate>
{
    UIPopoverController *popoverController_;
    BottomBarView *bottomBarView;
    BrushSizePannelView *brushSizePannelView;
    UIPopoverController *layersPopoverController;
    UIPopoverController *picturePopoverController;
    UIPopoverController *menuPopoverController;
    // 图层
    ZXHLayersViewController *_layersViewController;
    UIBarButtonItem *pictureItem;
    // 图形
    ZXHShapeBoxController *_shapeBoxController;
    UIPopoverController *_shapeBoxPopoverController;
    // 背景图选择
    UIPopoverController *_canvasBgPopoverController;
    ZXHCanvasBackgroundController *_canvasBackgroundController;
    // 列表
    UIPopoverController *_listPopoverController;
    
    ///
    BottomBarButtonType activeButton;
    
    MBProgressHUD       *hud;
    
    TransformOverlayer *transformOverlayer;
    
}

@property (nonatomic,strong) WDColorPickerController* colorPickerController;
@property (nonatomic,strong) UIPopoverController *settingPopoverController;                 //< seting popover controller
@end

@implementation HYDrawingViewController
{
    UIImage *_choosedImg;
    CGAffineTransform _transinfo;
}


- (void)constrainFullScreenSubview:(UIView *)subview toMatchWithSuperview:(UIView *)superview
{
    
    subview.translatesAutoresizingMaskIntoConstraints = NO;
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(subview);
    
    NSArray *constraints = [NSLayoutConstraint
                            constraintsWithVisualFormat:@"H:|[subview]|"
                            options:0
                            metrics:nil
                            views:viewsDictionary];
    constraints = [constraints arrayByAddingObjectsFromArray:
                   [NSLayoutConstraint
                    constraintsWithVisualFormat:@"V:|[subview]|"
                    options:0
                    metrics:nil
                    views:viewsDictionary]];
    [superview addConstraints:constraints];
}


// 隐藏状态栏
-(BOOL)prefersStatusBarHidden {
    return NO;
}

-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    
    // 全透明背景
    [self.navigationController.navigationBar setBackgroundImage:[UIImage new] forBarMetrics:UIBarMetricsDefault];
    self.navigationController.navigationBar.translucent = YES;
    // 去掉分割线
    self.navigationController.navigationBar.shadowImage = [UIImage new];
    
}

#pragma mark 判断是否可以绘画
- (void)showMessageView:(ShowingMessageType)msgType
{
    NSLog(@"showMessageView");
    NSInteger curLayerIndex = [[HYBrushCore sharedInstance]getActiveLayerIndex];
    BOOL visible = [[HYBrushCore sharedInstance]isVisibleOfLayer:curLayerIndex];
    BOOL locked = [[HYBrushCore sharedInstance]isLockedofLayer:curLayerIndex];
    
    ZXHEditableTipsView *tipsView = [ZXHEditableTipsView defaultTipsView];
    tipsView.visible = visible;
    tipsView.locked = locked;
    [self.view addSubview:tipsView];
    [tipsView showTips];
}

-(void)dismissMessageView{
    NSLog(@"dismissMessageView");
    [[ZXHEditableTipsView defaultTipsView] dismissTips];
}


#pragma mark - Properties

- (WDColorPickerController*) colorPickerController {
    if (!_colorPickerController) {
        _colorPickerController = [[WDColorPickerController alloc] initWithNibName:@"ColorPicker" bundle:nil];
        _colorPickerController.delegate = self;
    }
    return _colorPickerController;
}

- (UIPopoverController*) settingPopoverController {
    if (!_settingPopoverController) {
        
        SettingViewController *settingCtrl =  [[SettingViewController alloc]init];
        settingCtrl.delegate = self;
        _settingPopoverController = [[UIPopoverController alloc] initWithContentViewController:settingCtrl];
        _settingPopoverController.backgroundColor = kBorderColor;
    }
    
    return _settingPopoverController;
}

#pragma mark viewDidLoad
- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 导航栏橙色背景
    //    self.navigationController.navigationBar.barTintColor = kBackgroundColor;
    
    /**
     *  navBar 图片色 iOS7
     */
    self.navigationController.navigationBar.tintColor = [UIColor lightGrayColor];
    
    // 列表
    UIBarButtonItem *menuItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"menu"] style:UIBarButtonItemStylePlain target:self action:@selector(showListPopoverController:)];
    self.navigationItem.leftBarButtonItems = @[menuItem];
    //    menuItem.tintColor = [UIColor lightGrayColor];
    
    // 视频
    UIBarButtonItem *videoItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"video"] style:UIBarButtonItemStylePlain target:self action:@selector(tapVideo:)];
    
    // 设置
    UIBarButtonItem *settingItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"setting"] style:UIBarButtonItemStylePlain target:self action:@selector(showSettingPopoverController:)];
    
    // 图片
    pictureItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"picture"] style:UIBarButtonItemStylePlain target:self action:@selector(showPhotoBrowser:)];
    
    // 分享
    UIBarButtonItem *shareItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"share"] style:UIBarButtonItemStylePlain target:self action:@selector(showShareController:)];
    
    
    self.navigationItem.rightBarButtonItems = @[shareItem,pictureItem,settingItem,videoItem];
    
    self.view.opaque = YES;
    self.view.backgroundColor = [UIColor colorWithWhite:0.2 alpha:1];
    
#pragma mark 初始画板
    
    [[HYBrushCore sharedInstance]initializeWithWidth:kScreenW height:kScreenH scale:[UIScreen mainScreen].scale];
    CanvasView *canvasView = [[HYBrushCore sharedInstance] getPaintingView];
    canvasView.delegate = self;
    [self.view insertSubview:canvasView atIndex:0];
    
    // bottom bar view
    bottomBarView = [[BottomBarView alloc]initWithWellColor:[[HYBrushCore sharedInstance]getActiveStatePaintColor]];
    bottomBarView.delegate = self;
    [self.view addSubview:bottomBarView];
    [self constrainSubview:bottomBarView toMatchWithSuperview:self.view];
    activeButton = bottomBarView.currentButton.tag;
    
    // brush size pannel
    brushSizePannelView = [[BrushSizePannelView alloc] initWithFrame:CGRectMake(-132, 450, 346, 58)];
    brushSizePannelView.delegate = self;
    [self.view addSubview:brushSizePannelView];
    [brushSizePannelView setBrushSize:[[HYBrushCore sharedInstance]getActiveBrushSize]];
    
    NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self selector:@selector(paintColorChanged:) name:CZActivePaintColorDidChange object:nil];
    
    NSLog(@"Home path is %@",NSHomeDirectory());
}

#pragma mark 设置barItem是否可用
-(void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context{
    if ([keyPath isEqualToString:@"layersCount"]) {
        if (_layersViewController.layersCount == 10) {
            pictureItem.enabled = NO;
        }else{
            pictureItem.enabled = YES;
        }
    }
}

-(BOOL)shouldAutorotate {
    return NO;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

//- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation{
//    return  UIInterfaceOrientationLandscapeLeft | UIInterfaceOrientationLandscapeRight;
//}

#pragma mark - Setting
-(void)showSettingPopoverController:(UIBarButtonItem*)sender{
    
    [self.settingPopoverController presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}

#pragma mark SettingViewControllerDelegate

- (BOOL) settingViewControllerSavePainting:(SettingViewController *)settingController {
    //[[HYBrushCore sharedInstance] saveCurrentPainting];
    [_settingPopoverController dismissPopoverAnimated:YES];
    
    return YES;
}

- (BOOL) settingViewControllerClearLayer:(SettingViewController *)settingController {
    [[HYBrushCore sharedInstance]clearLayer:0];
    [_settingPopoverController dismissPopoverAnimated:YES];
    
    return YES;
}

- (BOOL) settingViewControllerTransformLayer:(SettingViewController *)settingController {
    
    [_settingPopoverController dismissPopoverAnimated:YES];
    
    float canvasScale = [[HYBrushCore sharedInstance] getCanvasScale];
    float screenScale = [[UIScreen mainScreen] scale];
    transformOverlayer = [[TransformOverlayer alloc] initWithFrame:self.view.bounds title:@"图层变换" scaleFactor:screenScale / canvasScale];
    
    __weak HYDrawingViewController *weakSelf = self;
    transformOverlayer.cancelBlock = ^{ [weakSelf cancelLayerTransformation];   };
    transformOverlayer.acceptBlock = ^{ [weakSelf transformActiveLayer];    };
    
    [self.view addSubview:transformOverlayer];
    
    [[HYBrushCore sharedInstance]setActiveLayerLinearInterprolation:YES];
    
    [self displayToolView:NO];
    
    [transformOverlayer addTarget:self action:@selector(layerTransformChanged:)
                forControlEvents:UIControlEventValueChanged];

    return YES;
}

#pragma mark - Actions

-(void)showListPopoverController:(UIBarButtonItem*)sender{
    
    UIImage *image = [UIImage imageNamed:@"list_popover_bg"];
    
    if (!_listPopoverController) {
        
        ZXHPaintingListController *listVC = [[ZXHPaintingListController alloc]init];
        listVC.delegate = self;
        listVC.preferredContentSize = CGSizeMake(image.size.width, image.size.height-10);
        _listPopoverController = [[UIPopoverController alloc]initWithContentViewController:listVC];
        
        
    }
    
    _listPopoverController.popoverBackgroundViewClass =[DDPopoverBackgroundView class];
    [DDPopoverBackgroundView setContentInset:0];
    [DDPopoverBackgroundView setBackgroundImage:image];
    [DDPopoverBackgroundView setBackgroundImageCornerRadius:4];
    
    // 弹出
    [_listPopoverController presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
    
    [(ZXHPaintingListController*)_listPopoverController.contentViewController refreshData];
}


- (void)tapVideo:(id)sender{
    
}

- (void)showShareController:(id)sender{
    HYMenuViewController *menuViewController = [[HYMenuViewController alloc]init];
    UINavigationController *menuNavigationController = [[UINavigationController alloc]initWithRootViewController:menuViewController];
    menuNavigationController.navigationBar.barTintColor = kBackgroundColor;
    menuNavigationController.navigationBar.barStyle = UIBarStyleBlackTranslucent;
    
    menuPopoverController = [[UIPopoverController alloc]initWithContentViewController:menuNavigationController];
    
    menuPopoverController.popoverBackgroundViewClass =[DDPopoverBackgroundView class];
    UIImage *image = [UIImage imageNamed:@"menu_popover_bg"];
    [DDPopoverBackgroundView setBackgroundImage:image];
    [DDPopoverBackgroundView setBackgroundImageCornerRadius:2.f];
    [DDPopoverBackgroundView setArrowBase:0];
    [DDPopoverBackgroundView setArrowHeight:2];
    [menuPopoverController setPopoverContentSize:CGSizeMake(image.size.width, image.size.height-20)];
    [menuPopoverController presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}

- (void)showPhotoBrowser:(UIBarButtonItem*) sender {
    if ([self shouldDismissPopoverForClassController:[UIImagePickerController class] insideNavController:NO]) {
        [self hidePopovers];
        return;
    }
    
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    
    picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    picker.delegate = self;
    
    [self showController:picker fromBarButtonItem:sender animated:YES];
}

-(void)camera {
    UIImagePickerController  *picker = [[UIImagePickerController alloc]init];
    UIImagePickerControllerSourceType sourcheType =     UIImagePickerControllerSourceTypeCamera
    ;
    picker.sourceType = sourcheType;
    picker.delegate = self;
    picker.allowsEditing = YES;
    if (iOS(8.0)) {
        [[NSOperationQueue mainQueue]addOperationWithBlock:^{
            [self presentViewController:picker animated:YES completion:nil];
        }];
    }
    else{
        [self presentViewController:picker animated:YES completion:nil];
    }
    
}
-(void)photoAlbum {
    UIImagePickerController  *picker = [[UIImagePickerController alloc]init];
    UIImagePickerControllerSourceType sourcheType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    picker.sourceType = sourcheType;
    picker.delegate = self;
    picker.allowsEditing = YES;
    if (iOS(8.0)) {
        [[NSOperationQueue mainQueue]addOperationWithBlock:^{
            [self presentViewController:picker animated:YES completion:nil];
        }];
    }
    else{
        [self presentViewController:picker animated:YES completion:nil];
    }
}

#pragma mark - HYDrawingViewController

- (NSArray *)constrainSubview:(UIView *)subview toMatchWithSuperview:(UIView *)superview {
    subview.translatesAutoresizingMaskIntoConstraints = NO;
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(subview);
    NSDictionary *metrics = @{@"vHeight":@(103),@"hPadding":@(11)};
    
    NSArray *constraints = [NSLayoutConstraint
                            constraintsWithVisualFormat:@"H:|-hPadding-[subview]-hPadding-|"
                            options:0
                            metrics:metrics
                            views:viewsDictionary];
    constraints = [constraints arrayByAddingObjectsFromArray:
                   [NSLayoutConstraint
                    constraintsWithVisualFormat:@"V:[subview(vHeight)]|"
                    options:0
                    metrics:metrics
                    views:viewsDictionary]];
    [superview addConstraints:constraints];
    
    return constraints;
}

- (void) paintColorChanged:(NSNotification *)aNotification
{
    WDColor *newPaintColor = [aNotification userInfo][@"pickedColor"];
    [self.colorPickerController setColor:newPaintColor];
}

#pragma mark Layer Transform
- (void) layerTransformChanged:(TransformOverlayer *)sender
{
    CGAffineTransform trans = sender.alignedTransform;
    [[HYBrushCore sharedInstance] setActiveLayerTransform: trans];
    
}

- (void) cancelLayerTransformation
{
//    if (!self.controller.runningOnPhone) {
//        [UIView animateWithDuration:UINavigationControllerHideShowBarDuration
//                         animations:^{ transformOverlay_.alpha = 0.0f; }
//                         completion:^(BOOL finished) {
//                             [transformOverlay_ removeFromSuperview];
//                             transformOverlay_ = nil;
//                             
//                             [self.controller showInterface];
//                         }];
//    } else
    {
        [transformOverlayer removeFromSuperview];
        transformOverlayer = nil;
        [self updateLayersView];
        [self displayToolView:YES];
    }
    
    [[HYBrushCore sharedInstance] setActiveLayerTransform:CGAffineTransformIdentity];
    [[HYBrushCore sharedInstance] setActiveLayerLinearInterprolation:NO];
}

- (void) transformActiveLayer
{
//    if (transformOverlay_.horizontalFlip) {
//        changeDocument(self.painting, [WDModifyLayer modifyLayer:self.painting.activeLayer withOperation:WDFlipLayerHorizontal]);
//    }
//    
//    if (transformOverlay_.verticalFlip) {
//        changeDocument(self.painting, [WDModifyLayer modifyLayer:self.painting.activeLayer withOperation:WDFlipLayerVertical]);
//    }
    
//    // only change the doc if the transform actually changed
//    if (!CGAffineTransformIsIdentity(rawLayerTransform)) {
//        changeDocument(self.painting, [WDTransformLayer transformLayer:self.painting.activeLayer transform:rawLayerTransform]);
//    }
    
    [[HYBrushCore sharedInstance] renderActiveLayerWithTransform:transformOverlayer.alignedTransform];
    [self cancelLayerTransformation];
}

#pragma mark Photo Placement
- (void)photoTransformChanged:(TransformOverlayer *)sender
{
    CGAffineTransform trans = sender.alignedTransform;
    [[HYBrushCore sharedInstance] setPhotoTransform: trans];
}

- (void) cancelPhotoPlacement
{
//    if (!self.controller.runningOnPhone) {
//        [UIView animateWithDuration:UINavigationControllerHideShowBarDuration
//                         animations:^{ transformOverlay_.alpha = 0.0f; }
//                         completion:^(BOOL finished) {
//                             [transformOverlay_ removeFromSuperview];
//                             transformOverlay_ = nil;
//                             
//                             [self.controller showInterface];
//                         }];
//    } else
    {
        [transformOverlayer removeFromSuperview];
        transformOverlayer = nil;
        [self updateLayersView];
        [self displayToolView:YES];
    }
    
    [[HYBrushCore sharedInstance]endPhotoPlacement:NO];
}

- (void) placePhoto
{
    
    {
        [transformOverlayer removeFromSuperview];
        transformOverlayer = nil;
        [self updateLayersView];
        [self displayToolView:YES];
    }
    
    [[HYBrushCore sharedInstance]endPhotoPlacement:YES];
}

#pragma mark - BottomBarViewDelegate

- (void)bottomBarView:(BottomBarView*)bottomBarView forButtonAction:(UIButton*)button {
    switch (button.tag) {
        case COLORWHEEL_BTN:        ///< 调色板
            [self showColorPicker:button];
            break;
        case ERASER_BTN:            ///< 橡皮擦
            [[HYBrushCore sharedInstance]activeEraser];
            [brushSizePannelView setBrushSize:[[HYBrushCore sharedInstance]getActiveBrushSize]];
            break;
        case PENCIL_BTN:            ///< 铅笔
            [[HYBrushCore sharedInstance]activePencil];
            [brushSizePannelView setBrushSize:[[HYBrushCore sharedInstance]getActiveBrushSize]];
            break;
        case MARKERPEN_BTN:         ///< 马克笔
            [[HYBrushCore sharedInstance]activeMarker];
            [brushSizePannelView setBrushSize:[[HYBrushCore sharedInstance]getActiveBrushSize]];
            break;
        case COLORBRUSH_BTN:        ///< 水彩笔
            [[HYBrushCore sharedInstance]activeWaterColorPen];
            [brushSizePannelView setBrushSize:[[HYBrushCore sharedInstance]getActiveBrushSize]];
            break;
        case CRAYON_BTN:
            [[HYBrushCore sharedInstance]activeCrayon];
            [brushSizePannelView setBrushSize:[[HYBrushCore sharedInstance]getActiveBrushSize]];
            break;
        case BUCKET_BTN:            ///< 倒色桶
            [[HYBrushCore sharedInstance]activeBucket];
            break;
        case SHAPEBOX_BTN:          ///< 图形箱
            [self showShapeBoxPopoverController:button];
            break;
        case EYEDROPPER_BTN:        ///< 取色管
            [[HYBrushCore sharedInstance]activeColorPicker];
            break;
        case LAYERS_BTN:            ///< 图层
            [self showLayerPopoverController:button];
            break;
        case CLIP_BTN:              ///< 裁减
            //            [self showCliperView];
            break;
        case CANVAS_BTN:            ///< 背景图
            [self showCanvasBackgroundPopoverController:button];
            break;
        default:
            break;
    }
    
    activeButton = (BottomBarButtonType)button.tag;
    [self displayBrushSizePannel:YES];
    
}

- (void) showColorPicker:(id)sender {
    if ([self shouldDismissPopoverForClassController:[WDColorPickerController class] insideNavController:NO]) {
        [self hidePopovers];
        return;
    }
    
    [self showController:self.colorPickerController fromBarButtonItem:sender animated:NO];
}

- (void)showCanvasBackgroundPopoverController:(UIButton*)sender {
    UIImage *image = [UIImage imageNamed:@"canvasBg_bg"];
    if (!_canvasBackgroundController) {
        _canvasBackgroundController = [[ZXHCanvasBackgroundController alloc]initWithPreferredContentSize:CGSizeMake(image.size.width, image.size.height)];
    }
    
    if (!_canvasBgPopoverController) {
        _canvasBgPopoverController = [[UIPopoverController alloc]initWithContentViewController:_canvasBackgroundController];
    }
    
    _canvasBgPopoverController.popoverBackgroundViewClass =[DDPopoverBackgroundView class];
    [DDPopoverBackgroundView setContentInset:0];
    //
    [DDPopoverBackgroundView setBackgroundImage:image];
    
    // 弹出位置
    CGRect popRect = sender.frame;
    popRect.origin.x -= popRect.size.width;
    
    [_canvasBgPopoverController presentPopoverFromRect:popRect inView:bottomBarView permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
}

-(void)showLayerPopoverController:(UIButton*)sender {
    if (!_layersViewController) {
        _layersViewController = [ZXHLayersViewController new];
    }
    
    if (!layersPopoverController) {
        layersPopoverController = [[UIPopoverController alloc]initWithContentViewController:_layersViewController];
    }
    
    layersPopoverController.popoverBackgroundViewClass =[DDPopoverBackgroundView class];
    [DDPopoverBackgroundView setContentInset:2];
    
    UIImage *image = [UIImage imageNamed:@"layers_popover_bg"];
    [DDPopoverBackgroundView setBackgroundImage:image];
    [DDPopoverBackgroundView setBackgroundImageCornerRadius:1.f];
    [DDPopoverBackgroundView setArrowBase:0];
    [DDPopoverBackgroundView setArrowHeight:2];
    [layersPopoverController setPopoverContentSize:CGSizeMake(image.size.width, image.size.height-30)];
    
    // 弹出位置
    CGRect popRect = sender.frame;
    popRect.origin.y -= 10;
    
    [layersPopoverController presentPopoverFromRect:popRect inView:bottomBarView permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
    
    [_layersViewController addObserver:self forKeyPath:@"layersCount" options:NSKeyValueObservingOptionNew|NSKeyValueObservingOptionOld context:nil];
}

#pragma mark 形状选择弹出
-(void)didSelectedShape:(UIImage*)img{
    [_shapeBoxPopoverController dismissPopoverAnimated:YES];
   // [self showImageEditViewWithImage:img];
}

-(void)showShapeBoxPopoverController:(UIButton*)sender{
    UIImage *image = [UIImage imageNamed:@"shapebox_img_bg"];
    if (!_shapeBoxController) {
        _shapeBoxController = [[ZXHShapeBoxController alloc]initWithPreferredContentSize:image.size];
        _shapeBoxController.delegate = self;
    }
    
    if (!_shapeBoxPopoverController) {
        _shapeBoxPopoverController = [[UIPopoverController alloc]initWithContentViewController:_shapeBoxController];
    }
    
    _shapeBoxPopoverController.popoverBackgroundViewClass =[DDPopoverBackgroundView class];
    [DDPopoverBackgroundView setContentInset:0];
    [DDPopoverBackgroundView setBackgroundImage:[UIImage new]];
    [DDPopoverBackgroundView setBackgroundImageCornerRadius:4];
    
    // 弹出位置
    CGRect popRect = sender.frame;
    popRect.origin.x += popRect.size.width;
    
    [_shapeBoxPopoverController presentPopoverFromRect:popRect inView:bottomBarView permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
}



#pragma mark 显示裁剪视图
-(void)showCliperView{
    CliperView *cliper = [[CliperView alloc]initWithFrame:self.view.frame];
    [self.view addSubview:cliper];
}

#pragma mark - WDColorPickerControllerDelegate Methods
- (void) dismissViewController:(UIViewController *)viewController
{
    if (popoverController_) {
        [self hidePopovers];
    } else {
        [viewController.presentingViewController dismissViewControllerAnimated:YES completion:nil];
    }
}

- (void) setActiveStateColor:(WDColor*)color from:(WDColorPickerController*) colorPickerController
{
    [[HYBrushCore sharedInstance] setActiveStateColor:[color UIColor]];
    bottomBarView.colorWheelButton.color = color;
}

#pragma mark UIActionSheetDelegate
- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex{
    switch (buttonIndex) {
        case 0:
            [self camera];
            break;
        case 1:
            [self photoAlbum];
            break;
            
        default:
            break;
    }
}

#pragma mark BrushSizePannelViewDelegate
- (void)brushSizePannelView:(BrushSizePannelView *)brushSizePannelView valueChanged:(float)value
{
    [[HYBrushCore sharedInstance] setActiveBrushSize:value];
}

#pragma mark PaintingListControllerDelegate
- (void)paintingListController:(ZXHPaintingListController *)paintingListCtrl didSelectAt:(NSInteger)index
{
    if (!hud) {
        hud = [[MBProgressHUD alloc]initWithView:self.view];
        hud.labelText = @"正在切换绘画...";
        [self.view addSubview:hud];
    }
    
    [_listPopoverController dismissPopoverAnimated:YES];
    
    [self.view setUserInteractionEnabled:NO];
    [hud showAnimated:YES whileExecutingBlock:^{
        [[PaintingManager sharedInstance]loadPaintingAt:index];
    } completionBlock:^ {
        NSLog(@"finish loading");
        [self.view setUserInteractionEnabled:YES];
        [[HYBrushCore sharedInstance]draw];
        [[NSNotificationCenter defaultCenter]postNotificationName:LayersCountChange object:nil userInfo:nil];
    }];
    
}

#pragma mark UIImagePickerControllerDelegate

- (void) dismissImagePicker:(UIImagePickerController *)picker {
//    if (self.runningOnPhone) {
//        [[picker presentingViewController] dismissViewControllerAnimated:YES completion:nil];
//    } else {
        [popoverController_ dismissPopoverAnimated:YES];
        popoverController_ = nil;
//    }
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingImage:(UIImage *)image editingInfo:(NSDictionary *)editingInfo {
    [self dismissImagePicker:picker];
    
    CGSize imageSize = image.size;
    if (imageSize.width > imageSize.height) {
        if (imageSize.width > 2048) {
            imageSize.height = (imageSize.height / imageSize.width) * 2048;
            imageSize.width = 2048;
        }
    } else {
        if (imageSize.height > 2048) {
            imageSize.width = (imageSize.width / imageSize.height) * 2048;
            imageSize.height = 2048;
        }
    }

    image = [image resizedImage:imageSize interpolationQuality:kCGInterpolationHigh];

    // create transform overlayer of image
    float canvasScale = [[HYBrushCore sharedInstance] getCanvasScale];
    float screenScale = [[UIScreen mainScreen] scale];
    transformOverlayer = [[TransformOverlayer alloc] initWithFrame:self.view.bounds title:@"图片插入" scaleFactor:screenScale / canvasScale];
    
    __weak HYDrawingViewController *weakSelf = self;
    transformOverlayer.cancelBlock = ^{ [weakSelf cancelPhotoPlacement];    };
    transformOverlayer.acceptBlock = ^{ [weakSelf placePhoto];  };
    
    [transformOverlayer addTarget:self action:@selector(photoTransformChanged:)
                 forControlEvents:UIControlEventValueChanged];
    
    [self.view addSubview:transformOverlayer];
    
    [[HYBrushCore sharedInstance]setActiveLayerLinearInterprolation:YES];
    [[HYBrushCore sharedInstance]beginPhotoPlacement:image withTransform:[transformOverlayer configureInitialPhotoTransform:image]];
    
    [self displayToolView:NO];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [self dismissImagePicker:picker];
}


#pragma mark CanvasViewDelegate Methods

- (void)displayToolView:(BOOL) flag
{
    if (!flag && !bottomBarView.hidden) {
        [UIView animateWithDuration:0.25 animations:^{
            bottomBarView.alpha = 0;
            self.navigationController.navigationBar.alpha = 0;
            [self displayBrushSizePannel:NO];
        }completion:^(BOOL finished) {
            bottomBarView.hidden = YES;
        }];
    }
    else if(flag && bottomBarView.hidden) {
        [UIView animateWithDuration:0.25 animations:^{
            bottomBarView.alpha = 1;
            self.navigationController.navigationBar.alpha = 1;
            [self displayBrushSizePannel:YES];
        } completion:^(BOOL finished) {
            bottomBarView.hidden = NO;
        }];
    }
}

#pragma mark - Private Methods

- (void)displayBrushSizePannel:(BOOL) flag {
    if (!flag)
    {
        [brushSizePannelView setAlpha:0.0f];
        return;
    }
    
    switch (activeButton) {
        case ERASER_BTN:            ///< 橡皮擦
            [brushSizePannelView setAlpha:1.0f];
            break;
        case PENCIL_BTN:            ///< 铅笔
            [brushSizePannelView setAlpha:1.0f];
            break;
        case MARKERPEN_BTN:         ///< 马克笔
            [brushSizePannelView setAlpha:1.0f];
            break;
        case COLORBRUSH_BTN:        ///< 水彩笔
            [brushSizePannelView setAlpha:1.0f];
            break;
        case CRAYON_BTN:
            [brushSizePannelView setAlpha:1.0f];
            break;
        case BUCKET_BTN:            ///< 倒色桶
            [brushSizePannelView setAlpha:0.0f];
            break;
        case SHAPEBOX_BTN:          ///< 图形箱
            [brushSizePannelView setAlpha:0.0f];
            break;
        case EYEDROPPER_BTN:        ///< 取色管
            [brushSizePannelView setAlpha:0.0f];
            break;
        case CLIP_BTN:              ///< 裁减
            //            [self showCliperView];
            break;
        case CANVAS_BTN:            ///< 背景图
            [brushSizePannelView setAlpha:0.0f];
            break;
        default:
            break;
    }
}

- (void)updateLayersView
{
    _layersViewController.layersCount = [[HYBrushCore sharedInstance]getLayersNumber];
    [_layersViewController.tbView reloadData];
    
    [self displayToolView:YES];
}


#pragma mark - Copied directly

- (BOOL) shouldDismissPopoverForClassController:(Class)controllerClass insideNavController:(BOOL)insideNav
{
    if (!popoverController_) {
        return NO;
    }
    
    if (insideNav && [popoverController_.contentViewController isKindOfClass:[UINavigationController class]]) {
        NSArray *viewControllers = [(UINavigationController *)popoverController_.contentViewController viewControllers];
        
        for (UIViewController *viewController in viewControllers) {
            if ([viewController isKindOfClass:controllerClass]) {
                return YES;
            }
        }
    } else if ([popoverController_.contentViewController isKindOfClass:controllerClass]) {
        return YES;
    }
    
    return NO;
}

- (void) showController:(UIViewController *)controller fromBarButtonItem:(UIBarButtonItem *)barButton animated:(BOOL)animated
{
    [self runPopoverWithController:controller from:barButton];
}

- (UIPopoverController *) runPopoverWithController:(UIViewController *)controller from:(id)sender
{
    [self hidePopovers];
    
    popoverController_ = [[UIPopoverController alloc] initWithContentViewController:controller];
    popoverController_.delegate = self;
    
    //    NSMutableArray *passthroughs = [NSMutableArray arrayWithObjects:self.topBar, self.bottomBar, nil];
    //    if (self.isEditing) {
    //        [passthroughs addObject:self.canvas];
    //    }
    //    popoverController_.passthroughViews = passthroughs;
    //
    if ([sender isKindOfClass:[UIBarButtonItem class]]) {
        [popoverController_ presentPopoverFromBarButtonItem:sender
                                   permittedArrowDirections:UIPopoverArrowDirectionAny
                                                   animated:YES];
    } else {
        [popoverController_ presentPopoverFromRect:CGRectInset(((UIView *) sender).bounds, 10, 10)
                                            inView:sender
                          permittedArrowDirections:(UIPopoverArrowDirectionUp | UIPopoverArrowDirectionDown)
                                          animated:YES];
    }
    
    return popoverController_;
}

- (BOOL) popoverVisible
{
    return popoverController_ ? YES : NO;
}

- (void) hidePopovers
{
    if (popoverController_) {
        [popoverController_ dismissPopoverAnimated:NO];
        popoverController_ = nil;
    }
}

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController
{
    if (popoverController == popoverController_) {
        popoverController_ = nil;
    }
}

@end
