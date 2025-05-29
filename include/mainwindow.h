#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QString>
#include <QMainWindow>
#include "historywindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString username, QWidget *parent = nullptr); // 新增参数
    HistoryWindow* getHistoryWindow() const { return historyWindow; }  // 提供访问接口
    QString getUsername() const { return m_username; }

    ~MainWindow();

private slots:
    void on_encryptFileButton_clicked();

    void on_sendFileButton_clicked();

    void on_receiveFileButton_clicked();

    void on_viewHistoryButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::MainWindow *ui;
    QString m_username; // 保存用户名，用于后续展示或权限控制
    QString currentUsername;  // 当前登录的用户名
    HistoryWindow *historyWindow = nullptr;  // 声明成员变量
    
};
#endif // MAINWINDOW_H
