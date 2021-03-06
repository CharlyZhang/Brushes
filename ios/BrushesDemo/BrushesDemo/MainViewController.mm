//
//  MainViewController.m
//  AppleCoder-OpenGLES-00
//
//  Created by CharlyZhang on 14-11-6.
//
//

#import "MainViewController.h"
#include "BrushesCore.h"
#import "CZViewImpl.h"
#include "PaintingManager.h"
#include "stamp/CZBristleGenerator.h"
#include "EAGLView.h"
#import "LayersTableViewController.h"
#import <CoreGraphics/CoreGraphics.h>

#define BOTTOM_OFFSET   72

@interface MainViewController ()<UIPickerViewDelegate, UIPickerViewDataSource>
//<UITextFieldDelegate>
{
    CZViewImpl *viewImpl;
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
@property (retain, nonatomic) IBOutlet UIButton *undoButton;
@property (retain, nonatomic) IBOutlet UIButton *redoButton;


@property (retain, nonatomic) IBOutlet UIButton *itentity;
@property (retain, nonatomic) IBOutlet UIButton *angle;
@property (retain, nonatomic) IBOutlet UIButton *spacing;
@property (retain, nonatomic) IBOutlet UIButton *jitter;
@property (retain, nonatomic) IBOutlet UIButton *scatter;
@property (retain, nonatomic) IBOutlet UIButton *bDensity;
@property (retain, nonatomic) IBOutlet UITextField *weight;
@property (retain, nonatomic) IBOutlet UIButton *bSize;
@property (retain, nonatomic) IBOutlet UIButton *Dangle;
@property (retain, nonatomic) IBOutlet UIButton *Dweight;
@property (retain, nonatomic) IBOutlet UIButton *Dintentity;

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
    [_undoButton release];
    [_redoButton release];
    [_itentity release];
    [_angle release];
    [_spacing release];
    [_jitter release];
    [_scatter release];
    [_bDensity release];
    [_weight release];
    [_bSize release];
    [_Dangle release];
    [_Dweight release];
    [_Dintentity release];
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
    
    CZActiveState::getInstance()->mainScreenScale = 2.f;

    CGSize size = [UIScreen mainScreen].bounds.size;
    viewImpl = new CZViewImpl(CZRect(0,0,size.width,size.height-BOTTOM_OFFSET));
    canvas = new CZCanvas(viewImpl);
    [[PaintingManager sharedInstance] initializeWithWidth:size.width height:size.height-BOTTOM_OFFSET scale:2.f];
    painting = (CZPainting*)[[PaintingManager sharedInstance] getInitialPainting];
    canvas->setPaiting(painting);
    
    [self.view insertSubview:(UIView*)viewImpl->realView atIndex:0];
    
    [self setBrushSize:10.0];
    [self updatePaintColor];
    brushesName = [[NSArray alloc ]initWithObjects:@"brush1", @"brush2",@"brush3",nil];
    self.colorFillSwitcher = nil;
    self.layersNumber = painting->getLayersNumber();
    
    [self selectBrush:2];
    CZActiveState::getInstance()->setPaintColor(0, 0, 1);
}

- (BOOL)shouldAutorotate {
    return YES;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Actions
- (IBAction)clearButton:(UIButton *)sender {
    CZLayer *layer = painting->getActiveLayer();
    layer->clear();
    canvas->drawView();
}

- (IBAction)adjustProperty:(UISlider *)sender {
    float v = sender.value;
    CZBrush *brush = CZActiveState::getInstance()->getActiveBrush();
    CZBristleGenerator *gen = (CZBristleGenerator *)brush->getGenerator();
    
    switch (sender.tag) {
        case 0: ///< intentity
            brush->intensity.value = v;
            self.itentity.titleLabel.text = [NSString stringWithFormat:@"itentity:%0.2f",sender.value];
            break;
        case 1: ///< angle
            brush->angle.value = v;
            self.angle.titleLabel.text = [NSString stringWithFormat:@"angle:%0.2f",sender.value];
            break;
        case 2: ///< spacing
            brush->spacing.value = v;
            self.spacing.titleLabel.text = [NSString stringWithFormat:@"spacing:%0.2f",sender.value];
            break;
        case 3: ///< dynamic itentity
            brush->intensityDynamics.value = v;
            self.Dintentity.titleLabel.text = [NSString stringWithFormat:@"Dintentity:%0.2f",sender.value];
            break;
        case 4: ///< jitter
            brush->rotationalScatter.value = v;
            self.jitter.titleLabel.text = [NSString stringWithFormat:@"jitter:%0.2f",sender.value];
            break;
        case 5: ///< scatter
            brush->positionalScatter.value = v;
            self.scatter.titleLabel.text = [NSString stringWithFormat:@"scatter:%0.2f",sender.value];
            break;
        case 6: ///< dynamic weight
            brush->weightDynamics.value = v;
            self.Dweight.titleLabel.text = [NSString stringWithFormat:@"Dweight:%0.2f",sender.value];
            break;
        case 7: ///< bristle dentity
            gen->bristleDensity.value = v;
            gen->propertiesChanged();
            painting->clearLastBrush();
            self.bDensity.titleLabel.text = [NSString stringWithFormat:@"bDensity:%0.2f",sender.value];
            break;
        case 8: ///< bristle size
            gen->bristleSize.value = v;
            gen->propertiesChanged();
            painting->clearLastBrush();
            self.bSize.titleLabel.text = [NSString stringWithFormat:@"bSize:%0.2f",sender.value];
            break;
        case 9: ///< dynamic angle
            brush->angleDynamics.value = v;
            self.Dangle.titleLabel.text = [NSString stringWithFormat:@"Dangle:%0.2f",sender.value];
            break;
        default:
            break;
    }
}


- (IBAction)sizeSlider:(UISlider *)sender {
    NSLog(@"size of slider : %f",sender.value);
    [self setBrushSize:sender.value];
    self.weight.text = [NSString stringWithFormat:@"大小:%0.2f",sender.value];
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
    self.undoButton.enabled = true;
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

- (IBAction)undoButtonAction:(UIButton *)sender {
    CZLayer *layer = painting->getActiveLayer();
    layer->undoAction();
    self.undoButton.enabled = false;
    self.redoButton.enabled = true;
    canvas->drawView();
}

- (IBAction)redoButtonAction:(UIButton *)sender {
    CZLayer *layer = painting->getActiveLayer();
    layer->redoAction();
    self.undoButton.enabled = true;
    self.redoButton.enabled = false;
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
    NSLog(@"selected brush index: %u",brushIdx);
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
