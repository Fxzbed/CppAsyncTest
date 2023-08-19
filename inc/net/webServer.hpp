#ifndef _WEBSERVER_HPP_
#define _WEBSERBER_HPP_
#include "tokenGenerator.hpp"
#include "tcpSocket_epollServer.hpp"
#include "fileDatebase.hpp"
#include "threadPool.hpp"
#include "trieNode.hpp"
#include "locker.hpp"
#include <spdlog/logger.h>
#include <spdlog/cfg/env.h>
#include <string>
#include <mutex>
#include <map>
#include <unordered_map>
#include <thread>

#define User 3;
#define Admin 4;
#define Unknown 5

struct dataHead {
    uint16_t magic;
    uint16_t size;
    uint8_t type;
    TokenGenerator_64 token;
};

class webServer
{
    public:
        webServer(/* args */);
        void serverInit(const char* ip = "127.0.0.1", int port = 8888);

        bool isHead(std::string str);
        void __to_str(std::string &message, const void *src, std::size_t size);
        uint16_t str_to_uint16_t(std::string str);
        uint8_t str_to_uint8_t(std::string str);
        void uint16_t__to__str(uint16_t& num, std::string& str);
        void uint8_t__to__str(uint8_t& num, std::string& str);

        void headerBuild(uint8_t type_, uint16_t size_, std::string token, std::string& header);
        // void bodyBuild(std::string& body);

        bool dataDecode(std::string str);

            void ALL_Register(std::string str);
            void ALL_Login(std::string str);
            void ALL_openLock(std::string str);
            void ALL_querySeat(std::string str);
            void ALL_queryTime_rubbish(std::string str);
            void ALL_queryProve_activity(std::string str);
            void ALL_askSeat_notfixed(std::string str);

            void ADMIN_addUser(std::string str);
            void ADMIN_deleteUser(std::string str);
            void ADMIN_resetPassword(std::string str);
            void ADMIN_bindSeat_fixed(std::string str);
            void ADMIN_unbindSeat_fixed(std::string str);
            void ADMIN_allocateSeat_notfixed(std::string str);

        ~webServer();

    private:
        epollServer *webService;
        dataHead headRcv;
        locker lockerService;
        DataMS fileHandler;
        std::mutex globalLock;
        Trie trieNode_token_normaluser;
        Trie TrieNode_token_admin;
        std::unordered_map<std::string, std::string> nameHashMap;
};

// /*  
// *
// *
// */
// void webServer::serverInit(const char* ip = "127.0.0.1", int port = 8888) {
//     this->webService = new(epollServer);
//     if (this->webService != nullptr) {
//         spdlog::info("webServer instantiation successful");
//     } else {
//         spdlog::warn("webServer instantiation failed");
//         return;
//     }
//     //data base init 
//     // this->threadPool_webService = new(threadPool);
//     spdlog::info("server listening...");
//     while (true) {
//         if (this->webService->getRdyret()) {
//             for (int i = 0; i < webService->ret; i ++) {
//                 if (this->webService->epollCtl(i) > 0) {
//                     char buf[1024];
//                     ssize_t bytesRead;
//                     this->webService->epollRead(buf, bytesRead);
//                     std::string str(buf, bytesRead);
//                     if (this->isHead(str)) {
//                         if (this->headRcv.type == 1) {
//                             this->ALL_Login(std::move(str.substr(69, 69 + this->headRcv.size)));
//                         } else {
//                             if (this->headRcv.token.isTokenValid())
//                                 this->dataDecode(std::move(str.substr(69, 69 + this->headRcv.size)));
//                             else {
//                                 spdlog::warn("token error!");
//                             }
//                         }
//                     } else {
//                         spdlog::warn("head data error from Addr: " + this->webService->getClientAddr() + ':' + this->webService->getClientPort());
//                         // spdlog::warn("wrong token from Addr :" + this->webService->getClientAddr() + ':' + this->webService->getClientPort());
//                         continue;
//                     }
//                     continue; 
//                 } 
//             }
//         }
//     }
// }

// void webServer::__to_str(std::string &message, const void *src, std::size_t size) {
//     message = message.append(reinterpret_cast<const char *>(src), size);
// }

// uint16_t webServer::str_to_uint16_t(std::string str) {
//     return *reinterpret_cast<const uint16_t *>(str.c_str());
// }

// uint8_t webServer::str_to_uint8_t(std::string str) {
//     return *reinterpret_cast<const uint8_t *>(str.c_str());
// }

// void webServer::uint16_t__to__str(uint16_t& num, std::string& str) {
//     str = str.append(reinterpret_cast<const char*>(num), sizeof(num));
// }

// void webServer::uint8_t__to__str(uint8_t& num, std::string& str) {
//     str = str.append(reinterpret_cast<const char*>(num), sizeof(num));
// }

// bool webServer::isHead(std::string str) {
//     headRcv.magic = this->str_to_uint16_t(std::move(str.substr(0, 2)));
//     if (headRcv.magic != 114) return false;
//     headRcv.magic = this->str_to_uint16_t(std::move(str.substr(2, 4)));
//     headRcv.type = this->str_to_uint8_t(std::move(str.substr(4, 5)));
//     headRcv.token.setToken(std::move(str.substr(5, 69)));
//             // if (str.substr(0, 2) == 114)
// }

// void webServer::headerBuild(uint8_t type_, uint16_t size_, std::string token, std::string& header) {
//     header = header.append(reinterpret_cast<const char *>(114), sizeof(short));
//     header = header.append(reinterpret_cast<const char *>(size_), sizeof(uint16_t));
//     header = header.append(reinterpret_cast<const char *>(type_), sizeof(uint8_t));
//     header += token;
// }

// // void webServer::bodyBuild(std::string& body) {
// // }

// void webServer::ALL_Login(std::string str) {
//     auto usernameSize = this->str_to_uint16_t(std::move(str.substr(0, 2)));
//     auto username = str.substr(2, 2 + usernameSize);
//     auto passwordSize = this->str_to_uint16_t(std::move(str.substr(2 + usernameSize, 2 + usernameSize + 2)));
//     auto password = str.substr(2 + usernameSize + 2, 2 + usernameSize + 2 + passwordSize);
//     uint8_t typeUser = Unknown;
//     typeUser = (this->fileHandler.username_password_find_ADMIN(username, password)) ? 4 : 5;
//     if (typeUser == Unknown) {
//         typeUser = (this->fileHandler.username_password_find_USER(username, password)) ? 3 : 5;
//     } 
//     std::string respond;
//     std::string body;
//     std::string name;

//     bool result = false;

//     if (typeUser == 5) {
//         result = false;     
//     } else if (typeUser == 3 || typeUser == 4) {
//         name = password.substr(password.find(',') + 1, password.size());
//         result = true;
//         this->headRcv.token.generateNewToken();
//         this->nameHashMap.emplace(this->headRcv.token.getToken(), name);
//         this->headerBuild(14, 65, this->headRcv.token.getToken(), respond);
//         if (typeUser == 3) this->trieNode_token_normaluser.insert(headRcv.token.getToken());
//         else  this->TrieNode_token_admin.insert(headRcv.token.getToken());
//     }
//     respond.append(reinterpret_cast<const char *>(result), sizeof(result));
//     this->webService->epollSend(respond);
// }

// void webServer::ALL_openLock(std::string str)
// {   
//     auto openLock = *reinterpret_cast<const bool *>(str.c_str());
//     std::string name = this->nameHashMap[this->headRcv.token.getToken()];
//     this->lockerService.buildLockerHeader(str, 1, (2 + name.size()));
//     uint16_t nameSize = name.size();
//     str += str.append(reinterpret_cast<const char *>(nameSize), sizeof(uint16_t));
//     str += name;
//     this->lockerService.tcpConnect();
//     this->lockerService.sendLocker(str);
//     str.clear();
//     this->lockerService.recvLocker(str, 22);
//     uint16_t& size = nameSize;
//     size = str_to_uint16_t(std::move(str.substr(2, 4)));
//     uint8_t type = str_to_uint8_t(std::move(str.substr(4, 5)));
//     std::string respond;
//     bool doorStatus = false;
//     if (this->lockerService.isTokenValid(str.substr(5, 22))) {
//         doorStatus = true;
//     } 
//     this->headerBuild(15, 1, std::string(64, '0'), respond);
//     respond += respond.append(reinterpret_cast<const char *>(doorStatus), sizeof(bool));
//     this->webService->epollSend(respond);
//     // this->lockerService.recvLocker();
// }

// void webServer::ALL_Register(std::string str) {
//     if (this->TrieNode_token_admin.search(headRcv.token.getToken())) {
//         auto usernameSize = this->str_to_uint16_t(std::move(str.substr(0, 2)));
//         auto username = str.substr(2, 2 + usernameSize);
//         auto passwordSize = this->str_to_uint16_t(std::move(str.substr(2 + usernameSize, 2 + 2 + usernameSize)));
//         auto password = str.substr(2 + 2 + usernameSize, 2 + 2 + usernameSize + passwordSize);
//         auto userType = this->str_to_uint8_t(std::move(str.substr(2 + 2 + usernameSize + passwordSize, 2 + 2 + usernameSize + passwordSize + 1)));
//         auto nameSize = this->str_to_uint16_t(std::move(str.substr(2 + 2 + usernameSize + passwordSize + 1, 2 + 2 + usernameSize + passwordSize + 1 + 2)));
//         auto name = str.substr(2 + 2 + usernameSize + passwordSize + 1 + 2, 2 + 2 + usernameSize + passwordSize + 1 + 2 + nameSize);
//         std::string respond;
//         password = password + ',' + name;
//         bool returnStatus = false;
//         if (userType == 3) {
//             if (!this->fileHandler.username_password_check_USER(username)) {
//                 if (this->fileHandler.username_password_write_USER(username, password)) {
//                     goto successRespond;
//                 }
//             }
//         } else if (userType == 4) {
//             if (!this->fileHandler.username_password_check_ADMIN(username)) {
//                 if (this->fileHandler.username_password_write_ADMIN(username, password)) {
//                     goto successRespond;
//                 }
//             }
//         }

//         failRespond:
//             respond.append(reinterpret_cast<const char *>(returnStatus), sizeof(bool));
//             headerBuild(13, 1, std::string(64, '0'), respond);
//             this->webService->epollSend(respond);
//             return;

//         successRespond:
//             respond.append(reinterpret_cast<const char *>(returnStatus), sizeof(bool));
//             headerBuild(13, 1, std::string(64, '0'), respond);
//             this->webService->epollSend(respond);
//             return;

//     } else {
//         spdlog::warn("only admin can register user!");
//     }
// }

//  void webServer::ALL_querySeat(std::string str) {
//     str = str.substr(this->str_to_uint16_t(str.substr(0, 2)), str.size());
//     std::string seatNum;
//     bool isQuery = true;
//     uint8_t seatStatus = 0;
//     if (this->fileHandler.seats_fixed_query_USER(str, seatNum)) seatStatus = 1;
//     this->headerBuild(16, 10, std::string(64, '0'), str);
   
//     str += str.append(reinterpret_cast<const char *>(isQuery), sizeof(bool));
//     str += str.append(reinterpret_cast<const char *>(seatStatus), sizeof(uint8_t));
//     str += seatNum;

//     this->webService->epollSend(str);
//  }

//  inline void webServer::ALL_queryTime_rubbish(std::string str)  //............
//  {
//     str = str.substr(2, this->str_to_uint16_t(str.substr(0, 2)));
//     this->fileHandler;
    
//  }

//  inline void webServer::ALL_queryProve_activity(std::string str)
//  {
//     str = str.substr(2, this->str_to_uint16_t(str.substr(0, 2)));
//     std::string output;
//     this->fileHandler.activity_query_USER(str, output);
//     bool isQuery = true;
//     uint8_t activityNumber = this->str_to_uint8_t(output.substr(0, 1));
//     uint16_t activityAddrSize[activityNumber];
//     int i = 1;
//     for (uint16_t& each : activityAddrSize) {
//         each = str_to_uint16_t(output.substr(i, activityNumber * 2 + i));
//         i = activityNumber * 2 + i;
//     }
//     this->headerBuild(18, 2+(activityNumber * 2), std::string(64, '0'), str);
//     str = str.append(reinterpret_cast<const char *>(isQuery), sizeof(bool));
//     str = str.append(reinterpret_cast<const char *>(activityNumber), sizeof(uint8_t));
//     str = str.append(reinterpret_cast<const char *>(activityAddrSize), sizeof(activityAddrSize));
//     this->webService->epollSend(str);
//  }

//  inline void webServer::ALL_askSeat_notfixed(std::string str)
//  {
    
//  }

//  inline void webServer::ADMIN_addUser(std::string str)
//  {
//     auto usernameSize = this->str_to_uint16_t(str.substr(0, 2));
//     auto userName = str.substr(2, 2 + usernameSize);
//     auto passwordSize = this->str_to_uint16_t(str.substr(2 + usernameSize, 2 + usernameSize + 2));
//     auto password = str.substr(2 + usernameSize + 2, 2 + usernameSize + 2 + passwordSize);
//     auto userType = this->str_to_uint8_t(str.substr(2 + usernameSize + 2 + passwordSize, 2 + usernameSize + 2 + passwordSize + 1));
//     auto nameSize = this->str_to_uint16_t(str.substr(2 + usernameSize + 2 + passwordSize + 1, 2 + usernameSize + 2 + passwordSize + 1 + 2));
//     auto name = str.substr(2 + usernameSize + 2 + passwordSize + 1 + 2, 2 + usernameSize + 2 + passwordSize + 1 + 2 + nameSize);

//     password = password + ',' + name;
//     bool isSuccess = false;
//     if (this->fileHandler.username_password_write_ADMIN(userName, password)) isSuccess = true;
//     std::string& respond = name;
//     this->headerBuild(19, 1, std::string(64, 0), respond);
//     respond = respond.append(reinterpret_cast<const char *>(isSuccess), sizeof(bool));
//     this->webService->epollSend(respond);
//  }

//  inline void webServer::ADMIN_deleteUser(std::string str)
//  {
//     uint16_t usernameSize = this->str_to_uint16_t(str.substr(0, 2));
//     std::string username = str.substr(2, usernameSize + 2);
//     bool isDeleteUser = false;
//     std::string respond;
//     if (this->fileHandler.username_password_delete_ADMIN(str)) isDeleteUser = true;
//     this->headerBuild(20, 1, std::string(64, '0'), respond);
//     respond = respond.append(reinterpret_cast<const char *>(isDeleteUser), sizeof(bool)); 
//     this->webService->epollSend(respond);
//  }

//  inline void webServer::ADMIN_resetPassword(std::string str)
//  {
//     uint16_t usernameSize = this->str_to_uint16_t(str.substr(0, 2));
//     std::string username = str.substr(2, str.size());
//     bool isResetPassword = false;
//     if (this->fileHandler.username_password_reset_ADMIN(username)) isResetPassword = true;
//     username.clear();
//     std::string& respond = username;
//     this->headerBuild(21, 1, std::string(64, '0'), respond);
//     respond = respond.append(reinterpret_cast<const char *>(isResetPassword), sizeof(bool));
//     this->webService->epollSend(respond);
//  }

//  inline void webServer::ADMIN_bindSeat_fixed(std::string str)
//  {
//     uint16_t usernameSize = this->str_to_uint16_t(str.substr(0, 2));
//     std::string username = str.substr(2, 2 + usernameSize);
//     uint64_t seatNumber = *reinterpret_cast<const uint64_t *>((str.substr(2 + usernameSize, 2 + usernameSize + 8)).c_str());
//     std::string seatNumber_;
//     bool isBindSeat = false;
//     if (this->fileHandler.seats_fixed_set_ADMIN(username, seatNumber_)) isBindSeat = true;
//     username.clear();
//     this->headerBuild(22, 1, std::string(64, '0'), username); 
//     username = username.append(reinterpret_cast<const char *>(isBindSeat), sizeof(bool));
//     this->webService->epollSend(username);
//  }

//  inline void webServer::ADMIN_unbindSeat_fixed(std::string str)
//  {
//     uint16_t usernameSize = this->str_to_uint16_t(str.substr(0, 2));
//     std::string username = str.substr(2, 2 + usernameSize);
//     str = str.substr(2 + usernameSize, str.size());
//     bool isUnbindSeat = false;
//     if (this->fileHandler.seats_fixed_unbind_ADMIN(username, str)) isUnbindSeat = true; 
//     str.clear();
//     this->headerBuild(23, 1, std::string(64, '0'), str);
//     str = str.append(reinterpret_cast<const char *>(isUnbindSeat), sizeof(bool));
//  }

// //  inline void webServer::ADMIN_unbindSeat_fixed(std::string str)
// //  {

// //  }

// bool webServer::dataDecode(std::string str) {
    
//     switch (this->headRcv.type)
//         {
//             case 1:
//                 this->ALL_Register(str);
//                 break;
//             case 3:
//                 this->ALL_openLock(str);
//                 break;
//             case 4:
//                 this->ALL_querySeat(str);
//                 break;
//             case 5:
//                 this->ALL_queryTime_rubbish(str);
//                 break;
//             case 6:
//                 this->ALL_askSeat_notfixed(str);
//                 break;
//             case 7:
//                 this->ADMIN_addUser(str);
//                 break;
//             case 8:
//                 this->ADMIN_deleteUser(str);
//                 break;
//             case 9:
//                 this->ADMIN_resetPassword(str);
//                 break;
//             case 10:
//                 this->ADMIN_bindSeat_fixed(str);
//                 break;
//             case 11:
//                 this->ADMIN_unbindSeat_fixed(str);
//                 break;
//             case 12:
//                 this->ALL_askSeat_notfixed(str);
//                 /* code */
//                 break;
                
//             default:
//                 spdlog::warn("type wrong! Addr: " + this->webService->getClientAddr() + ':' + this->webService->getClientPort());
//             break;
//         }
// }




#endif _WEBSERVER_HPP_