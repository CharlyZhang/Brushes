//
//  MainViewController.m
//  AppleCoder-OpenGLES-00
//
//  Created by CharlyZhang on 14-11-6.
//
//

#import "MainViewController.h"
#include "CZCanvas.h"
#include "painting/CZPainting.h"
#include "basic/CZRect.h"
@interface MainViewController ()
{
    CZCanvas *canvas;
    CZPainting *painting;
}

@end

@implementation MainViewController
//@synthesize glView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
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
    // Do any additional setup after loading the view.
    //glView = [[EAGLView alloc]initWithFrame:self.view.bounds];
    self.view.backgroundColor = [UIColor redColor];
    //[self.view addSubview:glView];
    CGSize size = self.view.bounds.size;
    
    canvas = new CZCanvas(CZRect(0,0,size.width,size.height));
    painting = new CZPainting(CZSize(size.width,size.height));
    canvas->setPaiting(painting);
    
    [self.view addSubview:(UIView*)canvas->getView()];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
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
