#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#define PORT 12345
#define SERVER_ADDR "127.0.0.1"


void receive_msgs(int socketfd) {
    char msg_buffer[4096];
    while(true) {
        int msg_len = recv(socketfd, msg_buffer, 4096, 0);
        msg_buffer[msg_len] = '\0';
        std::cout << msg_buffer << std::endl;
    }
}


void send_msgs(int socketfd) {
    std::string msg;
    while(true) {
        std::getline(std::cin, msg);
        send(socketfd, msg.c_str(), msg.length(), 0);
    }
}


int main(void) {
    // Creating new IPv4 TCP socket:
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd == -1) {
        std::cerr << "Error creating the socket: "
                  << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Defining an address for the socket:
    struct sockaddr_in socket_addr;
    socket_addr.sin_family = AF_INET;                      // protocol family
    socket_addr.sin_port = htons(PORT);                    // port number
    socket_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);  // remote host address

    // Connecting to the server:
    if(connect(socketfd, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) == -1) {
        std::cerr << "Error connecting to the server (" << SERVER_ADDR << ":"
                  << PORT << "): " << ": " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Sending and receiving data:
    std::thread msg_receiver(receive_msgs, socketfd);
    std::thread msg_sender(send_msgs, socketfd);

    msg_receiver.join();
    msg_sender.join();

    // Closing the connection:
    close(socketfd);

    return 0;
}