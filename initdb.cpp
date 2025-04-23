#include "initdb.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QCoreApplication>

bool initDatabase()
{
    QString dbPath = QCoreApplication::applicationDirPath() + "/users.db";
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
                          "password TEXT NOT NULL)";
    if (!query.exec(createTable)) {
        qDebug() << "创建表失败:" << query.lastError().text();
        return false;
    }

    query.exec("INSERT OR IGNORE INTO users (username, password) VALUES ('user1', 'pass1')");
    query.exec("INSERT OR IGNORE INTO users (username, password) VALUES ('user2', 'pass2')");
    query.exec("INSERT OR IGNORE INTO users (username, password) VALUES ('user3', 'pass3')");

    db.close();
    // 不要 removeDatabase，否则登录窗口就无法用这个连接
    return true;
}
