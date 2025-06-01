#include "initdb.h"
#include "utils.h"           // 引入computeHash定义
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QTime>
#include <cstdlib>           // std::rand, std::srand
#include <ctime>             // std::time


bool initDatabase()
{
    // 用系统时间作为随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    QString dbPath = QCoreApplication::applicationDirPath() + "/../data/users.db";
    qDebug() << "数据库路径:" << dbPath;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "initConnection");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "无法打开数据库:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    QString createTable = "CREATE TABLE IF NOT EXISTS users ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "username TEXT NOT NULL UNIQUE, "
                          "salt TEXT NOT NULL, "
                          "password_hash TEXT NOT NULL)";
    if (!query.exec(createTable)) {
        qDebug() << "创建表失败:" << query.lastError().text();
        return false;
    }

    auto insertUser = [&](const QString &username, const QString &password){
        QString salt = generateSalt();
        QString hash = computeHash(salt, password);
        QSqlQuery insertQuery(db);
        insertQuery.prepare("INSERT OR IGNORE INTO users (username, salt, password_hash) VALUES (:username, :salt, :password_hash)");
        insertQuery.bindValue(":username", username);
        insertQuery.bindValue(":salt", salt);
        insertQuery.bindValue(":password_hash", hash);
        if (!insertQuery.exec()) {
            qDebug() << "插入用户失败:" << username << insertQuery.lastError().text();
        }
    };

    db.close();
    return true;
}
