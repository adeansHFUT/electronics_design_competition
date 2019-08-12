# electronics_design_competition 2019 HFUT
### 选题：H
#### 完成内容：
> * 设定角度和距离并发射出去（可完成基础题全部）
> * 根据红色引导自动寻靶，超声波自动测距射击（可完成发挥一）
> * 摇头时找靶射击（发挥二）
> * 实现高射和低射切换（算是其他发挥部分？）

* 关于建了模的反思：最简单和实用的方法是直接试验构建个射出的距离和俯仰舵机pwm对应的线性表，之后可直接查表也和拟合出函数；但我比赛时我傻了，设定炮弹只受重力以及忽略炮管长度，则射出距离
<br><a href="https://www.codecogs.com/eqnedit.php?latex=L=\frac{2*sin\alpha*v_{0}}{g}*cos\alpha&space;*v_{0}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?L=\frac{2*sin\alpha*v_{0}}{g}*cos\alpha&space;*v_{0}" title="L=\frac{2*sin\alpha*v_{0}}{g}*cos\alpha *v_{0}" /></a>
&emsp;&emsp;&emsp;&emsp;(<a href="https://www.codecogs.com/eqnedit.php?latex=\alpha" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\alpha" title="\alpha" /></a>是炮管仰角)
<br><a href="https://www.codecogs.com/eqnedit.php?latex=L" target="_blank"><img src="https://latex.codecogs.com/gif.latex?L" title="L" /></a>和<a href="https://www.codecogs.com/eqnedit.php?latex=sin\alpha*cos\alpha" target="_blank"><img src="https://latex.codecogs.com/gif.latex?sin\alpha*cos\alpha" title="sin\alpha*cos\alpha" /></a>成正比，比例为<a href="https://www.codecogs.com/eqnedit.php?latex=k_{rate}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?k_{rate}" title="k_{rate}" /></a>，先试验出<a href="https://www.codecogs.com/eqnedit.php?latex=k_{rate}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?k_{rate}" title="k_{rate}" /></a>就可以根据设定的距离求出炮管的仰角，再根据仰角就可以求出舵机的输出pwm，想想好像没问题哈哈。。。，但最后发现不同的距离对应<a href="https://www.codecogs.com/eqnedit.php?latex=k_{rate}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?k_{rate}" title="k_{rate}" /></a>还不一样(因为还有各种阻力和炮管长度等等的影响)，就把<a href="https://www.codecogs.com/eqnedit.php?latex=k_{rate}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?k_{rate}" title="k_{rate}" /></a>和距离<a href="https://www.codecogs.com/eqnedit.php?latex=L" target="_blank"><img src="https://latex.codecogs.com/gif.latex?L" title="L" /></a>做拟合（期间又因为代码问题有正确拟合和错误拟合)，把问题还搞复杂了精度反而变低了，所以虽然我建了模型但模型考虑的太简单，所以还不如不建模型直接试验然后查表来做。以后要吸取教训，难顶哦---
* 机械结构：1、我们炮管没找到合适内径的硬管子，最后用的纸管。。。发射炮弹时比较抖 2、云台被队友踩了一脚，纠正过来后还有一点歪。。。 3、炮管的根部和云台左右转动的轴心相差了1cm，发现时距离封箱还有4、5小时，这是最大的错误，所以咋可能射的准嘛（之前一直调，重复性都很低）
* 这次比赛比较好的是：在单片机上加入了RT-Thread操作系统的内核，向系统中加入功能变得简单多了，写程序更快了，运行的也稳定，调试也方便（第二天软件就基本 写完了）

![俯视图](https://raw.githubusercontent.com/adeansHFUT/electronics_design_competition/master/今年题/照片和视频/俯视图.jpg)

