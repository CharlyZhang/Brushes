//
//  HYDrawingViewController.m
//  HYDrawing
//
//  Created by 李 雷川 on 15/7/16.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "HYDrawingViewController.h"
#import "HYMenuViewController.h"
#import "HYPopoverBackgroundView.h"
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
    UIBarButtonItem *imageItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"picture"] style:UIBarButtonItemStylePlain target:self action:@selector(tapMenu:)];
    UIBarButtonItem *shareItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"share"] style:UIBarButtonItemStylePlain target:self action:@selector(tapMenu:)];
    self.navigationItem.rightBarButtonItems = @[shareItem,imageItem,settingItem,videoItem];
    
}

-(void)viewWillAppear:(BOOL)animated{
    //self.navigationController.navigationBar.backgroundColor = [UIColor orangeColor];
    
    [self.navigationController.navigationBar setBackgroundImage:[UIImage imageNamed:@"transparent"] forBarMetrics:UIBarMetricsCompact];
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


    HYPopoverBackgroundView *backgroundView = [[HYPopoverBackgroundView  alloc]init];

    popoverController.popoverBackgroundViewClass =[backgroundView class];
    backgroundView.backgroundImageView.image = [UIImage imageNamed:@"menu_bg"];
    
    
    [popoverController setPopoverContentSize:CGSizeMake(216, 605)];
    [popoverController presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
//    popoverController.backgroundColor = UIPopoverBorderColor;

   
 
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
