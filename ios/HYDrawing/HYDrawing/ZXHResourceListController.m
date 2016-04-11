//
//  ZXHResourceListController.m
//  HYDrawing
//
//  Created by zhuxuhong on 16/4/11.
//  Copyright © 2016年 Founder. All rights reserved.
//

#import "ZXHResourceListController.h"
#import "AFNetworking.h"

@interface ResourceListCell : UITableViewCell
@end

@implementation ResourceListCell

-(void)awakeFromNib{
	self.textLabel.textColor = [UIColor grayColor];
	self.backgroundColor = [UIColor colorWithRed:254/255.0 green:251/255.0 blue:234/255.0 alpha:1];
}

-(void)setSelected:(BOOL)selected{
	if (selected) {
		self.textLabel.textColor = [UIColor whiteColor];
		
		UIView *bgView = [UIView new];
		bgView.backgroundColor = [UIColor colorWithRed:242/255.0 green:177/255.0 blue:74/255.0 alpha:1];
		self.selectedBackgroundView = bgView;
	}
}


@end

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
		
//		NSLog(@"_chapts: %@",_chapts);
	}
}



@end


@interface ZXHResourceListController ()

@end

@implementation ZXHResourceListController
{
	NSMutableArray *_dataSource;
	NSString *navTitle;
}
- (void)viewDidLoad {
    [super viewDidLoad];
	
	UINavigationController *nav = [self.splitViewController.viewControllers lastObject];
	self.delegate = nav.topViewController;
	
	UIView *borderView = [[UIView alloc]initWithFrame:CGRectMake(319, 0, 1, self.view.bounds.size.height)];
	borderView.backgroundColor = [UIColor colorWithRed:146/255.0 green:107/255.0 blue:35/255.0 alpha:1];
	[self.view addSubview:borderView];
	
	
	[self initData];
}


// init data
-(void)initData{
	_dataSource = [NSMutableArray new];
	
	NSURL *url = [NSURL URLWithString:@"https://dn-huiyun.qbox.me/chengzimeishu.json"];
	AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
	[manager GET:url.absoluteString parameters:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
		
		NSArray *content = responseObject[0][@"content"];
		navTitle = responseObject[0][@"publisher"];
		
		for (NSDictionary *dic in content) {
			ResourceListModel* model = [ResourceListModel new];
			[model setValuesForKeysWithDictionary:dic];
			
			[_dataSource addObject:model];
		}
		// 切换数据
		ResourceListModel *model = (ResourceListModel*)_dataSource[0];
		[self.delegate changePictures: model.chapts[0][@"images"]];
		[self.delegate setTitle:model.chapts[0][@"chapter"] navTitle:navTitle];
		model.isOn = YES;
//		NSLog(@"_dataSource: %ld",_dataSource.count);
		
		[self.tableView reloadData];
		
	} failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
		NSLog(@"initData Error: %@",error);
	}];
}

- (IBAction)dismiss:(UIBarButtonItem *)sender {
	[self.splitViewController dismissViewControllerAnimated:true completion:nil];
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return _dataSource.count;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	ResourceListModel *model = (ResourceListModel*)_dataSource[section];
	if (model.isOn) {
		return model.chapts.count;
	}
	return 0;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"ResourceListCell" forIndexPath:indexPath];
	NSArray *chapts = ((ResourceListModel*)_dataSource[indexPath.section]).chapts;
	NSDictionary *chapter = chapts[indexPath.row];
	
	cell.textLabel.text = chapter[@"chapter"];
	if (indexPath.row ==0 && indexPath.section==0) {
		[tableView selectRowAtIndexPath:indexPath animated:NO scrollPosition:UITableViewScrollPositionTop];
	}
	
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
	return 50;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
	return 44;
}

-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
	CGFloat sectionHeight = 50;
	CGFloat paddingLeft = 15;
	ResourceListModel *model = (ResourceListModel*)_dataSource[section];
	
	UIView *view = [[UIView alloc]initWithFrame:CGRectMake(0, 0, self.view.bounds.size.width, 44)];
	UILabel *label = [[UILabel alloc]initWithFrame:CGRectMake(paddingLeft, 0, 200, sectionHeight)];
	view.backgroundColor = [UIColor colorWithRed:254/255.0 green:251/255.0 blue:234/255.0 alpha:1];
	
	// border-bottom
	UIView *borderViewBottom = [[UIView alloc]initWithFrame:CGRectMake(0, 49, 320, 1)];
	borderViewBottom.backgroundColor = [UIColor colorWithRed:214/255.0 green:202/255.0 blue:126/255.0 alpha:1];
	[view addSubview:borderViewBottom];
	
	// border
	UIView *borderView = [[UIView alloc]initWithFrame:CGRectMake(319, 0, 1, sectionHeight)];
	borderView.backgroundColor = [UIColor colorWithRed:146/255.0 green:107/255.0 blue:35/255.0 alpha:1];
	[view addSubview:borderView];
	
	label.text = model.grade;
	view.tag = section+1000;
	[view addSubview:label];
	//tap 手势
	UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(openOrCloseSection:)];
	[view addGestureRecognizer:tap];
	
	CGFloat x = self.view.bounds.size.width - 14 - paddingLeft;
	UIImageView *icon = [[UIImageView alloc]initWithFrame:CGRectMake(x, sectionHeight/2-4, 14, 8)];
	if (model.isOn) {
		icon.image = [UIImage imageNamed:@"res_icon_arrow_up"];
	}else{
		icon.image = [UIImage imageNamed:@"res_icon_arrow_down"];
	}
	[view addSubview:icon];
	
	return  view;
}

// 卷展
-(void)openOrCloseSection: (UITapGestureRecognizer*)tap{
	NSInteger section = tap.view.tag - 1000;
	ResourceListModel *model = (ResourceListModel*)_dataSource[section];
	model.isOn = !model.isOn;
	[self.tableView reloadSections:[NSIndexSet indexSetWithIndex:section] withRowAnimation:UITableViewRowAnimationFade];
}

// 选中
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
	ResourceListCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
	if (cell.selected) {
		return;
	}
	
	[self selectCell:indexPath];
	
	// 切换数据
	[self changePictures:indexPath];
	
	NSLog(@"%d", cell.selected);
}

-(void)changePictures: (NSIndexPath*)indexPath{
	NSArray *chapts = ((ResourceListModel*)_dataSource[indexPath.section]).chapts;
	NSDictionary *chapter = chapts[indexPath.row];
	[self.delegate changePictures:chapter[@"images"]];
	[self.delegate setTitle:chapter[@"chapter"] navTitle:navTitle];
}

-(void)selectCell:(NSIndexPath*) indexPath{
	ResourceListCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
	if (cell.selected) {
		return;
	}
	
	cell.textLabel.textColor = [UIColor whiteColor];
	
	UIView *bgView = [UIView new];
	bgView.backgroundColor = [UIColor colorWithRed:242/255.0 green:177/255.0 blue:74/255.0 alpha:1];
	cell.selectedBackgroundView = bgView;

}

-(void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath{
	ResourceListCell *cell = [tableView cellForRowAtIndexPath:indexPath];
	cell.textLabel.textColor = [UIColor grayColor];
}


@end
