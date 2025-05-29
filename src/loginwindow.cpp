// loginwindow.cpp

#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "utils.h"               // 引入computeHash函数
#include "mainwindow.h"
#include "enrollwindow.h"  // 添加这一行（如果还没加）

#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

// 静态成员初始化
QMap<QString, MainWindow*> LoginWindow::loggedInUsers;

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("登录界面");
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名和密码不能为空！");
        return;
    }

    // 检查用户是否已经登录
    if (loggedInUsers.contains(username)) {
        QMessageBox::information(this, "提示", "该用户已登录，已激活对应窗口。");
        MainWindow* existWindow = loggedInUsers.value(username);
        if (existWindow) {
            existWindow->raise();
            existWindow->activateWindow();
        }
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("initConnection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "错误", "数据库未连接！");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT salt, password_hash FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        QString salt = query.value("salt").toString();
        QString correctHash = query.value("password_hash").toString();

        // 调用 utils.h 中的 computeHash 函数
        QString inputHash = computeHash(salt, password);

        if (inputHash == correctHash) {
            MainWindow *mainWindow = new MainWindow(username);
            mainWindow->setAttribute(Qt::WA_DeleteOnClose);

            // 保存已登录窗口
            loggedInUsers.insert(username, mainWindow);

            // 窗口关闭时移除用户
            connect(mainWindow, &MainWindow::destroyed, [username]() {
                loggedInUsers.remove(username);
            });

            mainWindow->show();

            ui->usernameLineEdit->clear();
            ui->passwordLineEdit->clear();
            return;
        }
    }

    QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
}

void LoginWindow::on_enrollButton_clicked()
{
    EnrollWindow *enroll = new EnrollWindow();
    enroll->show();
    this->close();  // 如果你希望关闭当前登录窗口
}
