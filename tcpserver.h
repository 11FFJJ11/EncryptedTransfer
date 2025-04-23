#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = nullptr);
    void startServer(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void receiveData();

private:
    QTcpSocket *clientSocket;
    QFile receivedFile;
    bool fileNameReceived = false;
};

#endif // TCPSERVER_H
