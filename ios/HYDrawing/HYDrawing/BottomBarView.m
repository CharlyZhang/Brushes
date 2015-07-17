//
//  BottomBarView.m
//  HYDrawing
//
//  Created by CharlyZhang on 15/7/17.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "BottomBarView.h"

@interface BottomBarView()
{
    UIButton *currentButton;
}


@end

@implementation BottomBarView

#pragma mark - Properties

- (UIButton*)colorWheelButton {
    if (!_colorWheelButton) {
        _colorWheelButton = [[UIButton alloc] init];
        [_colorWheelButton setImage:[UIImage imageNamed:@"color_wheel"] forState:UIControlStateNormal];
        
    }
    
    return _colorWheelButton;
}

- (UIButton*)eraserButton {
    if (!_eraserButton) {
        _eraserButton = [[UIButton alloc]init];
        [_eraserButton setImage:[UIImage imageNamed:@"eraser"] forState:UIControlStateNormal];
        [_eraserButton setImage:[UIImage imageNamed:@"eraser_sel"] forState:UIControlStateSelected];
    }
    return _eraserButton;
}

- (UIButton*)pencilButton {
    if (!_pencilButton) {
        _pencilButton = [[UIButton alloc]init];
        [_pencilButton setImage:[UIImage imageNamed:@"pencil"] forState:UIControlStateNormal];
        [_pencilButton setImage:[UIImage imageNamed:@"pencil_sel"] forState:UIControlStateSelected];
        [_pencilButton addTarget:self action:@selector(tapPencilButton:) forControlEvents:UIControlEventTouchUpInside];

    }
    return _pencilButton;
}

- (UIButton*)markerPenButton {
    if (!_markerPenButton) {
        _markerPenButton = [[UIButton alloc]init];
        [_markerPenButton setImage:[UIImage imageNamed:@"marker"] forState:UIControlStateNormal];
        [_markerPenButton setImage:[UIImage imageNamed:@"marker_sel"] forState:UIControlStateSelected];
    }
    
    return _markerPenButton;
}

- (UIButton*)colorBrushButton {
    if (!_colorBrushButton) {
        _colorBrushButton = [[UIButton alloc]init];
        [_colorBrushButton setImage:[UIImage imageNamed:@"color_brush"] forState:UIControlStateNormal];
        [_colorBrushButton setImage:[UIImage imageNamed:@"color_brush_sel"] forState:UIControlStateSelected];
    }
    
    return _colorBrushButton;
}

- (UIButton*)crayonButton {
    if (!_crayonButton) {
        _crayonButton = [[UIButton alloc]init];
        [_crayonButton setImage:[UIImage imageNamed:@"crayon"] forState:UIControlStateNormal];
        [_crayonButton setImage:[UIImage imageNamed:@"crayon_sel"] forState:UIControlStateSelected];
    }
    
    return _crayonButton;
}

- (UIButton*)bucketButton {
    if (!_bucketButton) {
        _bucketButton = [[UIButton alloc]init];
        [_bucketButton setImage:[UIImage imageNamed:@"bucket"] forState:UIControlStateNormal];
        [_bucketButton setImage:[UIImage imageNamed:@"bucket_sel"] forState:UIControlStateSelected];
    }
    
    return _bucketButton;
}

- (UIButton*)eyedropperButton {
    if (!_eyedropperButton) {
        _eyedropperButton = [[UIButton alloc]init];
        [_eyedropperButton setImage:[UIImage imageNamed:@"eyedropper"] forState:UIControlStateNormal];
        [_eyedropperButton setImage:[UIImage imageNamed:@"eyedropper_sel"] forState:UIControlStateSelected];
    }
    
    return _eyedropperButton;
}

- (UIButton*)canvasButton {
    if (!_canvasButton) {
        _canvasButton = [[UIButton alloc]init];
        [_canvasButton setImage:[UIImage imageNamed:@"canvas"] forState:UIControlStateNormal];
        [_canvasButton setImage:[UIImage imageNamed:@"canvas_sel"] forState:UIControlStateSelected];
    }
    
    return _canvasButton;
}

- (UIButton*)clipButton {
    if (!_clipButton) {
        _clipButton = [[UIButton alloc]init];
        [_clipButton setImage:[UIImage imageNamed:@"clip"] forState:UIControlStateNormal];
        [_clipButton setImage:[UIImage imageNamed:@"clip_sel"] forState:UIControlStateSelected];
    }
    
    return _clipButton;
}

- (UIButton*)layersButton {
    if (!_layersButton) {
        _layersButton = [[UIButton alloc]init];
        [_layersButton setImage:[UIImage imageNamed:@"layer"] forState:UIControlStateNormal];
    }
    
    return _layersButton;
}
//- (UIButton&)
#pragma mark - UIView Methods

- (instancetype) init {
    if(self = [super init]) {
        /// setting
        
        /// add subviews
        UIImageView *backgroundView = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"bottom_bar"]];
        [self addSubview:backgroundView];
        [self addSubview:self.colorWheelButton];
        [self addSubview:self.eraserButton];
        [self addSubview:self.pencilButton];
        [self addSubview:self.markerPenButton];
        [self addSubview:self.colorBrushButton];
        [self addSubview:self.crayonButton];
        [self addSubview:self.bucketButton];
        [self addSubview:self.eyedropperButton];
        [self addSubview:self.canvasButton];
        [self addSubview:self.clipButton];
        [self addSubview:self.layersButton];
        /// add constraints
        [self addConstrainsForSubViews];
    }
    
    return self;
}

#pragma mark - BottomBarView Methods

- (void) addConstrainsForSubViews {
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(_colorWheelButton,_eraserButton,_pencilButton,
                                                                   _markerPenButton,_colorBrushButton,_crayonButton,
                                                                   _bucketButton,_eyedropperButton,_canvasButton,
                                                                   _clipButton,_layersButton);
    NSDictionary *metrics = @{@"hPadding1" :@11,@"hPadding2" :@8,@"vPadding" :@8,@"vHeight":@98,@"vWidth":@73};
    NSArray *constraints = [NSLayoutConstraint
                            constraintsWithVisualFormat:@"H:|-hPadding1-[_colorWheelButton(vWidth)]-hPadding1-[_eraserButton(vWidth)]-hPadding1-[_pencilButton(vWidth)]-hPadding2-[_markerPenButton(vWidth)]-hPadding2-[_colorBrushButton(vWidth)]-hPadding2-[_crayonButton(vWidth)]-hPadding2-[_bucketButton(vWidth)]-hPadding2-[_eyedropperButton(vWidth)]-hPadding2-[_canvasButton(vWidth)]-hPadding2-[_clipButton(vWidth)]-hPadding1-[_layersButton(vWidth)]-|"
                            options:0
                            metrics:metrics
                            views:viewsDictionary];
    
    /// deal with vertical constraints
    _colorWheelButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                            constraintsWithVisualFormat:@"V:[_colorWheelButton(vHeight)]|"
                            options:0
                            metrics:metrics
                            views:viewsDictionary]];
    
    _eraserButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                                                              constraintsWithVisualFormat:@"V:[_eraserButton(vHeight)]|"
                                                              options:0
                                                              metrics:metrics
                                                              views:viewsDictionary]];
    
    _pencilButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                                                              constraintsWithVisualFormat:@"V:[_pencilButton(vHeight)]|"
                                                              options:0
                                                              metrics:metrics
                                                              views:viewsDictionary]];
    
    _markerPenButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                                                              constraintsWithVisualFormat:@"V:[_markerPenButton(vHeight)]|"
                                                              options:0
                                                              metrics:metrics
                                                              views:viewsDictionary]];
    
    _colorBrushButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                                                              constraintsWithVisualFormat:@"V:[_colorBrushButton(vHeight)]|"
                                                              options:0
                                                              metrics:metrics
                                                              views:viewsDictionary]];
    
    _crayonButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                                                              constraintsWithVisualFormat:@"V:[_crayonButton(vHeight)]|"
                                                              options:0
                                                              metrics:metrics
                                                              views:viewsDictionary]];
    
    _bucketButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                                                              constraintsWithVisualFormat:@"V:[_bucketButton(vHeight)]|"
                                                              options:0
                                                              metrics:metrics
                                                              views:viewsDictionary]];
    
    _eyedropperButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                                                              constraintsWithVisualFormat:@"V:[_eyedropperButton(vHeight)]|"
                                                              options:0
                                                              metrics:metrics
                                                              views:viewsDictionary]];
    
    _canvasButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                                                              constraintsWithVisualFormat:@"V:[_canvasButton(vHeight)]|"
                                                              options:0
                                                              metrics:metrics
                                                              views:viewsDictionary]];
    
    _clipButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                                                              constraintsWithVisualFormat:@"V:[_clipButton(vHeight)]|"
                                                              options:0
                                                              metrics:metrics
                                                              views:viewsDictionary]];
    
    _layersButton.translatesAutoresizingMaskIntoConstraints = NO;
    constraints = [constraints arrayByAddingObjectsFromArray:[NSLayoutConstraint
                                                              constraintsWithVisualFormat:@"V:[_layersButton(vHeight)]|"
                                                              options:0
                                                              metrics:metrics
                                                              views:viewsDictionary]];
    [self addConstraints:constraints];
}

#pragma mark - Actions

- (void)tapPencilButton:(UIButton*)sender{
    sender.selected = true;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
