//
//  ZXHPaintingListController.m
//  HYDrawing
//
//  Created by macbook on 15/8/24.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ZXHPaintingListController.h"
#import "Macro.h"
#import "ZXHPaintingListCell.h"

@interface ZXHPaintingListController ()<UITableViewDataSource,UITableViewDelegate>

@end

@implementation ZXHPaintingListController
{
    UIButton *editButton;
    UITableView *tbView;
    CGSize preferredSize;
    BOOL isListEditing;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    preferredSize = self.preferredContentSize;
    isListEditing = NO;
    
    [self createUI];
}

#pragma mark UI
-(void)createUI{
    // 编辑按钮
    UIImage *img = kImage(@"list_btn_edit_normal");
    editButton = [UIButton buttonWithType:UIButtonTypeCustom];
    editButton.frame = CGRectMake(preferredSize.width-40-10, 8+42/2-40/2, 40, 40);
    [editButton addTarget:self action:@selector(editButtonClicked:) forControlEvents:UIControlEventTouchUpInside];
    [editButton setImage:img forState:0];
    [self.view addSubview:editButton];
    
    // 表格
    tbView = [[UITableView alloc]initWithFrame:CGRectMake(2, 55, preferredSize.width-4, preferredSize.height-60)];
    [self.view addSubview:tbView];
    tbView.dataSource = self;
    tbView.delegate = self;
    tbView.rowHeight = 130;
    tbView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    // 注册xib
    [tbView registerNib:[UINib nibWithNibName:@"ZXHPaintingListCell" bundle:nil] forCellReuseIdentifier:@"PaintingListCell"];
    tbView.backgroundColor = [UIColor clearColor];
}

#pragma mark 编辑
-(void)editButtonClicked:(UIButton*)btn{
    if (!isListEditing) {
        [btn setImage:kImage(@"list_btn_edit_selected") forState:0];
    }else{
        [btn setImage:kImage(@"list_btn_edit_normal") forState:0];
    }
    isListEditing = !isListEditing;
    [tbView reloadData];
}

#pragma mark 表格回调
// 行数
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return 5;
}

// 复用
-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    ZXHPaintingListCell *cell = [tableView dequeueReusableCellWithIdentifier:@"PaintingListCell"];
    
    if (!isListEditing) {
        cell.btnDelete.hidden = YES;
        cell.btnRename.hidden = YES;
        cell.imgViewRightCons.constant = 20;
        cell.nameLabelRightCons.constant = 20;
    }else{
        cell.btnDelete.hidden = NO;
        cell.btnRename.hidden = NO;
        cell.imgViewRightCons.constant = 8;
        cell.nameLabelRightCons.constant = 10;
    }
    
    cell.imgView.image = kImage(@"defaultPaintingImage");
    cell.nameLabel.text = @"hello";
    cell.selectedBackgroundView = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"list_cell_selectedBg"]];
    
    return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    ZXHPaintingListCell *cell = (ZXHPaintingListCell *)[tableView cellForRowAtIndexPath:indexPath];
    [cell setBorderStyleWithColor:UIPopoverBorderColor];
}

-(void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath{
    ZXHPaintingListCell *cell = (ZXHPaintingListCell *)[tableView cellForRowAtIndexPath:indexPath];
    [cell setBorderStyleWithColor:kCommenCyanColor];
}

@end
