//
//  HYDrawingViewController.m
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/16.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "HYDrawingViewController.h"
#import "BottomBarView.h"
#import "HYMenuViewController.h"
#import "DDPopoverBackgroundView.h"
#import "Macro.h"
@interface HYDrawingViewController ()

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
    
    BottomBarView *bottomBarView = [[BottomBarView alloc]init];
    [self.view addSubview:bottomBarView];
    [self constrainSubview:bottomBarView toMatchWithSuperview:self.view];
    
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
        
    }];
}
@end
