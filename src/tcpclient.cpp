#include "tcpclient.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
}

void TcpClient::sendFile(const QString &filePath, const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
    if (!socket->waitForConnected(3000)) {
        qWarning() << "连接失败！";
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开文件";
        return;
    }

    QByteArray fileName = QFileInfo(file).fileName().toUtf8();
    socket->write(fileName + "\n");
    socket->waitForBytesWritten();

    while (!file.atEnd()) {
        socket->write(file.read(4096));
        socket->waitForBytesWritten();
    }

    file.close();
    socket->disconnectFromHost();
}
