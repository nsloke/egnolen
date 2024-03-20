#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8888
#define BUFFER_SIZE 1024
#define DEST_IP "127.0.0.1"  // Destination IP address
#define LOCAL_IP "192.168.1.100" // Your desired source IP address

int mainu() {
    int sockfd;
    struct sockaddr_in dest_addr, local_addr;
    char buffer[BUFFER_SIZE] = "Hello from client";
    int send_len;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Prepare the destination address structure
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, DEST_IP, &dest_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sockfd);
        return -1;
    }

    // Prepare the local address structure
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(0);  // Use any available local port
    if (inet_pton(AF_INET, LOCAL_IP, &local_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sockfd);
        return -1;
    }

    // Bind the socket to the local address
    if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) == -1) {
        std::cerr << "Bind failed" << std::endl;
        close(sockfd);
        return -1;
    }

    // Send data to server
    send_len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (send_len == -1) {
        std::cerr << "Send failed" << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Message sent from " << LOCAL_IP << " to " << DEST_IP << ":" << PORT << std::endl;

    close(sockfd);
    return 0;
}
