#ifndef HISTORYWINDOW_H
#define HISTORYWINDOW_H

#include <QWidget>

namespace Ui {
class HistoryWindow;
}

class MainWindow;  // 前向声明

class HistoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryWindow(MainWindow *mainWindow, const QString &username, QWidget *parent = nullptr);
    ~HistoryWindow();

    void addHistoryRecord(const QString &record);  // 添加记录
    void loadHistory();                            // 加载历史记录

private slots:
    void on_backButton_clicked();

private:
    Ui::HistoryWindow *ui;
    MainWindow *m_mainWindow;
    QString m_username;
    QString logFilePath;  // 根据用户名动态生成
};

#endif // HISTORYWINDOW_H
