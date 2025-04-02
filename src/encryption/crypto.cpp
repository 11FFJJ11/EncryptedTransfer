#include "crypto.h"
#include <iostream>

std::string encryptData(const std::string& data) {
    std::cout << "Encrypting data..." << std::endl;
    return data; // 这里暂时不加密，后续集成SM4
}

std::string decryptData(const std::string& data) {
    std::cout << "Decrypting data..." << std::endl;
    return data; // 这里暂时不解密，后续集成SM4
}

