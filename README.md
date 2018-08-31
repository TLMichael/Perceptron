# Perceptron

Hard hat detection system by QT5

## 界面预览

![](https://i.imgur.com/QTSsLwo.png)

## 开发计划

类别|功能|状态
---|---|---
首页|引导界面|已完成
实时视频分析|使用摄像头|已完成
视频文件分析|导入视频文件|已完成
信息查询|MySQL数据库查询|已完成


## 安装部署

### Windows-GPU版本

#### 安装Nvidia GPU驱动

- 如果已经安装驱动则跳过这一步
- 访问[Nvidia驱动下载](https://www.nvidia.com/Download/index.aspx?lang=cn)，下载适合自己电脑环境的驱动
- 双击**驱动安装程序**安装
- 重启电脑

#### 解压安全帽检测系统

- 下载压缩包**HardhatSystem-gpu-win-x86_64-Release-2.2.0.rar**并解压

#### 打开安全帽检测系统

- 进入**HardhatSystem**文件夹
- 双击运行**HardhatSystem.exe**
- 等待3秒后，软件界面出现

#### 可能出现的问题

- 软件打开时弹出错误：双击运行**vc_redist.x64.exe**，安装完成后重新运行**HardhatSystem.exe

### Ubuntu-GPU版本

#### 安装Nvidia GPU驱动
- 在终端使用如下命令安装
```shell
    sudo add-apt-repository ppa:graphics-drivers/ppa
    sudo apt update
    sudo apt install nvidia-396
```
- 重启电脑后使用nvidia-smi命令检查是否安装成功

#### 安装安全帽检测系统
- 下载压缩包**HardhatSystem-gpu-linux-x86_64-Release-2.2.0.zip**并解压

#### 打开安全帽检测系统
- 进入目录打开终端执行命令
```shell
    cd <your dir>/HardhatSystem-gpu-linux-x86_64-Release-2.2.0
    ./run.sh
```

### Windows-CPU版本

#### 解压安全帽检测系统

- 下载压缩包**HardhatSystem-cpu-win-x86_64-Release-2.2.0.rar**并解压

#### 打开安全帽检测系统

- 进入**HardhatSystem**文件夹
- 双击运行**HardhatSystem.exe**
- 等待3秒后，软件界面出现

#### 可能出现的问题

- 软件打开时弹出错误：双击运行**vc_redist.x64.exe**，安装完成后重新运行**HardhatSystem.exe

### Ubuntu-CPU版本

#### 安装安全帽检测系统
- 下载压缩包**HardhatSystem-cpu-linux-x86_64-Release-2.2.0.zip**并解压

#### 打开安全帽检测系统
- 进入目录打开终端执行命令
```shell
    cd <your dir>/HardhatSystem-cpu-linux-x86_64-Release-2.2.0
    ./run.sh
```

## 使用说明

### Windows
- 进入HardhatSystem文件夹
- 双击HardhatSystem.exe打开

### Ubuntu
- 解压HardhatSystem-linux-x86_64-Release-2.2.0.zip后进入解压后的文件夹
- 在终端执行以下命令，打开**安全帽检测系统**
```shell
    cd <your work_home>/HardhatSystem-linux-x86_64-Release-2.2.0
    ./run.sh
```
