#ifndef ENCRYPTFILEWINDOW_H
#define ENCRYPTFILEWINDOW_H

#include <QWidget>
#include "historywindow.h"
#include <gmssl/sm4.h>   // 这个路径根据 gmssl 的具体实现可能有所不同
#include "mainwindow.h"

namespace Ui {
class EncryptFileWindow;
}

class MainWindow;  // 前向声明

class EncryptFileWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EncryptFileWindow(QWidget *parent = nullptr, MainWindow *mainWindow = nullptr);
    void setUsername(const QString &username) {
        m_username = username;
        this->setWindowTitle(QString("文件加密 - 用户: %1").arg(m_username));
    }
    ~EncryptFileWindow();

private slots:
    void on_browseButton_clicked();
    void on_encryptButton_clicked();
    void on_backButton_clicked();
        // 新增解密按钮槽函数声明
    void on_decryptButton_clicked();

private:
    Ui::EncryptFileWindow *ui;
    QString m_username;  // 保存当前用户名
    MainWindow *m_mainWindow;  // 保存主窗口指针，用于访问历史窗口和用户名等

    QString selectedFilePath;
    QByteArray decryptWithSM4(const QByteArray &data);
    QByteArray encryptWithXOR(const QByteArray &data, char key);
    QByteArray encryptWithBase64(const QByteArray &data);
    QByteArray encryptWithSM4(const QByteArray &data);
};

#endif // ENCRYPTFILEWINDOW_H
