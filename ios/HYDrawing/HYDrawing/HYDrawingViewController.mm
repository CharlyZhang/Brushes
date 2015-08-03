//
//  HYDrawingViewController.m
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/16.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "HYDrawingViewController.h"
#import "BottomBarView.h"
#import "ColorWheelView.h"
#import "HYMenuViewController.h"
#import "DDPopoverBackgroundView.h"
#import "ImageEditViewController.h"
#import "Macro.h"
#include "BrushesCore.h"

@interface HYDrawingViewController ()<BottomBarViewDelegate,ColorWheelViewDelegate> {
    CZCanvas *canvas;
    CZPainting *painting;
}

@end

@implementation HYDrawingViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    UIBarButtonItem *menuItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"menu"] style:UIBarButtonItemStylePlain target:self action:@selector(tapMenu:)];
    
    self.navigationItem.leftBarButtonItems = @[menuItem];
    
    UIBarButtonItem *videoItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"video"] style:UIBarButtonItemStylePlain target:self action:@selector(tapMenu:)];
    UIBarButtonItem *settingItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"setting"] style:UIBarButtonItemStylePlain target:self action:@selector(tapMenu:)];
    UIBarButtonItem *pictureItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"picture"] style:UIBarButtonItemStylePlain target:self action:@selector(tapPicture:)];
    UIBarButtonItem *shareItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"share"] style:UIBarButtonItemStylePlain target:self action:@selector(tapMenu:)];
    self.navigationItem.rightBarButtonItems = @[shareItem,pictureItem,settingItem,videoItem];
    
    
    // bottom bar view
    BottomBarView *bottomBarView = [[BottomBarView alloc]init];
    bottomBarView.delegate = self;
    [self.view addSubview:bottomBarView];
    [self constrainSubview:bottomBarView toMatchWithSuperview:self.view];
    
    // load brush core
    CGSize size = [UIScreen mainScreen].bounds.size;
    canvas = new CZCanvas(CZRect(0,0,size.width,size.height));
    painting = new CZPainting(CZSize(size.width,size.height));
    canvas->setPaiting(painting);
    [self.view insertSubview:(__bridge UIView*)canvas->getView() atIndex:0];
    
    CZActiveState::getInstance()->setEraseMode(false);
    CZActiveState::getInstance()->setActiveBrush(kPencil);
}

//-(BOOL)shouldAutorotate{
//    return YES;
//}
//
//- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation{
//    return  UIInterfaceOrientationLandscapeLeft | UIInterfaceOrientationLandscapeRight;
//}

-(void)viewWillAppear:(BOOL)animated{
    
    [self.navigationController.navigationBar setBackgroundImage:[UIImage imageNamed:@"transparent"] forBarMetrics:UIBarMetricsDefault];
     self.navigationController.navigationBar.barStyle = UIBarStyleBlackTranslucent;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



- (void)tapMenu:(id)sender{
    HYMenuViewController *menuViewController = [[HYMenuViewController alloc]init];
    UINavigationController *menuNavigationController = [[UINavigationController alloc]initWithRootViewController:menuViewController];
    menuNavigationController.navigationBar.barTintColor = UIPopoverBackgroundColor;
    menuNavigationController.navigationBar.barStyle = UIBarStyleBlackTranslucent;
    
    UIPopoverController *popoverController = [[UIPopoverController alloc]initWithContentViewController:menuNavigationController];


    popoverController.popoverBackgroundViewClass =[DDPopoverBackgroundView class];
    UIImage *image = [UIImage imageNamed:@"menu_popover_bg"];
    [DDPopoverBackgroundView setBackgroundImage:image];
    [DDPopoverBackgroundView setBackgroundImageCornerRadius:2.f];
    [DDPopoverBackgroundView setArrowBase:0];
    [DDPopoverBackgroundView setArrowHeight:0];
    [popoverController setPopoverContentSize:CGSizeMake(image.size.width, image.size.height)];
    [popoverController presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];

   
 
}


- (void)tapPicture:(id)sender{
    
    UIImagePickerController  *picker = [[UIImagePickerController alloc]init];
    UIImagePickerControllerSourceType sourcheType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    picker.sourceType = sourcheType;
    picker.delegate = self;
    UIPopoverController *popoverController = [[UIPopoverController alloc]initWithContentViewController:picker];
    popoverController.delegate = self;
    [popoverController presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
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
//    popoverController.backgroundColor = UIPopoverBorderColor;
}
-(void)insertImage:(UIImage *)image{
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
    
    CZAffineTransform trans = CZAffineTransform::makeFromTranslation(100, 100);
    
    painting->getActiveLayer()->renderImage(brushImg, trans);
    canvas->drawView();

}
- (void)dealloc{
    if (canvas) {
        delete canvas;
        canvas = NULL;
    }
    
    if (painting) {
        delete painting;
        painting = NULL;
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

#pragma mark - BottomBarViewDelegate Methods

- (void)bottomBarView:(BottomBarView*)bottomBarView forButtonAction:(UIButton*)button {
    UIPopoverController *popoverController;
    UIViewController  *vc;
    CGRect rect;
    
    switch (button.tag) {
        case COLORWHEEL_BTN:        ///< 调色板
            vc = [[UIViewController alloc]init];
           // vc.view = [[ColorWheelView alloc]init];
            vc.view = [[NSBundle mainBundle] loadNibNamed:@"ColorWheelView" owner:self options:Nil][0];
            [(ColorWheelView*)vc.view setDelegate:self];
            popoverController = [[UIPopoverController alloc]initWithContentViewController:vc];
            popoverController.delegate = self;
            rect = [button convertRect:button.frame toView:self.view];
            [popoverController presentPopoverFromRect:rect inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
            //[popoverController presentPopoverFromBarButtonItem:button permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];

            
            break;
        case ERASER_BTN:
            CZActiveState::getInstance()->setEraseMode(true);
            CZActiveState::getInstance()->setActiveBrush(kEraser);
            break;
        case PENCIL_BTN:
            CZActiveState::getInstance()->setEraseMode(false);
            CZActiveState::getInstance()->setActiveBrush(kPencil);
            break;
        case MARKERPEN_BTN:
            CZActiveState::getInstance()->setEraseMode(false);
            CZActiveState::getInstance()->setActiveBrush(kCrayon);
            break;
        default:
            break;
    }
}

- (void)colorWheelView:(ColorWheelView *)colorWheelView setColor:(UIColor *)color {
    
    CGFloat red,green,blue,alpha;
    [color getRed:&red green:&green blue:&blue alpha:&alpha];
    CZColor c(red,green,blue,alpha);
    CZActiveState::getInstance()->setPaintColor(c);
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

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
#pragma mark UIImagePickerControllerDelegate
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info{
    UIImage *image =info[@"UIImagePickerControllerOriginalImage"];
    [picker dismissViewControllerAnimated:YES completion:^{
        //[self insertImage:image];
        ImageEditViewController *imageEditViewController = [[ImageEditViewController alloc]init];
        [self.navigationController pushViewController:imageEditViewController animated:NO];
        
    }];
}
@end
