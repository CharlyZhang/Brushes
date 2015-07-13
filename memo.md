* 2014年9月24日14:17:23
	1. ! preview内的时事件注册函数需要在Util中实现
	2. ! Shader中的参数传递是死的，需要更新
	3. ! 所有指针，如果只是简单引用，则变量名应该改为以ptr开始
	4. ! FBO中或许不要添加深度缓存
	5. ? 为了减少空间，绘制纹理可以采用8uint类型
	6. ! 绘制过程中，还是将shader传给了path,这是没有必要的

* 2014年9月25日16:47:04：
	1. ? 编解码接口没完成
	2. ! path的绘制函数与shader 隔离

* 2014年9月26日10:53:51：
	1. ! windows上应该将glut换掉，自己生成Context,才能有多个绘制上下文
	2. !	收获：glTexEnvf控制的是纹理贴图的颜色和片段的颜色，得到最终的颜色；GL_BLEND控制的是片段颜色和缓存颜色的混合。
	3. ? 画刷纹理中的smallStamp没实现

* 2014年9月28日09:59:32：
	1. ! 开始编写FreeTool类，用以做输入控制

* 2014年9月29日17:04:06：
	1. ! 初步完成鼠标控制生成轨迹

* 2014年9月30日10:39:07:
	1. ! 设计渲染器，（与平台相关）
	2. ! 实现了windows平台渲染窗口

* 2014年10月8日17:25:21:
* 2014年10月9日17:22:21：
	1. ! NotificationCenter的解决方案-用委托模式解决，存在的问题是当监听者remove的时候
	2. ! 完善Brush类，对于uuid，采用的是uint代替了字符串
	3. ! Brush只负责保存参数，以及预览图，strokePreview应该存为图片，该图片的生成由生成器负责。

* 2014年10月10日14:29:23
	1. ! 完善preview类，将绘制部分放入render类
	2. ! 改写fbo类，其类型由set后决定，增加复用性
	3. ! 修改stampGenerator,生成的笔刷以图片格式取代纹理格式
	4. ! texture,fbo等类应该与context相关

* 2014年10月11日16:34:17
	1. ? preview的setup里面，关于给proj乘以scale是为了增加对原轨迹数据的缩放，待到IOS平台后验证。
	2. ! 修改fbo，让其可以反复设置绘制模式 

* 2014年10月13日10:11:31
	1. ! 修改path类，将绘制部分用render实现。
	2. ! generator不利用path生成图案。
	3. ? path纯范围bounds未做考虑。

* 2014年10月14日11:32:29
	1. ! 编写NotificationCenter类，实现消息通知。
	2. ! 将shader,texture移到render中
	3. ! 确定PaintingRender,PreviewRender,StampRender三种绘制器。
	4. ! 在render中确定绘制流程

* 2014年10月15日17:14:29
	1. ? 尝试将所有和gl有关的放进render中
	2. ! 完善render的接口
	3. ! 将CZImage转换成CZTexture的功能放到CZTexture中，CZImage为model，应该平台无关。

* 2014年10月16日09:50:19
	1.？ 窗口中应该加模板缓存
	2.？ UUID是字符串，这里没实现
	3.？ 没有实现撤销动作

* 2014年10月17日11:32:39
	1. ? painting中的width,和bounds都直接和dimensions相关，layer中的imageDataInRect用参数可以规范，以后测试。
	2. ! 增加render接口，使读取图像数据代码归入其中。
	3. ! 假设CZPainting只有一个实例，且CZPaintingRender的大小与其保持一致，activatePaintTexture也是同样大小。

* 2014年10月20日14:09:20
	1. ! CZPainting中的imageForCurrentState投影矩阵的重构无意义，删除代码。
	2. ! 完善shader。

* 2014年10月21日15:31:20
	1. ! 收获：同一个shader程序load两遍会出问题。
	2. ! 收获：传uniform变量之前，必须要use该shader的程序。
	3. ! 收获：对于pc上的glsl的顶点属性数据传输，用VAO没有问题，用属性数组不能传递位置数据。
	4. ! render还是主要采用委托模式，即painting,layer等都有自己的绘制动作，只是将其委托给render绘制。
	5. ! 需要按照上述思想，重新改回path，其也应该有独立的绘制动作，可以考虑通过函数重载接受两种不同的render。
	6. ! 收获：被委托者作为参数出现，委托者调用被委托者时可以将自己赋值给被委托者的内部指针，便于被委托者获取外部数据。

* 2014年10月22日21:44:25
	1. ! 重新编写三个render，将平台相关的绘制代码归于其中。
	2. ! 实现了用painting等参与的freetool.
	3. ! 将path的绘制功能恢复，仍然采用委托模型。

* 2014年10月23日18:27:15
	1. ! 思路：freehand将点收集，形成path；利用painting将轨迹绘制到activePaintTexture;
	2. ! 思路：最终绘制时调用painting的blit，painting将当前的轨迹activePaintTexture带颜色地合并（添加或擦除）
			到当前层activeLayer,同时遍历所有layer,将其texture混合出来。
	3. ? layer的commitStroke中会用textureName纹理同时做输入输出，按照es文档上说这会导致不确定性。
	4. ! 思路：painting将轨迹绘制到activePaintTexture后，会通过NotificationCenter让canvas定时刷新view,
			调用drawViewInRect将轨迹绘制到屏幕上。在鼠标弹起后，layer调用commitStroke将轨迹融合到其texture中。

* 2014年10月24日14:04:59
	1. !	解决绘制笔画到屏幕时的黑屏问题。
	2. ! 完成将绘制笔画合并到图层以及图层到屏幕的绘制。

* 2014年10月27日17:21:32
	1. ! 整理代码，画类图、时序图，写ppt

* 2014年10月28日17:21:32
	1. ! 完善CZActiveState，其不再继承Observer
	2. ! 将代码迁到Brushes项目，文件移动到src文件夹（没有将代码放入各自文件夹是由于避免需要多次改动工程的引用文件夹）
	
* 2014年10月29日15:52:52
	1. ! 将“回撤”和“通知”相关的功能剔除。

* 2014年10月30日15:52:52
	1. ! 规整关键类，同时用EA制作类图。
    
* 2014年11月13日：
    1. ! 收获：只有尺寸为2的幂的纹理能调用glGenerateMipmap自动生成纹理
    2. ! 收获：只有生成了mipmap的纹理能采用GL_MIPMAP_*的过滤方式
    3. ! 平台相关的代码不应该放进内核，利用Imp结构体进行隔离
    4. ! 上下文有资源控制者持有，并对其进行封装
    5. ? 需要将资源从Render中剥离，归还给真正持有者；对数据结构甚至接口来说都可以通过宏统一进行区分定义

* 2015年4月9日：
	1. !删除stampRender；
	2. !整理类stampGenerator、CZSpiralGenerator、CZProperty、CZGLContext、
			CZShader、CZTexture、CZFbo、CZImage、CZUtil、CZ2DPoint、CZSize、CZRect

* 2015年4月23日：
	1. !修改CZImage，自控制数据格式；	
	2. !修改CZTexture，去掉BrushTex模式，定义与CZImage一致的模式；	
					不再管理是否支持深度颜色
	3. !修改CZFbo，增加生成CZImage的方法

* 2015年4月24日：
	1. !修改Layer,其只能由Painting生成，并管理；
	2. !相应修改Painting对Layer的管理接口；
	3. !实现图层管理和图片绘制功能。

* 2015年4月27日：
	1. !修改CZStampGenerator,复用fbo和shader；
	2. !CZActiveState的单例改为懒人模式，使其适用于只有一个gl上下文的模式（win平台）；
	3. !完善螺旋笔触，使其能呈现不同亮度；
	4. !生成径向衰减笔触。

* 2015年4月28日：
	1. !修改CZBrush,其职能由ActiveState生成，并管理；
	2. !实现更改画刷功能。
	3. !stampGenerator的shader和fbo改为局部变量，利于资源利用。

* 2015年7月3日：
	1. !用深度搜索算法实现填充。

* 2015年7月9日：
    1. ?笔画／橡皮擦／填充／插图的撤销
    
* 2015年7月10日：
	1. !增加CZTexture修改内容的功能，避免填充时删除重建纹理；
	2. !在图层上实现撤销上一步绘制、擦除的功能；
	3. !在图层上实现重做撤销的功能；
	4. !在图层上实现插入图片的重做撤销功能。
	
* 2015年7月13日：
	1. !在图层上实现填充的上一步撤销和重做；
	2. !优化填充功能（只用填充的数据修改纹理）。
