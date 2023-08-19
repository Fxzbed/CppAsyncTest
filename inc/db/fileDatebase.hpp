#ifndef _FILEDATABASE_HPP_
#define _FILEDATABASE_HPP_
#include "credentialFileHandler.hpp"
#include "activityFileHandler.hpp"
#include "seatFileHandler.hpp"
#include <mutex>

class DataMS
{
private:
    std::mutex locker_DMS;
    const std::string credentials_PATH = "admin.txt";
    const std::string credentials_PATH = "credentials.txt";
    const std::string seat_fixed_PATH = "seat.txt";
    const std::string activity_PATH = "activity.txt";
    const std::string rubbish_PATH = "rubbish.txt";
    const std::string seat_notfixed_PATH = "seat_notfixed";
    CredentialFileHandler fileHandler_credentials;
    seatFileHandler fileHandler_seats;
    activityFileHandler fileHandler_activity;
    /* data */
public:
    bool username_password_write_USER(std::string& name, std::string& password) {
        fileHandler_credentials.setFilename(credentials_PATH);
        return fileHandler_credentials.username_password_write(name, password); 
    }

    bool username_password_find_USER(std::string& name, std::string& password) {
        fileHandler_credentials.setFilename(credentials_PATH);
        return fileHandler_credentials.username_password_find(name, password);
    }

    bool username_password_check_USER(std::string& name) {
        fileHandler_credentials.setFilename(credentials_PATH);
        return fileHandler_credentials.checkExits_name(name);
    }

    bool username_password_write_ADMIN(std::string& name, std::string& password) {
        fileHandler_credentials.setFilename(credentials_PATH);
        return fileHandler_credentials.username_password_write(name, password); 
    }

    bool username_password_find_ADMIN(std::string& name, std::string& password) {
        fileHandler_credentials.setFilename(credentials_PATH);
        return fileHandler_credentials.username_password_find(name, password);
    }

    bool username_password_check_ADMIN(std::string& name) {
        fileHandler_credentials.setFilename(credentials_PATH);
        return fileHandler_credentials.checkExits_name(name);
    }

    bool seats_fixed_set_ADMIN(std::string& name, std::string& seatNum) {
        fileHandler_seats.seatFileHandler_seatFixed(seat_fixed_PATH);
        fileHandler_seats.setSeat_fixed(seatNum, name);
    }

    bool seats_notfixed_set_ADMIN(std::string& name, std::string& seatNum) {
        fileHandler_seats.seatFileHandler_seatNotFixed(seat_notfixed_PATH);
        fileHandler_seats.setSeat_notfixed(seatNum, name);
    }

    bool seats_fixed_query_USER(std::string& name, std::string& seatNum) {
        fileHandler_seats.seatFileHandler_seatFixed(seat_fixed_PATH);
        fileHandler_seats.querySeat_fixed(name, seatNum);
    }

    bool seats_notfixed_query_USER(std::string& name, std::string& seatNum) {
        fileHandler_seats.seatFileHandler_seatNotFixed(seat_notfixed_PATH);
        fileHandler_seats.querySeat_fixed(name, seatNum);
    }

    bool seats_notfixed_allocation_USER(std::string& name, std::string& seatNum) {

    }

    bool activity_set_ADMIN(std::string& arg_input, std::string& arg_back) {
        fileHandler_activity.setFilename_(activity_PATH);
        fileHandler_activity.activity_set(arg_input, arg_back);
    }

    bool activity_query_USER(std::string& arg_input, std::string& arg_back) {
        fileHandler_activity.setFilename_(activity_PATH);
        fileHandler_activity.activity_query(arg_input, arg_back);
    }
};

#endif _FILEDATABASE_HPP_