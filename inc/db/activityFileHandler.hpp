#ifndef _ACTIVITYFILEHANDLER_HPP_
#define _ACTIVITYFILEHANDLER_HPP_
#include "fileBase.hpp"

class activityFileHandler : public fileBase 
{
    public:
        void setFilename_(std::string filename_) { this->filename = filename_; }

        bool activity_query(std::string& name, std::string& back) {
            if (this->OpenFileForReading()) {
                return this->ReadCredentials(name, back);
            } else {
                return false;
            }
        }

        bool activity_set(std::string& name, std::string& back) {
            if (this->OpenFileForWriting()) {
                if (this->WriteCredentials(name, back)) {
                    this->CloseFile();
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
};

#endif _ACTIVITYFILEHANDLER_HPP_