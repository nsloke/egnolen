#ifndef UDPSERVER_H_INCLUDED
#define UDPSERVER_H_INCLUDED
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class UDPServer {
public:
    using ReceiveCallback = std::function<void(const std::string&)>;
    UDPServer(boost::asio::io_context& io_context, short port);
    void start_receive();
    void set_receive_callback(ReceiveCallback callback);
    void send(const std::string& message, const udp::endpoint& destination);


private:


    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_;
    ReceiveCallback receive_callback_;
};





#endif // UDPSERVER_H_INCLUDED
