#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QString>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString username, QWidget *parent = nullptr); // 新增参数
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
};
#endif // MAINWINDOW_H
