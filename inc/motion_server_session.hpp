#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::address;

class MotionServerSession {
    public:
        MotionServerSession(std::string remote_ip, unsigned short port, std::string motor_name);
        ~MotionServerSession();
        void set_target(float target);
    private:
        std::string motor_name;
        boost::asio::io_service io_service;
        udp::socket socket;
        udp::endpoint remote_endpoint;
};
