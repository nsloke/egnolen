// Receiver.cpp
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

#define PORT 8889
#define BUFFER_SIZE 1024
#include "UDPServer.h"
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int mainx() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE] = {0};
    int recv_len, addr_len;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Prepare the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Socket bind failed" << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    addr_len = sizeof(client_addr);
    // Receive data from sender
    recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
    if (recv_len == -1) {
        std::cerr << "Receive failed" << std::endl;
        close(sockfd);
        return -1;
    }

    // Print received data and sender address
    std::cout << "Received from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " - " << buffer << std::endl;

    close(sockfd);
    return 0;
}

int main() {
    try {
        boost::asio::io_context io_context1;
        UDPServer server1(io_context1, 8891);
        //io_context1.run();


        std::thread thread1([&io_context1]() { io_context1.run(); });

        boost::asio::io_context io_context2;
        UDPServer server2(io_context2, 8892);
        //io_context2.run();
        std::thread thread2([&io_context2]() { io_context2.run(); });

        // Simulate communication between servers
       // std::this_thread::sleep_for(std::chrono::seconds(3)); // Allow time for servers to start

        // Send a message from server1 to server2
        std::cout<<"\nConsole Print Hello from Server Master 1\n";
        server1.send("Hello from Server Master 1", udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1988));

        server1.set_receive_callback([](const std::string& data) {
            std::cout << "Received data outside function: " << data << std::endl;
            // Process received data here
        });

        // Send a message from server2 to server1
        std::cout<<"\nConsole Print Hello from Server Master 2\n";
        server2.send("Hello from Server Master 2", udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 2988));

        server2.set_receive_callback([](const std::string& data) {
            std::cout << "Received data outside function: " << data << std::endl;
            // Process received data here
        });

        // Join threads
        thread1.join();
        thread2.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
