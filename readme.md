## AutoRobo A

![开元教育无人车](https://www.nooploop.com/wp-content/uploads/2020/11/autorobo-1.jpg)

本例程用于支持Nooploop的AutoRobo A无人车，使用C语言编写，通过STM32CubeMX 生成工程和驱动代码，并在 STM32CubeIDE 下进行开发，可以按照官网下载的用户手册中的详细流程来进行使用。 

AutoRobo A 是一款由Nooploop 空循环自主研发的面向高校与企业的开源教育无人车平台，致力于为开发者提供一个快速上手与验证的多功能开发平台。

用户需要先下载STM32CubeMX和STM32CubeIDE，安装ST-LINK驱动和JAVA的JRE支持包。详细的开发调试下载流程可以参考[AutoRobo A用户手册](http://ftp.nooploop.com/software/products/autorobo/AutoRobo_A_User_Manual_V1.0_zh.pdf)。

版本更新描述：
V1.0.0 初始版本
V1.0.1 修复TOFSense-UART版本兼容问题。
V1.1.0 增加TOFSense-M系列支持，只需一个TOFSense-M系列产品即可完成前方避障功能。参数设置：ID为0，波特率921600，UART查询模式，8*8像素。TOFSense-M安装方式和TOFSense一致，接口朝上安装在前方中间，TOFSense-M S接线朝上进行安装。
