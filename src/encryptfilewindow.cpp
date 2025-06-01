#include "encryptfilewindow.h"
#include "ui_encryptfilewindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QDebug>
#include <gmssl/sm4.h>   // 这个路径根据 gmssl 的具体实现可能有所不同
#include <cstring> // for memcpy
#include "historywindow.h"
#include "mainwindow.h"
#include <QDateTime>


extern "C" {
#include <gmssl/sm4.h>
}


EncryptFileWindow::EncryptFileWindow(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::EncryptFileWindow),
    m_mainWindow(mainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("文件加/解密");
    ui->fileLineEdit->setReadOnly(true);
       // 连接解密按钮的点击信号和槽函数
    connect(ui->decryptButton, &QPushButton::clicked, this, &EncryptFileWindow::on_decryptButton_clicked);

    // 初始化算法选择框
    ui->algoComboBox->addItem("XOR简单加密");
    ui->algoComboBox->addItem("Base64编码");
    ui->algoComboBox->addItem("SM4加密");
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
        encrypted = encryptWithSM4(content);
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

    // 这里写入历史记录
    if (m_mainWindow && m_mainWindow->getHistoryWindow()) {
        QString record = QString("[%1] 用户 %2 加密文件 %3 使用算法 %4")
                         .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                         .arg(m_mainWindow->getUsername())
                         .arg(newPath)
                         .arg(algo);
        m_mainWindow->getHistoryWindow()->addHistoryRecord(record);
    }
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
    // 密钥和初始向量（IV），长度都应为16字节
    unsigned char key[16] = { '1','2','3','4','5','6','7','8','9','0','a','b','c','d','e','f' };
    unsigned char iv[16]  = { 'a','b','c','d','e','f','1','2','3','4','5','6','7','8','9','0' };

    SM4_KEY sm4_key;

    // 设置加密密钥
    sm4_set_encrypt_key(&sm4_key, key);

    // 数据填充（使用 PKCS#7 填充）
    int padding = 16 - (data.size() % 16);
    QByteArray paddedData = data;
    paddedData.append(QByteArray(padding, (char)padding));

    // 存储加密后的数据
    QByteArray encrypted(paddedData.size(), 0);

    // 使用 CTR 模式进行加密
    sm4_ctr_encrypt(&sm4_key, iv, (unsigned char*)paddedData.constData(), paddedData.size(), (unsigned char*)encrypted.data());

    return encrypted;
}

QByteArray EncryptFileWindow::decryptWithSM4(const QByteArray &data)
{
    unsigned char key[16] = { '1','2','3','4','5','6','7','8','9','0','a','b','c','d','e','f' };
    unsigned char iv[16]  = { 'a','b','c','d','e','f','1','2','3','4','5','6','7','8','9','0' };

    SM4_KEY sm4_key;
    sm4_set_encrypt_key(&sm4_key, key);

    QByteArray decrypted(data.size(), 0);

    sm4_ctr_encrypt(&sm4_key, iv, (unsigned char*)data.constData(), data.size(), (unsigned char*)decrypted.data());

    return decrypted;
}

void EncryptFileWindow::on_decryptButton_clicked()
{
    if (selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要解密的文件！");
        return;
    }

    QFile file(selectedFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "错误", "无法打开文件！");
        return;
    }

    QByteArray content = file.readAll();
    file.close();

    QByteArray decrypted;
    QString algo = ui->algoComboBox->currentText();

    if (algo.contains("SM4")) {
        decrypted = decryptWithSM4(content);
    } else {
        QMessageBox::warning(this, "提示", "当前仅支持SM4算法解密！");
        return;
    }

    QString newPath;
    if (selectedFilePath.endsWith(".enc")) {
        newPath = selectedFilePath.left(selectedFilePath.length() - 4); // 去掉.enc后缀
    } else {
        newPath = selectedFilePath + ".dec";
    }

    QFile outFile(newPath);
    if (!outFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "错误", "写入解密文件失败！");
        return;
    }
    outFile.write(decrypted);
    outFile.close();

    QMessageBox::information(this, "完成", "解密成功！保存路径：" + newPath);

    if (m_mainWindow && m_mainWindow->getHistoryWindow()) {
        QString record = QString("[%1] 用户 %2 解密文件 %3 使用算法 %4")
                         .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                         .arg(m_mainWindow->getUsername())
                         .arg(newPath)
                         .arg(algo);
        m_mainWindow->getHistoryWindow()->addHistoryRecord(record);
    }
}

