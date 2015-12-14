### interface
        与内核相关的中间层
    * HYBrushCore   - 封装界面需要的所有OC接口
    * CZViewImpl    - CZView类的实现，包含OC的实际视图类，可为CPP代码调用
    * CanvasView    - UIview的子类，为OC界面绘制的实际视图，可为OC代码调用
    
    
### TO DO:
1. 改画：应用跳进来时图片作背景呈现；图层需要保存和加载（序列化数据）； !important  加载的painting和设备尺寸不一样  
2. 绘画：返回阅读器，自动保存为缩略图呈现在阅读器；
3. 设计: 画布大小改变；                                         'important
4. 笔触大小调整；                                              done


to do:

1. 转屏
2. painting文件的尺寸和本设备不相同

bug:

1. 填充颜色后，图层缩率图没有立马改变
2. 触摸其他popover后应该将当前popover消除


