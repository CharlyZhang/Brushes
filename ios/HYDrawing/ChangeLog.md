### TO DO:
1. 改画：应用跳进来时图片作背景呈现；图层需要保存和加载（序列化数据）； !important  加载的painting和设备尺寸不一样  
2. 绘画：返回阅读器，自动保存为缩略图呈现在阅读器；
3. 设计: 画布大小改变；                                         'important
4. 笔触大小调整；                                              done


to do:
1. 转屏
2. painting文件的尺寸和本设备不相同
3. 统一shader的加载方式
4. painting的flattenMode
5. 变换canvas后的插图功能修正ing

bug:
0. 初次启动后的新建绘画没有保存
1. 填充颜色后，图层缩率图没有立马改变
2. 触摸其他popover后应该将当前popover消除



### FIXED:
1. painting的DIC跳转   
2. 增加画笔大小调整

### UPDATED:
1. 修改Canvas的底色设置方式
2. 对screenScale的处理，可提高绘制清晰度
