//
//  ZXHResourcePicturesController.m
//  HYDrawing
//
//  Created by zhuxuhong on 16/4/11.
//  Copyright © 2016年 Founder. All rights reserved.
//

#import "ZXHResourcePicturesController.h"
#import "ZXHResourceListController.h"
#import "AFNetworking.h"

// data model
@implementation ResourceListModel
-(instancetype)init{
	if (self = [super init]) {
		_chapts = [NSMutableArray new];
		_isOn = NO;
	}
	return self;
}

-(void)setValue:(id)value forUndefinedKey:(NSString *)key{
	if ([key isEqualToString:@"chapters"]) {
		[_chapts addObject:value];
	}
}
@end

@implementation ZXHResourcePicturesController
{
	NSMutableArray *_dataSource;
	UIButton *_navTitleButton;
	ZXHResourceListController *_listVC;
	ZXHResourceContentController *_contentVC;
}

- (void)viewDidLoad {
    [super viewDidLoad];
	_dataSource = [NSMutableArray new];
	CGFloat listVCWidth = 320;
	
	self.navigationController.navigationBar.translucent = NO;
	self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:1.0 green:229/255.0 blue:174/255.0 alpha:1];
//	286 255 201
	self.navigationController.navigationBar.shadowImage = [UIImage imageNamed:@"nav_shadow"];
	_navTitleButton = [UIButton buttonWithType:UIButtonTypeCustom];
	[_navTitleButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
//	[_navTitleButton addTarget:self action:@selector(loadData) forControlEvents:UIControlEventTouchUpInside];
	self.navigationItem.titleView = _navTitleButton;

	self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc]initWithImage:[UIImage imageNamed:@"res_icon_nav_back"] style:UIBarButtonItemStylePlain target:self action:@selector(back)];
	
	_listVC = [[UIStoryboard storyboardWithName:@"Main" bundle:nil] instantiateViewControllerWithIdentifier:@"ZXHResourceListController"];
	_listVC.preferredContentSize = CGSizeMake(320, self.view.bounds.size.height);
	_listVC.view.frame = CGRectMake(0, 0, listVCWidth, self.view.bounds.size.height);
	[self.view addSubview:_listVC.view];
	[self addChildViewController:_listVC];
	
	
	//right borderView
	UIView *borderView = [[UIView alloc]initWithFrame:CGRectMake(listVCWidth, 0, 1, self.view.bounds.size.height)];
	borderView.backgroundColor = [UIColor colorWithRed:146/255.0 green:107/255.0 blue:35/255.0 alpha:1];
	[self.view addSubview:borderView];
	
	_contentVC = [[UIStoryboard storyboardWithName:@"Main" bundle:nil] instantiateViewControllerWithIdentifier:@"ZXHResourceContentController"];
	CGFloat w = self.view.bounds.size.width - listVCWidth - 1;
	_contentVC.preferredContentSize = CGSizeMake(w, self.view.bounds.size.height);
	_contentVC.view.frame = CGRectMake(listVCWidth+1, 0, w, self.view.bounds.size.height);
	[self.view addSubview:_contentVC.view];
	[self addChildViewController:_contentVC];
	_contentVC.delegate = self;
	
	_listVC.delegate = _contentVC;
	
	// load data
	[self loadData];
}

-(void)back{
	[self.navigationController popToRootViewControllerAnimated:true];
}

// load data
-(void)loadData{
	
	NSURL *url = [NSURL URLWithString:@"https://dn-huiyun.qbox.me/chengzimeishu.json"];
	AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
	[manager GET:url.absoluteString parameters:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
		
		NSArray *content = responseObject[0][@"content"];
		[_navTitleButton setTitle:responseObject[0][@"publisher"] forState:UIControlStateNormal];
		
		for (NSDictionary *dic in content) {
			ResourceListModel* model = [ResourceListModel new];
			[model setValuesForKeysWithDictionary:dic];
			
			[_dataSource addObject:model];
		}
		// 切换数据
		_listVC.dataSource = _dataSource;
		ResourceListModel *model = (ResourceListModel*)_listVC.dataSource[0];
		model.isOn = YES;
		[_listVC.tableView reloadData];
		
		[_listVC.delegate changePictures: model.chapts[0][@"images"]];
		[_listVC.delegate setTitle:model.chapts[0][@"chapter"]];
		
	} failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
		NSLog(@"initData Error: %@",error);
	}];
}

-(void)didSelectImage:(UIImage *)image{
	[self back];
	[self.delegate didSelectImage:image];
}

@end
