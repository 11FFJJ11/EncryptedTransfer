#ifndef FILETRANSFERWINDOW_H
#define FILETRANSFERWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <QDateTime>
#include "user_port_map.h"
#include "historywindow.h"

class MainWindow;

namespace Ui {
class FileTransferWindow;
}

class FileTransferWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FileTransferWindow(MainWindow *mainWindow, QWidget *parent = nullptr);
    void setUsername(const QString &username) {
        m_username = username;
        this->setWindowTitle(QString("文件加密 - 用户: %1").arg(m_username));
    }
    ~FileTransferWindow();

    void setHistoryWindow(HistoryWindow *historyWin);  // 设置历史窗口

private slots:
    void on_browseButton_clicked();
    void on_sendButton_clicked();
    void on_backButton_clicked();

private:
    Ui::FileTransferWindow *ui;
    MainWindow *m_mainWindow;
    HistoryWindow *historyWindow = nullptr;
    QString m_username;  // 保存当前用户名
    QString selectedFilePath;

    void sendFile(const QString &filePath, const QString &ip, quint16 port, const QString &username);
    quint16 getPortForUser(const QString &username);  // 获取端口，调用 user_port_map.h 的接口或自定义
};

#endif // FILETRANSFERWINDOW_H


