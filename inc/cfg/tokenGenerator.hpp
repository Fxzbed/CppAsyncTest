#ifndef _MSGBUFFER_HPP_
#define _MSGBUFFER_HPP_
#include <iostream>
#include <ctime>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
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

        bool isTokenValid() const {
            auto currentTime = std::chrono::system_clock::now();
            return currentTime < nextRefreshTime + std::chrono::minutes(60);
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

        void generateNewToken() {
            // 获取当前时间戳（分钟级）
            auto timestamp = std::chrono::duration_cast<std::chrono::minutes>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count();
            std::string timestampStr = std::to_string(timestamp);

            const std::string salt = "random_salt";
            std::string data = timestampStr + salt;
            currentToken = sha256(data);
        }

    private:
        std::string currentToken;
        std::chrono::system_clock::time_point nextRefreshTime;



        void calculateNextRefreshTime() {
            nextRefreshTime = std::chrono::system_clock::now() + std::chrono::minutes(1);
        }
};

class TokenGenerator_16
{
    public:
        void generateNewToken() {
            uint32_t time = (uint32_t)std::time(nullptr);
            for (int i = 0; i < 16; i++) {
                currentToken[i] = (uint8_t)(time >> (i * 2));
            }
        }

        bool isTokenValid(const uint8_t token[16]) {
            uint32_t current_time = (uint32_t)std::time(nullptr);
            
            uint32_t token_time = 0;
            for (int i = 0; i < 16; i++) {
                token_time |= ((uint32_t)token[i]) << (i * 2);
            }
            
            uint32_t time_difference = current_time - token_time;
            return time_difference <= 60;
        }

        void getCurrentToken(std::string& str) {
            str = std::string(reinterpret_cast<const char *>(this->currentToken), 16);
        }

    private:
        uint8_t currentToken[16];
};

#endif _MSGBUFFER_HPP_