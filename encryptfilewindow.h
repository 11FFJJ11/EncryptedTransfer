#ifndef ENCRYPTFILEWINDOW_H
#define ENCRYPTFILEWINDOW_H

#include <QWidget>

namespace Ui {
class EncryptFileWindow;
}

class EncryptFileWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EncryptFileWindow(QWidget *parent = nullptr, QWidget *mainWindow = nullptr);
    explicit EncryptFileWindow(QWidget *parent = nullptr);
    ~EncryptFileWindow();

private slots:
    void on_browseButton_clicked();

    void on_encryptButton_clicked();

    void on_backButton_clicked();

private:
    Ui::EncryptFileWindow *ui;
    QString selectedFilePath;
    QWidget *m_mainWindow;  // 添加这一行到 EncryptFileWindow 类的 private 部分


    QByteArray encryptWithXOR(const QByteArray &data, char key);
    QByteArray encryptWithBase64(const QByteArray &data);
    QByteArray encryptWithSM4(const QByteArray &data);  //空着，后面集成库再加
};




#endif // ENCRYPTFILEWINDOW_H
