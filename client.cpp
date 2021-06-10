#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include "lib/window.h"

#define PORT 12345
#define SERVER_ADDR "127.0.0.1"

Window win;
std::atomic<bool> running(true);

/**
 * Receives and prints messages from the server until the program is stopped or
 * the connection with the server is closed.
 */
void receive_msgs(int socketfd) {
    char msg_buffer[4096];
    while(running) {
        // Trying to get a message from the server:
        int msg_len = recv(socketfd, msg_buffer, 4096, 0);
        if(msg_len <= 0) {
            // Checking for failure:
            if(running) {
                std::cerr << ">> Connection closed by the server. "
                          << "Please try again later." << std::endl;
                exit(EXIT_FAILURE);
            } else {
                // If it's not running, then it's just a shutdown command
                break;
            }
        }

        // Log message
        msg_buffer[msg_len] = '\0';
        win.log_message(msg_buffer);
    }
}


/**
 * Reads messages from the user and sends them to the server.
 */
void send_msgs(int socketfd) {
    std::string msg;

    // Logging in the server with an username:
    msg = win.get_message();
    send(socketfd, msg.c_str(), msg.length(), 0);
    win.clear_input();

    // Starting the chat:
    while(running) {
        msg = win.get_message();
        if(msg.length() == 0) {
            // Don't send empty messages
            continue;
        }

        // Checking if it's not a command:
        if(msg[0] != '\\') {
            send(socketfd, msg.c_str(), msg.length(), 0);
            win.log_message((std::string("[Me] ") + msg).c_str());
        } else if(msg == "\\exit") {
            // Closing the connection
            shutdown(socketfd, SHUT_RDWR);
            close(socketfd);
            running = false;;
        }

        // Clearing the input window after sending a message
        win.clear_input();
    }
}


/**
 * Main function. Connects to the server.
 */
int main(void) {
    // Creating new IPv4 TCP socket to communicate with the server:
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

    return 0;
}