# SkyrimScreenshotFixer
A tool used to fix screenshots of The Elder Scroll V : Skyrim.

##编译说明
同时支持小端模式的架构下的Windows和Linux.
`./skconfig`调整编译信息, 这里可以设置编译选项, 目标平台
`./configure`生成Makefile
`make`编译,生成的文件位于`bin`目录下.

用法就是把要修复的截图作为参数传给它,Windows图形界面下可以直接往上拖,Linux下能不能我没试过.
老滚的截图代码是哪个白痴写的,不知道bmp文件每一行要4字节对齐么.
