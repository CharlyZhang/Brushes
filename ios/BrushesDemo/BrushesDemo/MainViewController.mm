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
#import "LayersTableViewController.h"
#import <CoreGraphics/CoreGraphics.h>

#define BOTTOM_OFFSET   72

@interface MainViewController ()<UIPickerViewDelegate, UIPickerViewDataSource>
//<UITextFieldDelegate>
{
    CZCanvas *canvas;
    CZPainting *painting;
    
    float red,green,blue;
    NSArray *brushesName;
    
    UIPopoverController *popoverController_;
    LayersTableViewController *layerController_;
    NSInteger currentLayerIdx;
}
@property (retain, nonatomic) IBOutlet UIPickerView *brushesSelectView;
@property (assign, nonatomic) UISwitch *colorFillSwitcher;
@property (assign, nonatomic) NSInteger layersNumber;

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
    [_brushesSelectView release];
    [brushesName release];
    [layerController_ release];
    [popoverController_ release];
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor lightGrayColor];


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
    brushesName = [[NSArray alloc ]initWithObjects:@"brush1", @"brush2",nil];
    self.colorFillSwitcher = nil;
    self.layersNumber = painting->getLayersNumber();
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

- (IBAction)fillColorSwitcher:(UISwitch *)sender {
    self.colorFillSwitcher = sender;
    CZActiveState::getInstance()->colorFillMode = true;
}

- (IBAction)insertImageButton:(UIButton *)sender {
    UIImage *image = [UIImage imageNamed:@"profile.jpg"];
   
    CGImageRef img = image.CGImage;
    
    //数据源提供者
    CGDataProviderRef inProvider = CGImageGetDataProvider(img);
    // provider’s data.
    CFDataRef inBitmapData = CGDataProviderCopyData(inProvider);
    
    //宽，高，data
    size_t width = CGImageGetWidth(img);
    size_t height = CGImageGetHeight(img);
    size_t componentNum = CGImageGetBitsPerPixel(img) / CGImageGetBitsPerComponent(img);
    StorageMode mode;
    if (componentNum == 3) {
        mode = RGB_BYTE;
    }
    else {
        mode = RGBA_BYTE;
    }
    
    CZImage *brushImg = new CZImage(width,height,mode,(void*)CFDataGetBytePtr(inBitmapData));
    CZAffineTransform trans = CZAffineTransform::makeFromTranslation(100, 100);
    
    painting->getActiveLayer()->renderImage(brushImg, trans);
    canvas->drawView();
}


- (IBAction)layerButton:(UIButton *)sender {
        
    if (!layerController_) {
        layerController_ = [[LayersTableViewController alloc]init];
//        layerController_ = [[LayersTableViewController alloc] initWithNibName:@"Layers" bundle:nil];
        layerController_.painting = painting;
//        layerController_.delegate = self;
        
        popoverController_ = [[UIPopoverController alloc] initWithContentViewController:layerController_];
    }
    
    [popoverController_ presentPopoverFromRect:CGRectInset(((UIView *) sender).bounds, 10, 10)
                                        inView:sender
                      permittedArrowDirections:(UIPopoverArrowDirectionUp | UIPopoverArrowDirectionDown)
                                      animated:YES];
}

- (IBAction)controlLayerStepper:(UIStepper *)sender {
    NSLog(@"layer number is %f",sender.value);
    
    if (sender.value > self.layersNumber) {
        painting->addNewLayer();
    }
    else if (sender.value < self.layersNumber) {
        painting->deleteActiveLayer();
    }
    
    self.layersNumber = sender.value;
    canvas->drawView();
}

#pragma mark - BrushCore Related Methods

- (void)setBrushSize:(float) value {
    NSLog(@"current size: %f",value);
    CZBrush *ptrActiveBrush = CZActiveState::getInstance()->getActiveBrush();
    ptrActiveBrush->weight.value = value;
}

- (void)updatePaintColor {
    NSLog(@"current color: (%f,%f,%f)",red,green,blue);
    CZActiveState::getInstance()->setPaintColor(red, green, blue);
}

- (void)selectBrush:(NSUInteger) brushIdx {
    NSLog(@"selected brush index: %d",brushIdx);
    CZActiveState::getInstance()->setActiveBrush(brushIdx);
}

#pragma mark - UIPickerViewDelegate Methods
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView{
    return 1;
}

// returns the # of rows in each component..
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
    return CZActiveState::getInstance()->getBrushesNumber();
}

-(NSString*) pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component{
    return [brushesName objectAtIndex:row];
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    [self selectBrush:row];
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
