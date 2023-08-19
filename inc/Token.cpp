#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <iostream>
#include <ctime>
#include <chrono>
#include <string>
#include <sstream>
#include <memory>
#include <iomanip>
#include <algorithm>
#include "spdlog/spdlog.h"
#include <iterator>
#include <thread>
#include <functional>

std::string sha256(const std::string& input) {
    // 使用 C++ 标准库的 SHA-256 哈希函数
    std::array<unsigned char, 32> hash;
    std::hash<std::string> hash_fn;
    hash_fn(input);

    std::ostringstream oss;
    for (const auto& byte : hash) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }

    return oss.str();
}

class TokenGenerator_64 {
public:
    TokenGenerator_64() {
        generateNewToken();
    }

    std::string getToken() const {
        return currentToken;
    }

    void refreshIfNecessary() {
        auto currentTime = std::chrono::system_clock::now();
        if (currentTime >= nextRefreshTime) {
            generateNewToken();
            calculateNextRefreshTime();
        }
    }

    void setToken(const std::string token) {
        currentToken = std::move(token);
    }

private:
    std::string currentToken;
    std::chrono::system_clock::time_point nextRefreshTime;

    void generateNewToken() {
        // 获取当前时间戳（分钟级）
        auto timestamp = std::chrono::duration_cast<std::chrono::minutes>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        // 将时间戳转换为字符串
        std::string timestampStr = std::to_string(timestamp);

        // 生成一个随机的 salt，可以根据需要使用更复杂的方法生成
        const std::string salt = "random_salt";

        // 将时间戳和 salt 合并
        std::string data = timestampStr + salt;

        // 计算 SHA-256 哈希
        currentToken = sha256(data);
    }

    void calculateNextRefreshTime() {
        nextRefreshTime = std::chrono::system_clock::now() + std::chrono::minutes(1);
    }
};

void appendStr(std::string &message, const void *src, std::size_t size) {
    message = message.append(reinterpret_cast<const char *>(src), size);
}

int main() {
    // std::string str("12345");
    // std::cout << str.substr(0, 2) << std::endl;
    // char x[10] = "123456789";
    // std::string a(x, 2);
    // std::cout << a << std::endl; 
    // TokenGenerator_64 tokenGenerator;
    // while (true) {
    //     tokenGenerator.refreshIfNecessary();
    //     std::string token = tokenGenerator.getToken();
    //     std::cout << "Current Token: " << token << std::endl;
    //     spdlog::info("this is a test");
    //     // 等待一段时间，以避免快速循环输出
    //     std::this_thread::sleep_for(std::chrono::seconds(10));
    // }
    short magic = 114;

    std::string str;
    appendStr(str, &magic, 2);
    // uint8_t result;

    auto result = *reinterpret_cast<const uint16_t *>(str.c_str());
    // try {
        // int temp = std::stoi(str);
    //     if (temp >= 0 && temp <= std::numeric_limits<uint8_t>::max()) {
            // result = static_cast<uint8_t>(temp);
    //         std::cout << "Conversion successful: " << result << std::endl;
    //     } else {
    //         std::cerr << "Value out of range for uint8_t." << std::endl;
    //     }
    // } catch (const std::exception& e) {
    //     std::cerr << "Conversion error: " << e.what() << std::endl;
    // }
    std::cout << result << std::endl;
    return 0;
}
