#include <motion_server_session.hpp>

MotionServerSession::MotionServerSession(std::string remote_ip, unsigned short port, std::string motor_name) : 
    motor_name(motor_name), 
    socket(io_service),
    remote_endpoint(address::from_string(remote_ip), port)
{
    socket.open(udp::v4());
}

void MotionServerSession::set_target(float target) {
    boost::system::error_code err;
    auto message = "{\"Controller\":[\"" + motor_name + "\",{\"SetTarget\":" + std::to_string(target) + "}]}\n";
    auto bytes_sent = socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, err);
    assert(message.length() == bytes_sent);
}

MotionServerSession::~MotionServerSession() {
    socket.close();    
}
