#include "mainwindow.h"
#include "filetransferwindow.h"
#include "ui_filetransferwindow.h"
#include "user_port_map.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHostAddress>
#include <QDateTime>
#include "historywindow.h"



FileTransferWindow::FileTransferWindow(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileTransferWindow),
    m_mainWindow(mainWindow),
    historyWindow(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("文件发送");
}

FileTransferWindow::~FileTransferWindow()
{
    delete ui;
}

void FileTransferWindow::setHistoryWindow(HistoryWindow *historyWin)
{
    historyWindow = historyWin;
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

    quint16 port = getPortForUser(username);
    if (port == 0) {
        QMessageBox::warning(this, "错误", "未知用户名！");
        return;
    }

    QString ip = "127.0.0.1";  // 本地测试用固定IP
    sendFile(filePath, ip, port, username);
}

void FileTransferWindow::sendFile(const QString &filePath, const QString &ip, quint16 port, const QString &username)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "错误", "无法打开文件！");
        return;
    }

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();

    QByteArray nameBytes = fileName.toUtf8();
    QByteArray fileData = file.readAll();
    QByteArray allData = nameBytes + "|||" + fileData;

    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "连接失败", "无法连接到接收方！");
        socket->deleteLater();
        return;
    }

    socket->write(allData);
    socket->flush();
    socket->waitForBytesWritten();
    socket->disconnectFromHost();
    socket->deleteLater();

    file.close();

    // 发送成功后调用：
    // 假设你在 FileTransferWindow.cpp 中的某处发送成功后：
    if (m_mainWindow && m_mainWindow->getHistoryWindow()) {
        QString record = QString("[%1] 发送文件给 %2：%3")
                         .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                         .arg(username)  // 接收者用户名
                         .arg(fileName);         // 文件名
        m_mainWindow->getHistoryWindow()->addHistoryRecord(record);
    }


    QMessageBox::information(this, "成功", "文件发送成功！");
}

void FileTransferWindow::on_backButton_clicked()
{
    this->hide();
    if (m_mainWindow) {
        m_mainWindow->show();
    }
}

quint16 FileTransferWindow::getPortForUser(const QString &username)
{
    return ::getPortForUser(username);
}
