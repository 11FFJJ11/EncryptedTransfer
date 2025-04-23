#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class LoginWindow;
}

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
};

#endif // LOGINWINDOW_H

