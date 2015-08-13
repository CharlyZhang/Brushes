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
#import "HYBrushCore.h"

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
    CGFloat w = bgImg.size.width;
    CGFloat h = bgImg.size.height-20;
    
    // 1.topToolBar
    UIView *topToolBar = [[UIView alloc]initWithFrame:CGRectMake(0, 0, w,44)];
    topToolBar.backgroundColor = UIPopoverBackgroundColor;
    [self.view addSubview:topToolBar];
    
    
    // 2.tableView
    _tbView = [[UITableView alloc]initWithFrame:CGRectMake(0, 44, w, h-104)];
    [self.view addSubview:_tbView];
    _tbView.dataSource = self;
    _tbView.delegate = self;
    _tbView.rowHeight = 124;
    [_tbView registerNib:[UINib nibWithNibName:@"LayersCell" bundle:nil] forCellReuseIdentifier:@"LayersCellId"];
    _tbView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    // 可编辑状态
    _tbView.editing = YES;
    _tbView.allowsSelectionDuringEditing = YES;
    // 默认选中第一行
    NSIndexPath *index = [NSIndexPath indexPathForRow:0 inSection:0];
    [_tbView selectRowAtIndexPath:index animated:YES scrollPosition:UITableViewScrollPositionTop];
    LayersCell *cell = (LayersCell *)[_tbView cellForRowAtIndexPath:index];
    [cell setOutlineViewBorderWithColor:UIPopoverBackgroundColor];
    cell.selectedBackgroundView = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"layer_cell_selected_bg"]];
    
    // 3.bottomToolBar
    UIView *bottomToolBar = [[UIView alloc]initWithFrame:CGRectMake(0, h-62, w,44)];
    bottomToolBar.backgroundColor = UIPopoverBackgroundColor;
    [self.view addSubview:bottomToolBar];

}

#pragma mark 表格视图回调
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return 10;
}

// 复用
-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    LayersCell *cell = [tableView dequeueReusableCellWithIdentifier:@"LayersCellId"];
    if (!cell.selected) {
        [cell setOutlineViewBorderWithColor:[UIColor cyanColor]];
    }else{
        [cell setOutlineViewBorderWithColor:UIPopoverBackgroundColor];
    }
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

// 编辑样式
-(UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath{
    return UITableViewCellEditingStyleNone;
}

-(BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath{
    return YES;
}

-(BOOL)tableView:(UITableView *)tableView shouldIndentWhileEditingRowAtIndexPath:(NSIndexPath *)indexPath{
    return NO;
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
@end
