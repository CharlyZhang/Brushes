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
5. 加入drawView对dirty部分rect的绘制
6. 将canvas的实现后缀改为.cpp
7. 插入图片后的撤销注册操作

bug:
3. 插入的图片显示不正确
4. 触摸其他popover后应该将当前popover消除

### FIXED:
1. 画布变换后的背景图片绘制错误bug  
2. 填充颜色后，图层缩略图没有立马改变bug
3. 初次启动后的新建绘画没有保存bug
4. 插入图片后，图层页码出错bug

### UPDATED:
1. 修改Canvas的底色设置方式
2. 对screenScale的处理，可提高绘制清晰度
3. Canvas的绘制部分移入到CZCanvas中
4. 更新插入图片的编辑方式
5. 增加画笔大小调整
6. painting的DIC跳转 
7. painting的删除
8. 增加画布变幻复原的手势（双指双击）



### CORE:
1. CZCanvas不仅仅作为core调用view的接口，同时承担着canvas的实体功能，由view去调用。
2. CZLayer的背景图片绘制方法