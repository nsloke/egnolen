#include "UDPServer.h"

UDPServer::UDPServer(boost::asio::io_context& io_context, short port)
        : socket_(io_context, udp::endpoint(udp::v4(), port)) {
        start_receive();
    }

void UDPServer::set_receive_callback(ReceiveCallback callback) {
    receive_callback_ = std::move(callback);
}

void UDPServer::start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    std::cout << "Received: " << recv_buffer_.data() << std::endl;
                     std::string received_data(recv_buffer_.data(), bytes_recvd);
                    if (receive_callback_) {
                        receive_callback_(received_data);
                    }
                    start_receive();
                }
            });
}

void UDPServer::send(const std::string& message, const udp::endpoint& destination) {
        socket_.async_send_to(
            boost::asio::buffer(message),
            destination,
            [](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
                // Handle send completion if needed
            });
}
