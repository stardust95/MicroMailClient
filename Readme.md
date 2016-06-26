# MicroMailClient
邮件客户端(MVC + QT + Material Design)

#主要功能:
- 使用MVC设计模式开发的邮件管理客户端(QT + Material Design, 设计参考Gmail移动客户端)
- 用户可以在客户端内使用常用的邮件系统(通过SMTP/POP3)或我们提供的邮件系统MicroMail
- 可以进行邮件及附件的收发, 查看等

#主要模块:
1. View(视图)(2人):
	- 登陆
	- 注册/密码管理(仅使用MicroMail时)
	- 收件箱
	- 发送邮件
	- 查看邮件
	- 上传, 下载附件
	
2. Controller(控制器)(1人):
	View与Model(即前端和后端)通信部分程序的实现

3. Model(模型)(2人):
	客户端登录及收发邮件, 查看邮件, 附件下载等的程序实现

4. 整体架构及接口, 数据库设计(1人)

5. MicroMail服务器搭建及程序实现(1人)

6. 通用(Utils)模块(1人)

7. 软件开发文档(1人)


#分工:

- 视图+整体架构+服务器部分

- 视图+控制器

- 模型

- 开发文档 + 部分模型(网络部分)

#P.S. 

1. 使用git作为版本管理工具, [教程在这里](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000)

2. 全局使用C++11新特性开发, 使用智能指针(shared_ptr, weak_ptr, unique_ptr, 不包括auto_ptr), 减少甚至避免使用new/delete自己管理内存; 使用static_cast<>而非(Type)作类型转换.

3. 尽量遵循[Google开源项目的C++风格](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents/), 尤其是头文件的使用和禁止全局变量. 

4. 开发环境和框架:Qt Creator + Qt 5.6(MinGW4.9), [下载地址](http://download.qt.io/official_releases/qt/5.6/5.6.0/qt-opensource-windows-x86-android-5.6.0.exe)
安装时勾选Tools/MinGW4.9

5. 网络编程主要使用POCO C++ Library实现, [官方文档](http://pocoproject.org/docs/Poco.Net.MailMessage.html)

6. 前端主要使用qml-material扩展模块实现, [官方文档](http://papyros.io/qml-material/)

	下载地址: box提取码 material

	配置说明: http://www.heilqt.com/topic/56b010459e5a7aac4bc63b42


---------