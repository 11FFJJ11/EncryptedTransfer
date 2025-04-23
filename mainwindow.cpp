#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encryptfilewindow.h"
#include "filetransferwindow.h"  // 引入文件发送页面头文件



MainWindow::MainWindow(QString username, QWidget *parent)  // 修改构造函数，接收用户名
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_username(username)  // 初始化成员变量
{
    ui->setupUi(this);

    // 可选：在窗口标题中显示用户名
    setWindowTitle("主页面 - 用户：" + m_username);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 保持原来的功能页面跳转代码不变
void MainWindow::on_encryptFileButton_clicked()
{
    EncryptFileWindow *encryptWindow = new EncryptFileWindow(nullptr, this);  // 传 this 指针
    encryptWindow->show();
    this->hide();
}



void MainWindow::on_sendFileButton_clicked()
{
    // 创建 FileTransferWindow 实例，并传入 MainWindow 指针
    FileTransferWindow *fileTransferWindow = new FileTransferWindow(this);  // 传入 this 作为父窗口

    // 显示文件传送页面
    fileTransferWindow->show();
    this->hide();  // 隐藏主窗口
}


void MainWindow::on_receiveFileButton_clicked()
{

}

void MainWindow::on_viewHistoryButton_clicked()
{

}

void MainWindow::on_exitButton_clicked()
{
   this->close();
}
