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
#import "AFNetworking/AFNetworking.h"
#import "HYBrushCore.h"

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
	NSOperationQueue *_downloadQueue;
	NSMutableArray *_operationList;
}

static NSString * const reuseIdentifier = @"ResourcePictureCell";

- (void)viewDidLoad {
    [super viewDidLoad];
	
	_downloadQueue = [NSOperationQueue new];
	_downloadQueue.maxConcurrentOperationCount = 1;
	_operationList = [NSMutableArray new];
}


-(void)changePictures:(NSArray *)images{
	NSLog(@"hello: %@",images);
	_images = images;
	[_collectionView reloadData];
}

-(void)setTitle:(NSString *)title{
	_titleLabel.text = title;
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
	[cell.imgView sd_setImageWithURL:_images[indexPath.item][@"url"]];
	[cell.downloadBtn addTarget:self action:@selector(download:) forControlEvents:UIControlEventTouchUpInside];
	cell.downloadBtn.tag = 1000+indexPath.item;
	
    return cell;
}

//download
- (void)download:(UIButton *)sender {
	NSIndexPath *indexPath = [NSIndexPath indexPathForItem:sender.tag-1000 inSection:0];
	[self downloading:indexPath];
	
}

-(void)downloading: (NSIndexPath*)indexPath{
	ResourcePictureCell *cell = (ResourcePictureCell*)[self.collectionView cellForItemAtIndexPath:indexPath];
	cell.downloadBtn.hidden = true;
	UIActivityIndicatorView *indicator = [[UIActivityIndicatorView alloc]initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
	indicator.tag = indexPath.item + 10000;
	indicator.center = cell.downloadBtn.center;
	[cell.maskView addSubview:indicator];
	
	[self startDownloadImageWithIndexPath: indexPath];
}

// startDownload
-(void)startDownloadImageWithIndexPath: (NSIndexPath*)indexPath{
	ResourcePictureCell *cell = (ResourcePictureCell*)[self.collectionView cellForItemAtIndexPath:indexPath];
	NSURL *URL = [NSURL URLWithString:_images[indexPath.item][@"url"]];
	AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
	UIActivityIndicatorView *indicator = [cell.maskView viewWithTag:indexPath.item + 10000];
	
	NSURLSessionDownloadTask *downloadTask = [manager downloadTaskWithRequest:[NSURLRequest requestWithURL:URL] progress:nil destination:^NSURL * _Nonnull(NSURL * _Nonnull targetPath, NSURLResponse * _Nonnull response) {
		NSURL *documentsDirectoryURL = [[NSFileManager defaultManager] URLForDirectory:NSDocumentDirectory inDomain:NSUserDomainMask appropriateForURL:nil create:NO error:nil];
//		return [documentsDirectoryURL URLByAppendingPathComponent:[response suggestedFilename]];
		NSString *fileType = [[[response suggestedFilename]componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"."]] lastObject];
		return [documentsDirectoryURL URLByAppendingPathComponent:[NSString stringWithFormat:@"%@.%@",_images[indexPath.item][@"name"],fileType]];
		
	} completionHandler:^(NSURLResponse * _Nonnull response, NSURL * _Nullable filePath, NSError * _Nullable error) {
		if (error) {
			cell.maskView.hidden = false;
			cell.downloadBtn.hidden = false;
			[indicator stopAnimating];
			NSLog(@"下载失败: %@",[error description]);
			return;
		}
		NSLog(@"File downloaded to: %@", filePath);
		cell.maskView.hidden = true;
		[indicator stopAnimating];
	}];
	[downloadTask resume];
	[indicator startAnimating];
}

- (IBAction)downloadAll:(UIButton *)sender {
	sender.enabled = false;
	
	for (int i=0; i<_images.count; i++) {
		NSIndexPath *indexPath = [NSIndexPath indexPathForItem:i inSection:0];
		NSBlockOperation *op = [NSBlockOperation blockOperationWithBlock:^{
			dispatch_async(dispatch_get_main_queue(), ^{
				[self downloading:indexPath];
			});
		}];
		[_downloadQueue addOperation:op];
	}
}

-(void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
	NSString *document = [NSHomeDirectory() stringByAppendingString:@"/Documents"];
	NSString *name = [[_images[indexPath.item][@"url"]componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"/"]] lastObject];
	NSString *type = [[name componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"."]] lastObject];
	NSString *filePath = [NSString stringWithFormat:@"%@/%@.%@",document,_images[indexPath.item][@"name"],type];
	NSLog(@"filePath: %@",filePath);
	
	UIImage *img = [UIImage imageWithContentsOfFile:[NSString stringWithFormat:@"%@/pic.jpg",document]];
	[self.delegate didSelectImage:img];
}

@end
