# proj_imx6ul_MCIMX6G2CVM05AB
芯片为imx6ul系列，具体CPU型号为：MCIMX6G2CVM05AB



<br>

---

<br>



# 一、初始版本-20201118【keyBoard】

具体功能如下：

1. 设备树中进行网卡移植，进行以太网口适配，默认ENET2修改为ENET1。

2. 设备树中添加矩阵键盘对应GPIO配置（4输出默认低电平，8输入）。

3. 【keyBoard文件夹】：

   * key_app.c：

     初始版本，代码未进行优化，不含硬件PCB锁存器与缓冲器器件适配，故还需再加上锁存器与缓冲器配置。

   * Makefile：

     用于编译key_app.c生成key_app，板卡命令行执行命令【./key_app &】，按键驱动就可以正确在后台运行。后续会将该app在内核中适配为随系统启动。

4. 【test_shell文件夹】：

   * 文件夹中sh文件为项目测试shell命令代码，跟项目功能交付无关。



<br>

---

<br>









