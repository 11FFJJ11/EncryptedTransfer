#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QCryptographicHash>
#include <QTime>

// inline 生成随机盐
inline QString generateSalt(int length = 8) {
    const QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString salt;
    qsrand(QTime::currentTime().msec());
    for (int i = 0; i < length; ++i) {
        salt += chars.at(qrand() % chars.length());
    }
    return salt;
}

// inline 哈希函数
inline QString computeHash(const QString &salt, const QString &password) {
    QByteArray data = (salt + password).toUtf8();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

#endif // UTILS_H
