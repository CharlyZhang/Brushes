//
//  ZXHShapeBoxController.m
//  HYDrawing
//
//  Created by macbook on 15/8/19.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "ZXHShapeBoxController.h"
#import "ZXHShapeBoxCell.h"

#define kBtnWidth [UIImage imageNamed:@"shapebox_btn_img_normal"].size.width
#define kBtnHeight [UIImage imageNamed:@"shapebox_btn_img_normal"].size.height

@interface ZXHShapeBoxController ()<UICollectionViewDataSource,UICollectionViewDelegate,UICollectionViewDelegateFlowLayout>


@end

@implementation ZXHShapeBoxController
{
    NSMutableArray *_dataSource;
    UICollectionView *_collectionView;
    CGSize preferSize;
}

-(instancetype)initWithPreferredContentSize:(CGSize)size{
    if (self = [super init]) {
        self.preferredContentSize = size;
        preferSize = size;
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 数据
    [self initData];
    
    // UI
    [self createUI];
}

#pragma mark 初始数据
-(void)initData{
    _dataSource = [NSMutableArray new];

    for (int j=0; j<3; j++) {
        for (int i=1; i<=8; i++) {
            NSString *name = [NSString stringWithFormat:@"shape%d",i];
            UIImage *img = [UIImage imageNamed:name];
            [_dataSource addObject:img];
        }
    }
}

#pragma mark 创建UI
-(void)createUI{
    // collectionView
    [self createCollectionView];
    
    // 按钮
    [self createButtons];
}

-(void)createCollectionView{
    UICollectionViewFlowLayout *flowLayout = [UICollectionViewFlowLayout new];
    _collectionView = [[UICollectionView alloc]initWithFrame:CGRectMake(kBtnWidth, 0, preferSize.width-kBtnWidth, preferSize.height-8) collectionViewLayout:flowLayout];
    
    // 注册xib
    [_collectionView registerNib:[UINib nibWithNibName:@"ZXHShapeBoxCell" bundle:nil] forCellWithReuseIdentifier:@"ShapeBoxCell"];
    flowLayout.scrollDirection = UICollectionViewScrollDirectionVertical;
    flowLayout.minimumInteritemSpacing = 20;
    flowLayout.minimumLineSpacing = 34;
    _collectionView.dataSource = self;
    _collectionView.delegate = self;
    [self.view addSubview:_collectionView];
    _collectionView.backgroundColor = [UIColor clearColor];
}


-(void)createButtons{
    UIButton *imgBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    imgBtn.frame = CGRectMake(0, 20, kBtnWidth, kBtnHeight);
    [imgBtn setImage:[UIImage imageNamed:@"shapebox_btn_img"] forState:UIControlStateNormal];
    [imgBtn setImage:[UIImage imageNamed:@"shapebox_btn_img"] forState:UIControlStateHighlighted];
    [self.view addSubview:imgBtn];
    
    UIButton *shapeBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    shapeBtn.frame = CGRectMake(0, 70, kBtnWidth, kBtnHeight);
    [shapeBtn setImage:[UIImage imageNamed:@"shapebox_btn_shape"] forState:UIControlStateNormal];
    [self.view addSubview:shapeBtn];
}


#pragma mark collectionView回调
// 个数
-(NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    return _dataSource.count;
}

// 大小
-(CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath{
    return CGSizeMake(60, 60);
}

// 边距
-(UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout insetForSectionAtIndex:(NSInteger)section{
    return UIEdgeInsetsMake(20, 20, 20, 20);
}

// 复用
-(ZXHShapeBoxCell*)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
   ZXHShapeBoxCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"ShapeBoxCell" forIndexPath:indexPath];
    cell.imgView.image = _dataSource[indexPath.item];
//    cell.backgroundColor = [UIColor redColor];
    
    return cell;
}

// 选中
-(void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    [self.delegate didSelectedShape:_dataSource[indexPath.item]];
}

@end
