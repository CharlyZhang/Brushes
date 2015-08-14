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
#import "ZXHLayerTopBar.h"
#import "HYBrushCore.h"

@interface ZXHLayersViewController ()<UITableViewDataSource,UITableViewDelegate,UIGestureRecognizerDelegate>

@property(nonatomic,copy)NSMutableArray *arrLayer;

@end

@implementation ZXHLayersViewController
{
    UITableView *_tbView;
    UILabel *_alphaLabel;
    ZXHLayerTopBar *_topToolBar;
    NSInteger _curLayerIndex;
    NSInteger _layersCount;
}

static ZXHLayersViewController *layersController;

#pragma mark 单例
+(id)defaultLayersController{
    if (!layersController) {
        layersController = [ZXHLayersViewController new];
    }
    
    return layersController;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 层数
    _layersCount = [[HYBrushCore sharedInstance]getLayersNumber];
    
    // 初始化UI
    [self createUI];
    
    // 监听图层状态
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(layerLockNotify:) name:@"LayerLockNotification" object:nil];
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(layerVisibleNotify:) name:@"LayerVisibleNotification" object:nil];
    
    // 设置当前选中图层
    _curLayerIndex = [[HYBrushCore sharedInstance] getActiveLayerIndex];
    [self selectRowAtIndexPath:_curLayerIndex];

}

#pragma mark UI
-(void)createUI{
    UIImage *bgImg = [UIImage imageNamed:@"layers_popover_bg"];
    CGFloat w = bgImg.size.width;
    CGFloat h = bgImg.size.height-20;
    
    // 1.topToolBar
    [self buildTopToolBar];
    
    // 2.tableView
    [self buildTableViewWithWidth:w height:h];
    
    // 3.bottomToolBar
    [self buildBottomToolBarWithWidth:w height:h];

}

#pragma mark 图层操作
// 消息 - 锁定
-(void)layerLockNotify:(NSNotification*)noti{
    NSArray *info = noti.object;
    BOOL lock = [info[0] boolValue];
    NSInteger cur = [info[1] integerValue];
    
    if (!lock && cur==_curLayerIndex) {
        // 锁了
        _topToolBar.btnDelete.enabled = NO;
    }else{
        _topToolBar.btnDelete.enabled = YES;
    }
    
    // 锁定图层
    [[HYBrushCore sharedInstance]setLocked:lock ofLayer:cur];
}

// 消息 - 隐藏
-(void)layerVisibleNotify:(NSNotification*)noti{
    NSArray *info = noti.object;
    BOOL visible = [info[0] boolValue];
    NSInteger cur = [info[1] integerValue];
    
    if (!visible && cur==_curLayerIndex) {
        // 不可见
    }else{
        
    }
    
    [[HYBrushCore sharedInstance]setVisibility:visible ofLayer:cur];
}

// 删除
-(void)deleteLayer:(UIButton*)btn{
    if (_layersCount == 1) {
        return;
    }
    
    // 选中下一行
    if (_curLayerIndex == _layersCount-1) {
        _curLayerIndex --;
        if (_curLayerIndex < 0) {
            return;
        }
    }
    
    // 删除选中行
    [[HYBrushCore sharedInstance] deleteActiveLayer];
    _layersCount = [[HYBrushCore sharedInstance]getLayersNumber];
    
    NSIndexPath *curIndexPath = [NSIndexPath indexPathForRow:_curLayerIndex inSection:0];
    [_tbView deleteRowsAtIndexPaths:@[curIndexPath] withRowAnimation:UITableViewRowAnimationTop];
    
    [_tbView reloadData];
    
    [self selectRowAtIndexPath:_curLayerIndex];
//    NSLog(@"行： %ld",_curLayerIndex);
}

// 拷贝
-(void)copyLayer:(UIButton*)btn{
    
    
    [_tbView reloadData];
}

// 添加
-(void)addLayer:(UIButton*)btn{
    if (_layersCount == 10) {
        _topToolBar.btnCopy.enabled = NO;
        _topToolBar.btnAdd.enabled = NO;
        return;
    }
    
    // 添加图层
    [[HYBrushCore sharedInstance] addNewLayer];
    
    _layersCount = [[HYBrushCore sharedInstance]getLayersNumber];
    
    // 插入行
    NSIndexPath *curIndexPath = [NSIndexPath indexPathForRow:_curLayerIndex inSection:0];
    [_tbView insertRowsAtIndexPaths:@[curIndexPath] withRowAnimation:UITableViewRowAnimationNone];
    
    NSMutableArray *arr = [NSMutableArray new];
    for (NSInteger i=0; i<_layersCount; i++) {
        NSIndexPath *index = [NSIndexPath indexPathForRow:i inSection:0];
        [arr addObject:index];
    }
    
    [_tbView reloadRowsAtIndexPaths:arr withRowAnimation:UITableViewRowAnimationAutomatic];
    
    // 选中
    [self selectRowAtIndexPath:_curLayerIndex];
    
}


// ++++ 选中某行并设置样式
-(void)selectRowAtIndexPath:(NSInteger)index{
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:index inSection:0];
    [_tbView selectRowAtIndexPath:indexPath animated:YES scrollPosition:UITableViewScrollPositionMiddle];
//        NSLog(@"行-： %ld",index);
    
    // 设置样式
    LayersCell *cell = (LayersCell *)[_tbView cellForRowAtIndexPath:indexPath];
    [cell setOutlineViewBorderWithColor:UIPopoverBackgroundColor];
    cell.selectedBackgroundView = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"layer_cell_selected_bg"]];
    
    // 是否可编辑
    if (!cell.isUnlocked) {
        _topToolBar.btnDelete.enabled = NO;
    }else{
        _topToolBar.btnDelete.enabled = YES;
    }
    
    // 设置选中层
    [[HYBrushCore sharedInstance] setActiveLayer:cell.rowIndex];
    NSLog(@"getActiveLayer: %ld",[[HYBrushCore sharedInstance] getActiveLayerIndex]);
    
    // 层数
    _layersCount = [[HYBrushCore sharedInstance] getLayersNumber];
    
    cell.outlineView.backgroundColor = kImageColor(@"layer_showimg_bg");
}

#pragma mark - 创建顶部工具栏
-(void)buildTopToolBar{
    _topToolBar = [[NSBundle mainBundle]loadNibNamed:@"ZXHLayerTopBar" owner:self options:nil][0];
    UIView *bgView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 215, 44)];
    
    [self.view addSubview:bgView];
    [bgView addSubview:_topToolBar];
    
    [_topToolBar.btnDelete addTarget:self action:@selector(deleteLayer:) forControlEvents:UIControlEventTouchUpInside];
    [_topToolBar.btnMerge addTarget:self action:@selector(mergeLayer:) forControlEvents:UIControlEventTouchUpInside];
    [_topToolBar.btnCopy addTarget:self action:@selector(copyLayer:) forControlEvents:UIControlEventTouchUpInside];
    [_topToolBar.btnAdd addTarget:self action:@selector(addLayer:) forControlEvents:UIControlEventTouchUpInside];
}

#pragma mark - 创建表格
-(void)buildTableViewWithWidth:(CGFloat)w height:(CGFloat)h{
    _tbView = [[UITableView alloc]initWithFrame:CGRectMake(0, 44, w, h-104)];
    _tbView.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:_tbView];
    _tbView.dataSource = self;
    _tbView.delegate = self;
    _tbView.rowHeight = 124;
    [_tbView registerNib:[UINib nibWithNibName:@"LayersCell" bundle:nil] forCellReuseIdentifier:@"LayersCellId"];
    _tbView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    // 可编辑状态
    _tbView.editing = YES;
    _tbView.allowsSelectionDuringEditing = YES;
}

#pragma mark - 创建底部工具栏
-(void)buildBottomToolBarWithWidth:(CGFloat)w height:(CGFloat)h{
    UIView *bottomToolBar = [[UIView alloc]initWithFrame:CGRectMake(0, h-62, w,44)];
    bottomToolBar.backgroundColor = UIPopoverBackgroundColor;
    [self.view addSubview:bottomToolBar];
    
    // 滑动条
    UISlider *alphaSlider = [[UISlider alloc]initWithFrame:CGRectMake(10, (44-10)/2, w-70, 10)];
    alphaSlider.minimumValue = 0.0;
    alphaSlider.maximumValue = 1.0;
    alphaSlider.value = 1.0;
    alphaSlider.layer.cornerRadius = 4;
    alphaSlider.backgroundColor = kImageColor(@"slider_layer_track_bg");
    alphaSlider.minimumTrackTintColor = [UIColor clearColor];
    alphaSlider.maximumTrackTintColor = [UIColor clearColor];
    [alphaSlider setThumbImage:kImage(@"slider_layer_thumb_bg") forState:0];
    
    [alphaSlider addTarget:self action:@selector(changeLayerAlpha:) forControlEvents:UIControlEventValueChanged];
    [bottomToolBar addSubview:alphaSlider];
    
    // 数值文本
    _alphaLabel = [[UILabel alloc]initWithFrame:CGRectMake(alphaSlider.bounds.size.width+20, (44-20)/2, 50, 20)];
    _alphaLabel.font = kFontSize(14);
    _alphaLabel.text = @"100%";
    _alphaLabel.textColor = UIPopoverBorderColor;
    [bottomToolBar addSubview:_alphaLabel];
}


#pragma mark - 改变图层透明度
-(void)changeLayerAlpha:(UISlider*)slider{
    NSInteger value = slider.value * 100;
    _alphaLabel.text = [NSString stringWithFormat:@"%ld%%",value];
}

#pragma mark 表格视图回调
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return _layersCount;
}

// 复用
-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    LayersCell *cell = [tableView dequeueReusableCellWithIdentifier:@"LayersCellId"];
    cell.rowIndex = indexPath.row;
    
    cell.positionLabel.text = [NSString stringWithFormat:@"%ld",cell.rowIndex+1];
    
    if (!cell.isUnlocked) {
        [cell.btnUnlock setImage:[UIImage imageNamed:@"layer_lock"] forState:0];
    }else{
        [cell.btnUnlock setImage:[UIImage imageNamed:@"layer_unlock"] forState:0];
    }
    
    UIImage *layerImage = [[HYBrushCore sharedInstance] getLayerThumbnailOfIndex:indexPath.row];
    cell.outlineView.image = layerImage;
    
    if (!cell.selected) {
        [cell setOutlineViewBorderWithColor:kCommenCyanColor];
    }else{
        [cell setOutlineViewBorderWithColor:UIPopoverBackgroundColor];
    }
    return cell;
}

// 选中cell
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    _curLayerIndex = indexPath.row;
    
    LayersCell *cell = (LayersCell *)[tableView cellForRowAtIndexPath:indexPath];
    [cell setOutlineViewBorderWithColor:UIPopoverBackgroundColor];
    if (!cell.isUnlocked) {
        _topToolBar.btnDelete.enabled = NO;
    }else{
        _topToolBar.btnDelete.enabled = YES;
    }
    
    [self selectRowAtIndexPath:indexPath.row];
}

// 取消选中时
-(void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath{
    LayersCell *cell = (LayersCell *)[tableView cellForRowAtIndexPath:indexPath];
    [cell setOutlineViewBorderWithColor:kCommenCyanColor];
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
    LayersCell *cell1 = (LayersCell *)[tableView cellForRowAtIndexPath:sourceIndexPath];
    LayersCell *cell2 = (LayersCell *)[tableView cellForRowAtIndexPath:destinationIndexPath];
    
    NSLog(@"after: %ld",destinationIndexPath.row);
    NSInteger fromRow = sourceIndexPath.row;
    NSInteger toRow = destinationIndexPath.row;
    
    cell1.rowIndex = toRow;
    cell2.rowIndex = fromRow;
    // 改变数据源内容
    [[HYBrushCore sharedInstance]moveLayerFrom:fromRow to:toRow];
    
    [_tbView reloadData];
    if (cell1.selected) {
        [self selectRowAtIndexPath:toRow];
    }else{
        [self selectRowAtIndexPath:_curLayerIndex];
    }
}
@end
