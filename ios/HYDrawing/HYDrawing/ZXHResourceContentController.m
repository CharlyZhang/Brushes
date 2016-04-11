//
//  ZXHResourceContentController.m
//  HYDrawing
//
//  Created by zhuxuhong on 16/4/11.
//  Copyright © 2016年 Founder. All rights reserved.
//

#import "ZXHResourceContentController.h"
#import "ZXHResourceListController.h"
#import "SDWebImage/UIImageView+WebCache.h"

@implementation ResourcePictureCell

@end

@interface ZXHResourceContentController ()<ResourcePicturesDelegate, UICollectionViewDelegate, UICollectionViewDataSource>

@property (weak, nonatomic) IBOutlet UILabel *titleLabel;
@property (weak, nonatomic) IBOutlet UIButton *btnDownloadAll;
@property (weak, nonatomic) IBOutlet UICollectionView *collectionView;

@end

@implementation ZXHResourceContentController
{
	NSArray *_images;
}
static NSString * const reuseIdentifier = @"ResourcePictureCell";

- (void)viewDidLoad {
    [super viewDidLoad];
	
}

-(void)changePictures:(NSArray *)images{
	NSLog(@"hello: %@",images);
	_images = images;
	[_collectionView reloadData];
}

-(void)setTitle:(NSString *)title navTitle:(NSString *)navTitle{
	_titleLabel.text = title;
	self.navigationItem.title = navTitle;
}

#pragma mark <UICollectionViewDataSource>

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return _images.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    ResourcePictureCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
	[cell.imgView sd_setImageWithURL:_images[indexPath.row][@"url"]];
	
    return cell;
}

- (IBAction)downloadAll:(UIButton *)sender {
	
}

@end
