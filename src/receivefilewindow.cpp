#include "receivefilewindow.h"
#include "ui_receivefilewindow.h"
#include <QMessageBox>
#include <QFile>
#include <QHostAddress>
#include <QFileDialog>
#include "mainwindow.h"
#include "user_port_map.h"
#include "historywindow.h"
#include <QDateTime>


receivefilewindow::receivefilewindow(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::receivefilewindow),
    m_mainWindow(mainWindow),
    server(nullptr)  // 初始化 server 指针
{
    ui->setupUi(this);
    this->setWindowTitle("接收文件");
}

receivefilewindow::~receivefilewindow()
{
    if (server) {
        server->close();
        server->deleteLater();
    }
    delete ui;
}

void receivefilewindow::on_receiveButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();  // 获取用户名
    QString savePath = ui->fileLineEdit->text().trimmed();

    if (username.isEmpty() || savePath.isEmpty()) {
        QMessageBox::warning(this, tr("错误"), tr("用户名和保存路径不能为空！"));
        return;
    }

    quint16 port = getPortForUser(username);
    if (port == 0) {
        QMessageBox::warning(this, tr("错误"), tr("未知用户名，无法监听端口！"));
        return;
    }

    // 防止重复监听
    if (server) {
        if (server->isListening()) {
            QMessageBox::information(this, tr("提示"), tr("已经在监听中，请勿重复点击。"));
            return;
        } else {
            server->deleteLater();
            server = nullptr;
        }
    }

    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, [=]() {
        QTcpSocket *client = server->nextPendingConnection();
        connect(client, &QTcpSocket::readyRead, this, [=]() {
            QByteArray data = client->readAll();

            int splitIndex = data.indexOf("|||");
            if (splitIndex == -1) {
                QMessageBox::warning(this, "错误", "接收数据格式错误！");
                client->disconnectFromHost();
                return;
            }

            QString fileName = QString::fromUtf8(data.left(splitIndex));
            QByteArray fileContent = data.mid(splitIndex + 3);

            QFile file(savePath + "/" + fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::warning(this, tr("错误"), tr("无法保存文件！"));
                return;
            }

            file.write(fileContent);
            file.close();
            QMessageBox::information(this, tr("成功"), tr("文件接收成功，已保存为 %1").arg(fileName));
            client->disconnectFromHost();
            // 接收成功后调用：
            if (m_mainWindow && m_mainWindow->getHistoryWindow()) {
                QString record = QString("[%1] 接收文件 %2 成功")
                                 .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                                 .arg(fileName);
                m_mainWindow->getHistoryWindow()->addHistoryRecord(record);
            }

        });
    });



    if (!server->listen(QHostAddress::Any, port)) {
        QMessageBox::critical(this, "错误", QString("监听端口 %1 失败！").arg(port));
        delete server;
        server = nullptr;
        return;
    }

    QMessageBox::information(this, "提示", QString("用户 %1 正在监听端口 %2，等待接收文件...").arg(username).arg(port));
}



void receivefilewindow::on_browseButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("选择保存路径"), QDir::homePath());
    if (!directory.isEmpty()) {
        ui->fileLineEdit->setText(directory);
    }
}

void receivefilewindow::on_backButton_clicked()
{
    this->hide();
    if (m_mainWindow) {
        m_mainWindow->show();
    }
}
