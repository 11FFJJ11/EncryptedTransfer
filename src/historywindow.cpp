#include "historywindow.h"
#include "ui_historywindow.h"
#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>

HistoryWindow::HistoryWindow(MainWindow *mainWindow, const QString &username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWindow),
    m_mainWindow(mainWindow),
    m_username(username)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("历史记录 - 用户: %1").arg(m_username));

    // 根据用户名设置日志文件名
    logFilePath = QString("../data/history_%1.txt").arg(m_username);

    loadHistory();  // 初始化时加载该用户的历史
    ui->historyTextEdit->setReadOnly(true);
}

HistoryWindow::~HistoryWindow()
{
    delete ui;
}

void HistoryWindow::addHistoryRecord(const QString &record)
{
    // 显示到界面
    ui->historyTextEdit->append(record);

    // 写入该用户的日志文件
    QFile file(logFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << record << "\n";
        file.close();
    }
}

void HistoryWindow::loadHistory()
{
    QFile file(logFilePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            ui->historyTextEdit->append(line);
        }
        file.close();
    }
}

void HistoryWindow::on_backButton_clicked()
{
    this->hide();
    if (m_mainWindow)
        m_mainWindow->show();
}
