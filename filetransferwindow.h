#ifndef FILETRANSFERWINDOW_H
#define FILETRANSFERWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>

namespace Ui {
class FileTransferWindow;
}

class MainWindow;  // 前向声明

class FileTransferWindow : public QWidget
{
    Q_OBJECT

public:
    // 构造函数，接受 MainWindow 指针，初始化时可以传入主窗口
    explicit FileTransferWindow(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~FileTransferWindow();

private slots:
    // 浏览按钮点击事件：选择文件
    void on_browseButton_clicked();

    // 发送文件按钮点击事件
    void on_sendButton_clicked();

    // 返回按钮点击事件：返回主窗口
    void on_backButton_clicked();

private:
    Ui::FileTransferWindow *ui;  // UI界面
    QString selectedFilePath;    // 存储选中的文件路径
    void sendFile(const QString &filePath, const QString &ip, quint16 port);  // 文件发送函数
    MainWindow *m_mainWindow;  // 存储主窗口指针，用于返回主界面
};

#endif // FILETRANSFERWINDOW_H

