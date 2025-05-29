#include "enrollwindow.h"
#include "loginwindow.h"
#include "ui_enrollwindow.h"
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QTime>
#include "utils.h"  // 使用 computeHash

EnrollWindow::EnrollWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnrollWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("用户注册");
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
}

EnrollWindow::~EnrollWindow()
{
    delete ui;
}

void EnrollWindow::on_registerButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写所有字段！");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "提示", "两次输入的密码不一致！");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("initConnection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "错误", "数据库未连接！");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "错误", "查询数据库失败！");
        return;
    }
    if (query.value(0).toInt() > 0) {
        QMessageBox::warning(this, "提示", "用户名已存在，请更换！");
        return;
    }

    QString salt = generateSalt();
    QString hash = computeHash(salt, password);

    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO users (username, salt, password_hash) "
                        "VALUES (:username, :salt, :hash)");
    insertQuery.bindValue(":username", username);
    insertQuery.bindValue(":salt", salt);
    insertQuery.bindValue(":hash", hash);

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "错误", "注册失败：" + insertQuery.lastError().text());
        return;
    }

    QMessageBox::information(this, "成功", "注册成功！");

}


void EnrollWindow::on_returnButton_clicked()
{
    LoginWindow *login = new LoginWindow();
    login->show();
    this->close();
}
