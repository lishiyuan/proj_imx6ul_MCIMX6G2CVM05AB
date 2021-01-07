

# 板卡跑自己的qt应用步骤：



<font color = #FF0000B>备注：</font>

<font color = #FF0000B>[QT应用程序]名字为：</font>

<font color = #FF0000B>单键盘界面：weigh_kbdtest</font>

<font color = #FF0000B>全部完整界面：demo2</font>



## 1.1 拷贝qt程序到目录/forlinx/qtbin/lsy_test_qt/weigh_kbdtest下

    cd 【QT桌面应用目录】
    
    mkdir /forlinx/qtbin/lsy_test_qt
    cp  weigh_kbdtest  /forlinx/qtbin/lsy_test_qt/weigh_kbdtest
    
    ------------------------分割线--------------------




## 1.2 修改/etc/matchbox/session文件

<font color = #FF0000B>备注：直接将该目录下session文件（该session是修改完成的session）替换板卡中的session即可，然后下面步骤就可以跳过了</font>

    cd /etc/matchbox
    vi session

将session文件做如下修改：

    注释掉：下面两行，表示注释掉原出厂qt桌面
    # matchbox-desktop &
    # matchbox-panel --titlebar --start-applets $START_APPLETS --end-applets $END_APPLETS &
    
    增加下面2行，表示显示自己的qt桌面应用：
    export DISPLAY=:0.0
    ./forlinx/qtbin/lsy_test_qt/weigh_kbdtest


保存退出，重启板卡




