#include <QApplication>
#include "loginwindow.h"  // 👈 改成包含登录窗口
#include "initdb.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 初始化数据库
    if (!initDatabase()) {
        qDebug() << "数据库初始化失败！";
        return -1;
    }

    LoginWindow login;  // 👈 实例化登录窗口
    login.show();       // 👈 显示登录窗口

    return a.exec();    // 启动事件循环
}
