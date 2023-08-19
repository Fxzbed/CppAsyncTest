#ifndef _SEATFILEHANDLER_HPP_
#define _SEATFILEHANDLER_HPP_
#include "fileBase.hpp"
class seatFileHandler : public fileBase
{
public:
    void seatFileHandler_seatNotFixed(std::string filename_) { this->filename = filename_; }
    void seatFileHandler_seatFixed(std::string filename_) { this->filename = filename_; }
    void setSeat_fixed(std::string& seatNum, std::string& name);
    void setSeat_notfixed(std::string& seatNum, std::string& name);
    void querySeat_fixed(std::string& name, std::string& seatNum);
    void querySeat_notfixed(std::string& name, std::string& seatNum);
};

void seatFileHandler::setSeat_fixed(std::string& seatNum, std::string& name) {
    if (this->OpenFileForReading()) {
        if (this->CheckSameCredentials(name)) {
            this->CloseFile();
            if (this->OpenFileForWriting()) {
                if (!this->WriteCredentials(name, seatNum)) {
                    spdlog::warn("file write failed");
                } else {
                    spdlog::info("new seat saved");
                }
            } else {
                spdlog::warn("open file failed");
            }
        } else {
            spdlog::warn("already have one seat");
        }
    } else {
        spdlog::warn("open file failed");
    }
}

void seatFileHandler::setSeat_notfixed(std::string& seatNum, std::string& name) {
    this->setSeat_fixed(seatNum, name);
}

void seatFileHandler::querySeat_fixed(std::string& name, std::string& seatNum) {
    if (this->OpenFileForReading()) {
        if (!this->ReadCredentials(name, seatNum)) {
            seatNum = "unkonwn";
        } 
    } else {
        spdlog::warn("open file failed");
    }
}

void seatFileHandler::querySeat_notfixed(std::string& name, std::string& seatNum) {
    this->querySeat_fixed(name, seatNum);
}

#endif _SEATFILEHANDLER_HPP_