# 图像质量评价
-----
## 图像颜色丰富度评价 <https://www.pyimagesearch.com/2017/06/05/computing-image-colorfulness-with-opencv-and-python/>
* 首先我们要有一个色彩丰富度的标准。Hasler and Süsstrunk的研究将颜色丰富度划分为7级。
> 无(Not colorful)
> 稍微(Slightly colorful)
> 适度(Moderately colorful)
> 平均(Averagely colorful)
> 非常(Quite colorful)
> 高度(Highly colorful)
> 极端(Extremely colorful)
* [Hasler and Süsstrunk]<https://infoscience.epfl.ch/record/33994/files/HaslerS03.pdf> 找了20个人对84副图片按照1-7分进行打分。最后对这份调查数据进行分析，发现图片颜色丰富度的计算，最后的C就是图片颜色丰富度的指示变量（其中sigma和miu分别代表标准差和平均值）

## 视频清晰度、色偏以及亮度异常检测
### 1.失焦检测
* 失焦的主要表现就是画面模糊，衡量画面模糊的主要方法就是梯度的统计特征，通常梯度值越高，画面的边缘信息越丰富，图像越清晰。需要注意的是梯度信息与每一个视频本身的特点有关系，如果画面中本身的纹理就很少，即使不失焦，梯度统计信息也会很少，对监控设备失焦检测需要人工参与的标定过程，由人告诉计算机某个设备正常情况下的纹理信息是怎样的。
### 2.色偏检测。
* 网上常用的一种方法是将RGB图像转变到CIE L\*a\*b\*空间，其中L\*表示图像亮度，a\*表示图像红/绿分量，b\*表示图像黄/蓝分量。通常存在色偏的图像，在a\*和b\*分量上的均值会偏离原点很远，方差也会偏小；通过计算图像在a\*和b\*分量上的均值和方差，就可评估图像是否存在色偏。计算CIE L\*a\*b\*空间是一个比较繁琐的过程，好在OpenCV提供了现成的函数，因此整个过程也不复杂。
### 3.亮度检测
* 亮度检测与色偏检测相似，计算图片在灰度图上的均值和方差，当存在亮度异常时，均值会偏离均值点（可以假设为128），方差也会偏小；通过计算灰度图的均值和方差，就可评估图像是否存在过曝光或曝光不足。函数如下。

### details to see [视频清晰度、色偏以及亮度异常检测]<http://blog.csdn.net/kklots/article/details/12720359>
