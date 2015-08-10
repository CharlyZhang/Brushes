//
//  ZXHLayersViewController.m
//  HYDrawing
//
//  Created by macbook on 15/8/10.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ZXHLayersViewController.h"
#import "Macro.h"
#import "LayersCell.h"

@interface ZXHLayersViewController ()<UITableViewDataSource,UITableViewDelegate,UIGestureRecognizerDelegate>

@property(nonatomic,copy)NSMutableArray *arrLayer;

@end

@implementation ZXHLayersViewController
{
    UITableView *_tbView;
}
- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 数据源数组
    _arrLayer = [NSMutableArray new];
    
    // 初始化UI
    [self createUI];
}

#pragma mark UI
-(void)createUI{
    UIImage *bgImg = [UIImage imageNamed:@"layers_popover_bg"];
    CGFloat h = bgImg.size.height-20;
    
    // 1.topToolBar
    UIView *topToolBar = [[UIView alloc]initWithFrame:CGRectMake(0, 0, kSelfViewWidth-4,44)];
    topToolBar.backgroundColor = UIPopoverBackgroundColor;
    [self.view addSubview:topToolBar];
    
    
    // 2.tableView
    _tbView = [[UITableView alloc]initWithFrame:CGRectMake(0, 44, kSelfViewWidth, h-88)];
    [self.view addSubview:_tbView];
    _tbView.dataSource = self;
    _tbView.delegate = self;
    _tbView.rowHeight = 124;
    [_tbView registerNib:[UINib nibWithNibName:@"LayersCell" bundle:nil] forCellReuseIdentifier:@"LayersCellId"];
    _tbView.separatorStyle = UITableViewCellSeparatorStyleNone;
    // 可编辑状态
    [_tbView setEditing:YES animated:YES];
    // 长按手势
    UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(moveCellTo:)];
    longPress.minimumPressDuration = 1;
    longPress.delegate = self;
    [_tbView addGestureRecognizer:longPress];
    
    
    // 3.bottomToolBar
    UIView *bottomToolBar = [[UIView alloc]initWithFrame:CGRectMake(0, h-54, kSelfViewWidth-2,44)];
//    bottomToolBar.backgroundColor = [UIColor redColor];
    [self.view addSubview:bottomToolBar];

}

#pragma mark 表格视图回调
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return 10;
}

// 复用
-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    LayersCell *cell = [tableView dequeueReusableCellWithIdentifier:@"LayersCellId"];
    cell.showsReorderControl  = YES;
    return cell;
}

// 选中cell
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    LayersCell *cell = (LayersCell *)[tableView cellForRowAtIndexPath:indexPath];
    [cell setOutlineViewBorderWithColor:UIPopoverBackgroundColor];
    cell.selectedBackgroundView = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"layer_cell_selected_bg"]];
}

// 取消选中时
-(void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath{
    LayersCell *cell = (LayersCell *)[tableView cellForRowAtIndexPath:indexPath];
    [cell setOutlineViewBorderWithColor:[UIColor cyanColor]];
}


//设置进入编辑状态时，Cell不会缩进
- (BOOL)tableView: (UITableView *)tableView shouldIndentWhileEditingRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

// 编辑样式
-(UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath{
    return UITableViewCellEditingStyleNone;
}

// 可以拖动cell
-(BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}

-(void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)sourceIndexPath toIndexPath:(NSIndexPath *)destinationIndexPath{
    NSLog(@"move");
    NSInteger fromRow = sourceIndexPath.row;
    NSInteger toRow = destinationIndexPath.row;
    // 改变数据源内容
    
}

#pragma mark 长按手势
-(void)moveCellTo:(UILongPressGestureRecognizer*)gesture{
    
    CGPoint p = [gesture locationInView:_tbView];
    NSIndexPath *indexPath = [_tbView indexPathForRowAtPoint:p];//获取响应的长按的indexpath
    if (indexPath == nil)
        return;
    else
        NSLog(@"row %ld", indexPath.row);
}


@end
