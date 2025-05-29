#ifndef USER_PORT_MAP_H
#define USER_PORT_MAP_H

#include <QString>

inline quint16 getPortForUser(const QString &username) {
    if (username == "user1") return 12345;
    if (username == "user2") return 12346;
    if (username == "user3") return 12347;
    return 0; // 未知用户
}

#endif // USER_PORT_MAP_H
