# zOS


zOS是由个人开发的带有实验性质的操作系统，开发过程中参考了川和秀实先生编著的《30天自制操作系统》一书。zOS的宿命可能仅仅停留在我的电脑的虚拟机中，这个项目对我个人来说则是一个宝贵的经历。由于个人工作原因，无法对开发进度每天更新，因此本项目将会在未来不定时更新进度。
欢迎各位操作系统爱好者与我交流：liubingxing@nucosc.com

zOS is an experimental operating system developed by a independent developer.
You can contact with me by email:liubingxing@nucosc.com

## zOS开发日志（Development log）
* 2019.09.18
  * 项目开始，提交了由二进制编写的img文件。
* 2019.10.17
  * 加入了ASCII编码的单个字符显示。
* 2019.10.19
  * 增加了鼠标指针，增加了在屏幕上打印字符串以及变量内容的功能。
* 2019.11.02
  * 将内核文件进行了分割，并在内核指令中加入了中断，为下一步的I/O功能做铺垫
* 2019.11.09
  * 加入了键盘与zOS之间的交互，但是目前只能是通过键盘触发中断。
* 2019.11.12
  * 取消了中断，键值储存在变量中。
* 2019.11.16
  * 加入了缓冲区，为下一步鼠标交互做准备
* 2019.12.18
  * 时隔一个月的又一次更新，加入了鼠标交互，目前已经实现了鼠标键盘与zOS之间的交互，加入了计时器，但是目前还需完善，重写了显卡设置，目前可以显示1024*768的高分辨率，提升了使用效果。zOS这个坑，我会一直把它填完。
