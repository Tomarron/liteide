<!-- Install -->

# 安装 LiteIDE
## 先决条件
使用LiteIDE开发golang需要安装Go语言开发环境。

### Go
先要安装Go语言，详细情况请参看<http://golang.org/doc/install.html>. 
在安装LiteIDE之前一定要先安装Go语言。

## 安装二进制文件
<http://sourceforge.net/projects/liteide/>

### Windows
下载压缩包并解压至c:\盘

### Linux and MacOSX
下载压缩包并解压到/usr/local或者是$HOME

## 从源代码编译
LiteIDE源码位于<https://github.com/visualfc/liteide>上。需要使用Qt4/Qt5来编译源代码，Qt库可以从<https://qt-project.org/downloads>上获取。Mac OS X用户可以不从源代码编译Qt，直接在终端中运行`brew update && brew install qt`，节省大量时间。下面的编译以Qt4为例。

### Windows
	> git clone https://github.com/visualfc/liteide.git
	> set QTDIR=c:\Qt\Qt484
	> set MINGWDIR=c:\Qt\MinGW
	> cd liteide/build
	> update_pkg.cmd
	> build_mingw.cmd
	> deploy_qt4.8_webkit.cmd

### Linux
	$ git clone https://github.com/visualfc/liteide.git
	$ export QTDIR=$HOME/QtSDK/Desktop/Qt/484/gcc
	$ cd liteide/build
	$ ./update_pkg.sh
	$ ./build_linux.sh
	$ ./deploy_linux_qt4.8_webkit.sh

### Mac OS X

**Qt 4**

	$ git clone https://github.com/visualfc/liteide.git
	$ export QTDIR=$HOME/QtSDK/Desktop/Qt/484/gcc #如果Qt是通过brew安装，输入: export QTDIR=/usr/local/Cellar/qt/4.8.6
	$ cd liteide/build
	$ ./update_pkg.sh
	$ ./build_osx.sh
	$ ./deploy_osx_qt4.sh
	$ open liteide/LiteIDE.app

**Qt 5 sdk install**

从 http://www.qt.io/download 下载并安装Qt. (Qt5.6.2/Qt5.7.1/Qt5.8)

	$ git clone https://github.com/visualfc/liteide.git
	$ export QTDIR=$HOME/Qt5.6.2/Qt5.6/clang_64
	$ cd liteide/build
	$ ./update_pkg.sh
	$ ./build_osx_clang.sh
	$ ./deploy_osx_qt5.sh
	$ open liteide/LiteIDE.app
	
**Qt 5 brew install**	

使用 brew 安装 Qt. (比如使用 brew install qt. 其他版本如 qt@5.5 qt@5.6 qt@5.7 ).

	$ git clone https://github.com/visualfc/liteide.git
	$ export QTDIR=/usr/local/Cellar/qt/5.8.0_2 # or modify accordingly for qt@5.5 and qt@5.7
	$ cd liteide/build
	$ ./update_pkg.sh
	$ ./build_osx_clang.sh
	$ open liteide/LiteIDE.app

警告！ 使用 brew 安装的 Qt rpath 不正确不要使用 deploy 脚本进行打包。

### OpenBSD
	$ git clone https://github.com/visualfc/liteide.git
	$ export QTDIR=/usr/local/lib/qt4
	$ cd liteide/build
	$ ./update_pkg.sh
	$ ./build_openbsd.sh

	## Run it: ##
	$ export LD_LIBRARY_PATH=$HOME/liteide/build/liteide/bin:$LD_LIBRARY_PATH
	$ cd ~/liteide/build/liteide/bin
	$ ./liteide

### Raspbian Jessie
	$ sudo apt-get update
	$ sudo apt-get install qt5-default xterm
	$ git clone https://github.com/visualfc/liteide.git
	$ cd liteide/build
	$ ./update_pkg.sh
	$ QTDIR=/usr ./build_linux.sh
	The build process from a µSD card takes approx. 105 minutes on a RPi 2.
	
	## Run it: ##
	$ cd ~/liteide/build/liteide/bin
	$ ./liteide


**补充：** 根据自己的环境设置好 `QTDIR` (在Windows上还需要设置`MINGWDIR`)
