#ifndef RECEIVEFILEWINDOW_H
#define RECEIVEFILEWINDOW_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTcpServer>

#include <QTcpSocket>



class MainWindow;  // 前向声明

namespace Ui {
class receivefilewindow;
}

class receivefilewindow : public QWidget
{
    Q_OBJECT

public:
    // 构造函数，接受 MainWindow 指针，初始化时可以传入主窗口
    explicit receivefilewindow(MainWindow *mainWindow, QWidget *parent = nullptr);
    void setUsername(const QString &username) {
        m_username = username;
        this->setWindowTitle(QString("文件加密 - 用户: %1").arg(m_username));
    }
    ~receivefilewindow();


private slots:
    // 接收文件按钮点击事件
    void on_receiveButton_clicked();

    // 浏览按钮点击事件：选择保存路径
    void on_browseButton_clicked();

    // 返回按钮点击事件：返回主窗口
    void on_backButton_clicked();

private:
    Ui::receivefilewindow *ui;  // UI界面
    QString m_username;  // 保存当前用户名
    MainWindow *m_mainWindow;  // 存储主窗口指针，用于返回主界面
    QTcpServer *server;  // 加入为成员变量
};

#endif // RECEIVEFILEWINDOW_H

