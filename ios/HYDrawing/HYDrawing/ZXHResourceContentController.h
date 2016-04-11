//
//  ZXHResourceContentController.h
//  HYDrawing
//
//  Created by zhuxuhong on 16/4/11.
//  Copyright © 2016年 Founder. All rights reserved.
//

#import <UIKit/UIKit.h>



@interface ResourcePictureCell : UICollectionViewCell
@property (weak, nonatomic) IBOutlet UIImageView *imgView;

@end


@protocol ResourcePicturesDelegate <NSObject>

-(void)changePictures:(NSArray*) urls;
-(void)setTitle:(NSString*) title navTitle:(NSString*)navTitle;

@end

@interface ZXHResourceContentController : UIViewController

@end
