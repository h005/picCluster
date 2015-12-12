运行程序，点击open（快捷键O）
打开待聚类图像的mask文件（注意是多个文件，不是一个）
mask文件背景为黑色
例如
mask文件目录为：
path = E:\ViewPoint\kxm\201511231826\proj
则程序会在../../path 下读取真实的图片

这个时候在聚类，时间有点久，稍等一下

点击save（快捷键S）
会在../path下生成聚类之后的文件夹
该文件加以时间加NUMCluster命名
例如：201512121057_50
在该文件加下有NUMCluster个文件夹
分别存放着对应图像文件的快捷方式

PS：在imglabel.cpp中ImgLabel的构造函数中
修改聚类数量
