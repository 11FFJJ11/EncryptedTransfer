#include "filetransferwindow.h"
#include "ui_filetransferwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHostAddress>
#include "mainwindow.h"  // 添加这一行

FileTransferWindow::FileTransferWindow(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileTransferWindow),
    m_mainWindow(mainWindow)  // 将 MainWindow 指针传入
{
    ui->setupUi(this);
}

FileTransferWindow::~FileTransferWindow()
{
    delete ui;
}

void FileTransferWindow::on_browseButton_clicked()
{
    selectedFilePath = QFileDialog::getOpenFileName(this, "选择要发送的文件");
    if (!selectedFilePath.isEmpty()) {
        ui->fileLineEdit->setText(selectedFilePath);
    }
}

void FileTransferWindow::on_sendButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString filePath = ui->fileLineEdit->text().trimmed();

    if (username.isEmpty() || filePath.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写接收用户名并选择文件！");
        return;
    }

    // 简化：直接写死 IP，后期你可以通过用户名查数据库
    QString ip = "127.0.0.1";
    quint16 port = 12345;

    sendFile(filePath, ip, port);
}

void FileTransferWindow::sendFile(const QString &filePath, const QString &ip, quint16 port)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "错误", "无法打开文件！");
        return;
    }

    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "连接失败", "无法连接到接收方！");
        return;
    }

    QByteArray fileData = file.readAll();
    socket->write(fileData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->disconnectFromHost();

    QMessageBox::information(this, "成功", "文件发送成功！");
    file.close();
}


void FileTransferWindow::on_backButton_clicked()
{
    this->hide();  // 隐藏当前窗口

    if (m_mainWindow) {
        m_mainWindow->show();  // 显示主窗口
    }
}

