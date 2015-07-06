//
//  MainViewController.m
//  AppleCoder-OpenGLES-00
//
//  Created by CharlyZhang on 14-11-6.
//
//

#import "MainViewController.h"
#include "BrushesCore.h"
#include "EAGLView.h"

#define BOTTOM_OFFSET   48

@interface MainViewController ()
//<UITextFieldDelegate>
{
    CZCanvas *canvas;
    CZPainting *painting;
    
    float red,green,blue;
}

@end

@implementation MainViewController
//@synthesize glView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
        red = green = blue = 0.0;
    }
    return self;
}

- (void)dealloc
{
    delete canvas;
    delete painting;
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];


    NSLog(@"sandbox path is:%@",NSHomeDirectory());
    
//    EAGLView *glView = [[EAGLView alloc]initWithFrame:self.view.bounds];
//    [self.view addSubview:glView];
//    [glView release];
//    
    
    CGSize size = [UIScreen mainScreen].bounds.size;
    canvas = new CZCanvas(CZRect(0,0,size.width,size.height-BOTTOM_OFFSET));
    painting = new CZPainting(CZSize(size.width,size.height-BOTTOM_OFFSET));
    canvas->setPaiting(painting);
    [self.view insertSubview:(UIView*)canvas->getView() atIndex:0];
    
    [self setBrushSize:10.0];
    [self updatePaintColor];
}

- (BOOL)shouldAutorotate {
    return NO;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Actions

- (IBAction)sizeSlider:(UISlider *)sender {
    NSLog(@"size of slider : %f",sender.value);
    [self setBrushSize:sender.value];
}
- (IBAction)redSlider:(UISlider *)sender {
    red = sender.value;
    [self updatePaintColor];
}
- (IBAction)greenSlider:(UISlider *)sender {
    green = sender.value;
    [self updatePaintColor];
}
- (IBAction)blueSlider:(UISlider *)sender {
    blue = sender.value;
    [self updatePaintColor];
}
- (IBAction)modeSegmentedCtrl:(UISegmentedControl *)sender {
    NSLog(@"mode segmented controller is:%d",sender.selectedSegmentIndex);
    if (sender.selectedSegmentIndex == 0) {
        CZActiveState::getInstance()->setEraseMode(false);
    }
    else {
        CZActiveState::getInstance()->setEraseMode(true);
    }
}

#pragma mark - BrushCore Related Methods

- (void)setBrushSize:(float) value {
    CZBrush *ptrActiveBrush = CZActiveState::getInstance()->getActiveBrush();
    ptrActiveBrush->weight.value = value;
}

- (void)updatePaintColor {
    NSLog(@"color : (%f,%f,%f)",red,green,blue);
    CZActiveState::getInstance()->setPaintColor(red, green, blue);
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
