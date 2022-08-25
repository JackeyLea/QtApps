# MindViewer

官方说明手册翻译：<a href="https://feater.top/tgam/translation-of-tgam-development-doc">TGAM开发手册翻译</a>

TGAM连接及测试：<a href="https://feater.top/tgam/setup-and-installation-of-tgam-module">神念科技TGAM模块组装测试</a>

本软件开发流程和使用介绍：<a href="https://feater.top/qt/mindviewer-tgam-module-graphic-application">MindViewer-TGAM模块数据图形化软件</a>

神念科技脑波模块TGAM图像化显示工具

配置好TGAM模块，连接好蓝牙，然后打开本软件读取端口数据就可以看见数据显示了。

每秒钟接近600个点，不想使用qcharts（听说性能不是很好），绘图部分使用qwt库。

Qt/qwt6.1.3/qserialport

绘图效果：<img src="https://cdn.feater.top/mindviewer_result.png?imageView2/0/q/75|watermark/2/text/ZmVhdGVyLnRvcA==/font/5a6L5L2T/fontsize/800/fill/I0ZGMDAwMA==/dissolve/100/gravity/SouthWest/dx/10/dy/10" alt="result"></img>

## 说明

- 在官方的API中没有给出眨眼（blink）/走神程度的接口，所以其一直为0.
- 每发511个包之后会发送一个EEG包，如果每10毫秒发送一个包，那么就得5秒才会收到一个eeg包
- 在每个有效数据包末尾添加随机个数的随机数据作为干扰，经过测试没有问题，不知道一个TGAM数据包内会不会有干扰数据，未经测试，如果有，可以提出来
- TGAM模块是每秒发送512个数据包，也就是每2毫秒有一个数据包，经过测试，本程序可以正常显示，但是程序线程会飙到100%，风扇开始转。而且肉眼也跟不上数据，所以在界面上进行了滤波处理