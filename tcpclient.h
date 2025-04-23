#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QObject>

class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent = nullptr);
    void sendFile(const QString &filePath, const QString &host, quint16 port);

private:
    QTcpSocket *socket;
};

#endif // TCPCLIENT_H
