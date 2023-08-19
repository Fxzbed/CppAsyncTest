#ifndef _FILEBASE_HPP_
#define _FILEBASE_HPP_
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include <string>
#include <map>

class fileBase 
{
    protected:
        bool OpenFileForWriting() {
            fileStream.open(filename, std::ios::out | std::ios::app); // 追加模式打开文件
            return fileStream.is_open();
        }

        bool WriteCredentials(const std::string& username, const std::string& password) {
            if (IsOpen()) {
                fileStream << this->XOREncrypt(username+','+password) << '\n';
                return true;
            }
            return false;
        }

        bool OpenFileForReading() {
            fileStream.open(filename, std::ios::in);
            return fileStream.is_open();
        }

        bool ReadCredentials(const std::string& username, std::string& password) {
            if (IsOpen()) {
                std::string line;
                while (getline(fileStream, line)) {
                    line = this->XORDecrypt(line);
                    size_t commaPos = line.find(',');
                    if (commaPos != std::string::npos) {
                        std::string storedUsername = line.substr(0, commaPos);
                        std::string storedPassword = line.substr(commaPos + 1);
                        if (storedUsername == username) {
                            password = storedPassword;
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        bool CheckSameCredentials(const std::string& username) {
            if (IsOpen()) {
                std::string line;
                while (getline(fileStream, line)) {
                    line = this->XORDecrypt(line);
                    size_t commaPos = line.find(',');
                    if (commaPos != std::string::npos) {
                        std::string storedUsername = line.substr(0, commaPos);
                        if (storedUsername == username) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        void CloseFile() {
            if (IsOpen()) {
                fileStream.close();
            }
        }

        std::fstream& GetFileStream() {
            return fileStream;
        }

        bool IsOpen() const {
            return fileStream.is_open();
        }

        std::string filename;
        std::fstream fileStream;

        std::string XOREncrypt(const std::string& plaintext, char key = 'K') {
            std::string ciphertext = plaintext;
            
            for (size_t i = 0; i < ciphertext.size(); ++i) {
                    ciphertext[i] = ciphertext[i] ^ key; // XOR 
            }
            return ciphertext;
        }

        std::string XORDecrypt(const std::string& ciphertext, char key = 'K') {
            return XOREncrypt(ciphertext, key); // XOR 
        }

};

#endif _FILEBASE_HPP_