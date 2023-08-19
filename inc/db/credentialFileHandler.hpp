#ifndef _CREDENTIALFILEHANDLER_HPP_
#define _CREDENTIALFILEHANDLER_HPP_
#include "fileBase.hpp"
class CredentialFileHandler : public fileBase {
    public:
    // CredentialFileHandler(const std::string& filename) : filename(filename) {}
        void setFilename(const std::string& filename_) {this->filename = filename_;}

        bool checkExits_name(std::string& username) {
            if (this->OpenFileForReading()) {
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
            } else {
                return false;
            }
        }

        bool username_password_write(std::string& username, std::string& password) {
            if (this->OpenFileForReading()) {
                if (this->CheckSameCredentials(username)) {
                    spdlog::warn("username exits! writing failed!");
                } else {
                    this->CloseFile();
                    if (this->OpenFileForWriting()) {
                        if (this->WriteCredentials(username, password)) {
                            spdlog::info("new username_password saved");
                            this->CloseFile();
                            return true;
                        }
                    } else {
                        spdlog::warn("username_password_write error!");
                    }
                }
            } else {
                this->CloseFile();
                spdlog::warn("username_password_write error!");
                return false;
            }
        }

        bool username_password_find(std::string& username, std::string& password) {
            if (password.find(',') != std::string::npos) password = password.substr(0, password.find(','));
            if (this->OpenFileForReading()) {
                if (this->ReadCredentials(username, password)) {
                    this->CloseFile();
                    return true;
                } else {
                    this->CloseFile();
                    password = "unkonwn";
                    return false;
                }
            } else {
                this->CloseFile();
                spdlog::warn("username_password_find error!");
                password = "unkonwn";
                return false;
            }

        }
};
#endif CREDENTIALFILEHANDLER_HPP_
