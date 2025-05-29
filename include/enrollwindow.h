#ifndef ENROLLWINDOW_H
#define ENROLLWINDOW_H

#include <QWidget>

namespace Ui {
class EnrollWindow;
}

class EnrollWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EnrollWindow(QWidget *parent = nullptr);
    ~EnrollWindow();

private slots:
    void on_registerButton_clicked();

    void on_returnButton_clicked();

private:
    Ui::EnrollWindow *ui;
};

#endif // ENROLLWINDOW_H
