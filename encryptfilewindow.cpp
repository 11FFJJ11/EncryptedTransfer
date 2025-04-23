#include "encryptfilewindow.h"
#include "ui_encryptfilewindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QDebug>

EncryptFileWindow::EncryptFileWindow(QWidget *parent, QWidget *mainWindow) :
    QWidget(parent),
    ui(new Ui::EncryptFileWindow),
    m_mainWindow(mainWindow)  // 保存主窗口指针
{
    ui->setupUi(this);
    ui->fileLineEdit->setReadOnly(true);

    // 初始化算法选择框
    ui->algoComboBox->addItem("XOR简单加密");
    ui->algoComboBox->addItem("Base64编码");
    ui->algoComboBox->addItem("SM4加密（占位）");
}

EncryptFileWindow::~EncryptFileWindow()
{
    delete ui;
}

void EncryptFileWindow::on_browseButton_clicked()
{
    selectedFilePath = QFileDialog::getOpenFileName(this, "选择文件");
    if (!selectedFilePath.isEmpty()) {
        ui->fileLineEdit->setText(selectedFilePath);
    }
}

void EncryptFileWindow::on_encryptButton_clicked()
{
    if (selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择文件！");
        return;
    }

    QFile file(selectedFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "错误", "无法打开文件！");
        return;
    }

    QByteArray content = file.readAll();
    file.close();

    QByteArray encrypted;
    QString algo = ui->algoComboBox->currentText();

    if (algo.contains("XOR")) {
        encrypted = encryptWithXOR(content, 0xAA);
    } else if (algo.contains("Base64")) {
        encrypted = encryptWithBase64(content);
    } else if (algo.contains("SM4")) {
        encrypted = encryptWithSM4(content);  // 你可以先返回原始内容
    }

    QString newPath = selectedFilePath + ".enc";
    QFile outFile(newPath);
    if (!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "错误", "写入失败！");
        return;
    }
    outFile.write(encrypted);
    outFile.close();

    QMessageBox::information(this, "完成", "加密成功！保存路径：" + newPath);
}

void EncryptFileWindow::on_backButton_clicked()
{
    this->hide();  // 隐藏加密窗口
    if (m_mainWindow) {
        m_mainWindow->show();  // 显示主窗口
    }
}

// 简单XOR加密
QByteArray EncryptFileWindow::encryptWithXOR(const QByteArray &data, char key)
{
    QByteArray result = data;
    for (int i = 0; i < result.size(); ++i) {
        result[i] = result[i] ^ key;
    }
    return result;
}

// Base64 编码（模拟加密）
QByteArray EncryptFileWindow::encryptWithBase64(const QByteArray &data)
{
    return data.toBase64();
}

// SM4加密占位（后期你可以替换为真正算法）
QByteArray EncryptFileWindow::encryptWithSM4(const QByteArray &data)
{
    QMessageBox::information(this, "提示", "SM4功能尚未实现（需要调用国密库）");
    return data;
}

