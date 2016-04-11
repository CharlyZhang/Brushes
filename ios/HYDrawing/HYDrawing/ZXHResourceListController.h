//
//  ZXHResourceListController.h
//  HYDrawing
//
//  Created by zhuxuhong on 16/4/11.
//  Copyright © 2016年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ZXHResourceContentController.h"

//data model
@interface ResourceListModel : NSObject

@property(nonatomic, strong)NSString *grade;
@property(nonatomic, strong)NSMutableArray *chapts;
@property(nonatomic, assign)BOOL isOn;

@end


// vc
@interface ZXHResourceListController : UITableViewController

@property(nonatomic,weak) id delegate;

@end
