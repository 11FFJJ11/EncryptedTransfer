#include "tcpserver.h"
#include <QDebug>
#include <QDir>

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent) {}

void TcpServer::startServer(quint16 port)
{
    listen(QHostAddress::Any, port);
    qDebug() << "服务端已监听端口：" << port;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);

    connect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::receiveData);
}

void TcpServer::receiveData()
{
    static QString fileName;

    if (!fileNameReceived) {
        fileName = QString::fromUtf8(clientSocket->readLine()).trimmed();
        QString fullPath = QDir::homePath() + "/Received_" + fileName;
        receivedFile.setFileName(fullPath);
        if (!receivedFile.open(QIODevice::WriteOnly)) {
            qWarning() << "无法创建文件";
            return;
        }
        fileNameReceived = true;
        return;
    }

    QByteArray data = clientSocket->readAll();
    receivedFile.write(data);

    if (clientSocket->atEnd()) {
        receivedFile.close();
        fileNameReceived = false;
        qDebug() << "接收完成：" << receivedFile.fileName();
    }
}
