#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QMap>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_loginButton_clicked();
    void on_enrollButton_clicked();

private:
    Ui::LoginWindow *ui;

    // 静态保存当前已登录的用户和窗口映射
    static QMap<QString, MainWindow*> loggedInUsers;
};

#endif // LOGINWINDOW_H
