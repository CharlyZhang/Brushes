//
//  ZXHSettingViewController.m
//  HYDrawing
//
//  Created by macbook on 15/8/24.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ZXHSettingViewController.h"
#import "Macro.h"


@interface ZXHSettingViewController ()<UITableViewDataSource,UITableViewDelegate>


@end

@implementation ZXHSettingViewController
{
    NSArray *titleArray;
    UITableView *_tbView;
    CGSize preferSize;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    titleArray = @[@"新建",@"清空",@"变换"];
    preferSize = self.preferredContentSize;
    
    _tbView = [[UITableView alloc]initWithFrame:CGRectMake(2, 6, preferSize.width-4, preferSize.height-6)];
    _tbView.dataSource = self;
    _tbView.delegate = self;
    [self.view addSubview:_tbView];
    _tbView.rowHeight = 49;
    _tbView.backgroundColor = [UIColor clearColor];
    _tbView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    self.view.backgroundColor = kImageColor(@"setting_popover_bg");
}

#pragma mark - Table view data source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 3;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *strId = @"CellId";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:strId];
    
    if (!cell) {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:strId];
    }
    
    cell.textLabel.text = titleArray[indexPath.row];
    cell.textLabel.textAlignment = NSTextAlignmentCenter;
    cell.backgroundColor = [UIColor clearColor];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    return cell;
}

// 选中
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    switch (indexPath.row) {
        case 0: // 新建
            [self.delegate settingForCreateNewCanvas];
            
            break;
        case 1: // 清空
            [self.delegate settingForClearCanvas];
            
            break;
        case 2: // 变换
            [self.delegate settingForTransformCanvas];
            
            break;
        default:
            break;
    }
    UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
    cell.backgroundColor = UIPopoverBackgroundColor;
}

-(void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath{
    UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
    cell.backgroundColor = [UIColor clearColor];
}

@end
