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
#import "ImageEditViewController.h"
#import "Macro.h"
#import "WDColorPickerController.h"
#import "ZXHLayersViewController.h"
#import "ZXHEditableTipsView.h"
#import "CanvasView.h"
#import "CliperView.h"
#import "ZXHShapeBoxController.h"
#import "ZXHCanvasBackgroundController.h"
#import "ZXHSettingViewController.h"
#import "ZXHPaintingListController.h"

extern NSString *CZActivePaintColorDidChange;

@interface HYDrawingViewController ()<
    BottomBarViewDelegate,UIPopoverControllerDelegate,WDColorPickerControllerDelegate,CanvasViewDelegate,ImageEditViewControllerDelegate,
    SettingViewControllerDelegate>
{
    UIPopoverController *popoverController_;
    BottomBarView *bottomBarView;
    ImageEditViewController *imageEditViewController;
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
    // 设置
    UIPopoverController *_settingPopoverController;
    // 列表
    UIPopoverController *_listPopoverController;
}

@property (nonatomic,strong) WDColorPickerController* colorPickerController;
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
-(BOOL)prefersStatusBarHidden{
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
    [[ZXHEditableTipsView defaultTipsView] dismissTips];
}

#pragma mark 显示或隐藏底部工具栏
-(void)displayBarView:(BOOL) flag {
    if (!flag && !bottomBarView.hidden) {
        [UIView animateWithDuration:0.25 animations:^{
            bottomBarView.alpha = 0;
            self.navigationController.navigationBar.alpha = 0;
        }completion:^(BOOL finished) {
            bottomBarView.hidden = YES;
            self.navigationController.navigationBar.hidden = YES;
        }];
    }
    else if(flag && bottomBarView.hidden) {
        [UIView animateWithDuration:0.25 animations:^{
            bottomBarView.hidden = NO;
            self.navigationController.navigationBar.hidden = NO;
            bottomBarView.alpha = 1;
            self.navigationController.navigationBar.alpha = 1;
        }];
    }
}

#pragma mark - Properties

- (WDColorPickerController*) colorPickerController {
    if (!_colorPickerController) {
        _colorPickerController = [[WDColorPickerController alloc] initWithNibName:@"ColorPicker" bundle:nil];
        _colorPickerController.delegate = self;
    }
    return _colorPickerController;
}


#pragma mark viewDidLoad
- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 导航栏橙色背景
//    self.navigationController.navigationBar.barTintColor = UIPopoverBackgroundColor;
    
    // 列表
    UIBarButtonItem *menuItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"menu"] style:UIBarButtonItemStylePlain target:self action:@selector(showListPopoverController:)];
    self.navigationItem.leftBarButtonItems = @[menuItem];
    
    // 视频
    UIBarButtonItem *videoItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"video"] style:UIBarButtonItemStylePlain target:self action:@selector(tapVideo:)];
    
    // 设置
    UIBarButtonItem *settingItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"setting"] style:UIBarButtonItemStylePlain target:self action:@selector(showSettingPopoverController:)];
    
    // 图片
    pictureItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"picture"] style:UIBarButtonItemStylePlain target:self action:@selector(tapPicture:)];
    
    // 分享
    UIBarButtonItem *shareItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"share"] style:UIBarButtonItemStylePlain target:self action:@selector(tapMenu:)];
    
    
    self.navigationItem.rightBarButtonItems = @[shareItem,pictureItem,settingItem,videoItem];
    
#pragma mark 初始画板

    [[HYBrushCore sharedInstance]initializeWithWidth:kScreenW height:kScreenH];
    CanvasView *canvasView = [[HYBrushCore sharedInstance] getPaintingView];
    canvasView.delegate = self;
    [self.view insertSubview:canvasView atIndex:0];
    
    // bottom bar view
    bottomBarView = [[BottomBarView alloc]initWithWellColor:[[HYBrushCore sharedInstance]getActiveStatePaintColor]];
    bottomBarView.delegate = self;
    [self.view addSubview:bottomBarView];
    [self constrainSubview:bottomBarView toMatchWithSuperview:self.view];
    
    NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self selector:@selector(paintColorChanged:) name:CZActivePaintColorDidChange object:nil];
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

-(BOOL)shouldAutorotate{
    return NO;
}

//- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation{
//    return  UIInterfaceOrientationLandscapeLeft | UIInterfaceOrientationLandscapeRight;
//}

#pragma mark 设置按钮弹出
-(void)showSettingPopoverController:(UIBarButtonItem*)sender{
    UIImage *image = [UIImage imageNamed:@"setting_popover_bg"];
    ZXHSettingViewController *settingVC = [[ZXHSettingViewController alloc]init];
    settingVC.preferredContentSize = image.size;
    settingVC.delegate = self;
    
    if (!_settingPopoverController) {
        _settingPopoverController = [[UIPopoverController alloc]initWithContentViewController:settingVC];
    }
    
    _settingPopoverController.popoverBackgroundViewClass =[DDPopoverBackgroundView class];
    [DDPopoverBackgroundView setContentInset:0];
    [DDPopoverBackgroundView setBackgroundImage:image];
    [DDPopoverBackgroundView setBackgroundImageCornerRadius:4];
    
    // 弹出
    [_settingPopoverController presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}

// 协议方法
-(void)settingForCreateNewCanvas{
    
    [_settingPopoverController dismissPopoverAnimated:YES];
}

-(void)settingForClearCanvas{
    [[HYBrushCore sharedInstance]clearLayer:0];
    
    [_settingPopoverController dismissPopoverAnimated:YES];
}

-(void)settingForTransformCanvas{
    
    [_settingPopoverController dismissPopoverAnimated:YES];
}

#pragma mark 作品列表弹出
-(void)showListPopoverController:(UIBarButtonItem*)sender{
    UIImage *image = [UIImage imageNamed:@"list_popover_bg"];
    ZXHPaintingListController *listVC = [[ZXHPaintingListController alloc]init];
    listVC.preferredContentSize = CGSizeMake(image.size.width, image.size.height-10);
    
    if (!_listPopoverController) {
        _listPopoverController = [[UIPopoverController alloc]initWithContentViewController:listVC];
    }
    
    _listPopoverController.popoverBackgroundViewClass =[DDPopoverBackgroundView class];
    [DDPopoverBackgroundView setContentInset:0];
    [DDPopoverBackgroundView setBackgroundImage:image];
    [DDPopoverBackgroundView setBackgroundImageCornerRadius:4];
    
    // 弹出
    [_listPopoverController presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}


-(void)hiddenNavBar{
    self.navigationController.navigationBar.hidden = NO;
}

// --- 视频
-(void)tapVideo:(id)sender{
    
}

- (void)tapMenu:(id)sender{
    HYMenuViewController *menuViewController = [[HYMenuViewController alloc]init];
    UINavigationController *menuNavigationController = [[UINavigationController alloc]initWithRootViewController:menuViewController];
    menuNavigationController.navigationBar.barTintColor = UIPopoverBackgroundColor;
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

#pragma mark 弹出相册图片选择
- (void)tapPicture:(UIBarButtonItem*)sender{
    
    UIImagePickerController  *picker = [[UIImagePickerController alloc]init];
    UIImagePickerControllerSourceType sourcheType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    picker.sourceType = sourcheType;
    picker.delegate = self;
    picturePopoverController = [[UIPopoverController alloc]initWithContentViewController:picker];
    [picturePopoverController presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];

    
//    if (iOS(8.0)) {
//        [[NSOperationQueue mainQueue]addOperationWithBlock:^{
//            [self presentViewController:picker animated:YES completion:nil];
//        }];
//    }
//    else{
//        [self presentViewController:picker animated:YES completion:nil];
//    }
    
//    UIActionSheet *actionSheet = [[UIActionSheet alloc]
//                                  initWithTitle:nil
//                                  delegate:self
//                                  cancelButtonTitle:nil
//                                  destructiveButtonTitle:nil
//                                  otherButtonTitles:@"拍照", @"从手机相册选取",nil];
//    [actionSheet showInView:self.view];
}


-(void)camera{
    
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
-(void)photoAlbum{
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

#pragma mark - HYDrawingViewController Methods

- (NSArray *)constrainSubview:(UIView *)subview toMatchWithSuperview:(UIView *)superview {
    subview.translatesAutoresizingMaskIntoConstraints = NO;
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(subview);
    NSDictionary *metrics = @{@"vHeight":@(119),@"hPadding":@(11)};
    
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

- (void) showColorPicker:(id)sender
{
    if ([self shouldDismissPopoverForClassController:[WDColorPickerController class] insideNavController:NO]) {
        [self hidePopovers];
        return;
    }
    
    [self showController:self.colorPickerController fromBarButtonItem:sender animated:NO];
}



- (void) paintColorChanged:(NSNotification *)aNotification
{
    WDColor *newPaintColor = [aNotification userInfo][@"pickedColor"];
    [self.colorPickerController setColor:newPaintColor];
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - 底部工具栏按钮

#pragma mark - BottomBarViewDelegate Methods
- (void)bottomBarView:(BottomBarView*)bottomBarView forButtonAction:(UIButton*)button {
    switch (button.tag) {
        case COLORWHEEL_BTN:        ///< 调色板
            [self showColorPicker:button];
            break;
        case ERASER_BTN:            ///< 橡皮擦
            [[HYBrushCore sharedInstance]activeEraser];
            break;
        case PENCIL_BTN:            ///< 铅笔
            [[HYBrushCore sharedInstance]activePencil];
            break;
        case MARKERPEN_BTN:         ///< 马克笔
            [[HYBrushCore sharedInstance]activeMarker];
            break;
        case COLORBRUSH_BTN:        ///< 水彩笔
            [[HYBrushCore sharedInstance]activeWaterColorPen];
            break;
        case CRAYON_BTN:
            [[HYBrushCore sharedInstance]activeCrayon];
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
    
    
}

#pragma mark - ImageEditViewControllerDelegate

- (void)updateLayersView
{
    _layersViewController.layersCount = [[HYBrushCore sharedInstance]getLayersNumber];
    [_layersViewController.tbView reloadData];
    
    bottomBarView.hidden = NO;
    self.navigationController.navigationBar.hidden = NO;
}

#pragma mark 形状选择弹出
-(void)didSelectedShape:(UIImage*)img{
    [_shapeBoxPopoverController dismissPopoverAnimated:YES];
    [self showImageEditViewWithImage:img];
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

#pragma mark 画布背景选择弹出

// 代理方法

-(void)showCanvasBackgroundPopoverController:(UIButton*)sender{
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


#pragma mark 显示裁剪视图
-(void)showCliperView{
    CliperView *cliper = [[CliperView alloc]initWithFrame:self.view.frame];
    [self.view addSubview:cliper];
}



#pragma mark 图层弹出视图
-(void)showLayerPopoverController:(UIButton*)sender{
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
    
#pragma mark 观察当前图层数量
    [_layersViewController addObserver:self forKeyPath:@"layersCount" options:NSKeyValueObservingOptionNew|NSKeyValueObservingOptionOld context:nil];
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

#pragma mark - 选择相册图片
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info{
    [picturePopoverController dismissPopoverAnimated:YES];

    [self showImageEditViewWithImage:info[@"UIImagePickerControllerOriginalImage"]];
}

#pragma mark 图像变换
-(void)showImageEditViewWithImage:(UIImage*)img{
    imageEditViewController = [ImageEditViewController new];
    imageEditViewController.delegate = self;
    imageEditViewController.originalImg = img;
    imageEditViewController.view.frame = self.view.frame;
    imageEditViewController.view.backgroundColor = [UIColor clearColor];
    // 隐藏导航栏
    self.navigationController.navigationBar.hidden = YES;
    // 隐藏底部工具栏
    bottomBarView.hidden = YES;

    [self.view addSubview:imageEditViewController.view];
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
