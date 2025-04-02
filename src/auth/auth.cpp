#include "auth.h"
#include <iostream>

bool authenticateUser(const std::string& username, const std::string& password) {
    std::cout << "Authenticating user: " << username << std::endl;
    return true; // 这里暂时返回成功，后续添加SM3加密验证
}

