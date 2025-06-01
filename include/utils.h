#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QByteArray>
#include <QTime>
#include <gmssl/sm3.h>

// 生成随机盐
inline QString generateSalt(int length = 8) {
    const QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString salt;
    qsrand(QTime::currentTime().msec());
    for (int i = 0; i < length; ++i) {
        salt += chars.at(qrand() % chars.length());
    }
    return salt;
}

// 使用 GmSSL 的 SM3 哈希函数
inline QString computeHash(const QString &salt, const QString &password) {
    QByteArray data = (salt + password).toUtf8();

    uint8_t digest[SM3_DIGEST_SIZE];
    sm3_digest(reinterpret_cast<const uint8_t *>(data.constData()), data.size(), digest);

    QByteArray hashResult(reinterpret_cast<const char *>(digest), SM3_DIGEST_SIZE);
    return QString(hashResult.toHex());
}

#endif // UTILS_H

