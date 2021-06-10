/**
 * TODO 
 */

#include "simple_server.h"
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <thread>


/**
 * TODO 
 */
SimpleServer::SimpleServer(Protocol protocol,
                           int port,
                           int queue_len) : protocol(protocol), port(port),
                                            queue_len(queue_len), running(true) {
    // Determining the protocol type:
    int socket_type = (this->protocol == Protocol::TCP) ? SOCK_STREAM : SOCK_DGRAM;

    // Creating the socket:
    this->socketfd = socket(AF_INET, socket_type, 0);
    if(this->socketfd == -1) {
        std::ostringstream msg_stream;
        msg_stream << "Error creating the socket: " 
                   << strerror(errno) << std::endl;
        throw std::runtime_error(msg_stream.str());
    }

    // Defining the socket's address:
    struct sockaddr_in socket_addr;
    socket_addr.sin_family = AF_INET;          // protocol family
    socket_addr.sin_port = htons(this->port);  // port number
    socket_addr.sin_addr.s_addr = INADDR_ANY;  // remote host address (any)

    // Setting the socket options:
    // (makes the socket reuse the address and port; useful when the server restarts)
    int opt = 1;
    if (setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::ostringstream msg_stream;
        msg_stream << "Error setting the socket options: "
                   << strerror(errno) << std::endl;
        throw std::runtime_error(msg_stream.str());
    }

    // Binding the socket to the port:
    if(bind(this->socketfd, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) == -1) {
        std::ostringstream msg_stream;
        msg_stream << "Error binding the socket to port " << this->port
                   << ": " << strerror(errno) << std::endl;
        throw std::runtime_error(msg_stream.str());
    }

    // Making the socket accept incoming connection requests (server):
    if(listen(this->socketfd, this->queue_len) == -1) {
        std::ostringstream msg_stream;
        msg_stream << "Error making the socket listen to port " << this->port
                   << ": " << strerror(errno) << std::endl;
        throw std::runtime_error(msg_stream.str());
    }
}


/**
 * TODO
 */
void SimpleServer::run(std::function<void(int)> job) {
    // Create thread to get input from server user
    std::thread input_thread([&](){
        std::string input_string;
        do {
            std::cin >> input_string;
        } while(running && input_string != "exit");
        if(running) {
            shutdown();
        }
    });

    // Get connections and create threads
    while(running) {
        // Accepting the first connection request on the queue:
        // (if the queue is empty, blocks the caller until a connection is present)
        int client_socket = accept(this->socketfd, 0, 0);
        if(client_socket == -1) {
            // Check if server was closed properly
            if(!running) {
                break;
            }

            std::ostringstream msg_stream;
            msg_stream << "Error accepting a new connection: " 
                    << strerror(errno) << std::endl;
            throw std::runtime_error(msg_stream.str());
        }

        // Creating a new thread to execute the job:
        std::thread job_thread(job, client_socket);
        job_thread.detach();
    }

    input_thread.join();
}


/**
 * Shutdown the server and close the socket
 */
void SimpleServer::shutdown() {
    this->running = false;
    // Call shutdown from the socket standard library
    ::shutdown(this->socketfd, SHUT_RDWR);
    close(this->socketfd);
}

/**
 * Return whether the server is running or not
 * 
 * Note: thread-safe
 */
bool SimpleServer::is_running() {
    return running;
}

/**
 * TODO
 */
Client::Client(int socketfd, int recv_buffer_len) : socketfd(socketfd),
                                                    recv_buffer_len(recv_buffer_len) {}


/**
 * Close client connection
 */
void Client::close_connection() {
    close(this->socketfd);
}


/**
 * Send message to client
 * 
 * Note: thread-safe.
 */
int Client::send_msg(std::string msg) {
    // Since POSIX defines send as an atomic operation and since each thread is
    // using its own buffer (c_msg bellow), this routine should be thread-safe.
    // Source: https://stackoverflow.com/questions/1981372/are-parallel-calls-to-send-recv-on-the-same-socket-valid
    
    const char *c_msg = msg.c_str();
    return send(this->socketfd, c_msg, strlen(c_msg), 0);
}


/**
 * Receive message from client
 * 
 * Note: thread-safe.
 */
std::string Client::receive_msg() {
    // Since POSIX defines recv as an atomic operation and since each thread is
    // using its own buffer (allocated bellow), this routine should be thread-safe.
    // Source: https://stackoverflow.com/questions/1981372/are-parallel-calls-to-send-recv-on-the-same-socket-valid

    char *recv_buffer = new char[this->recv_buffer_len];
    int in_len = recv(this->socketfd, recv_buffer, this->recv_buffer_len, 0);
    
    recv_buffer[in_len] = '\0';

    std::string msg(recv_buffer);
    delete[] recv_buffer;

    return msg;
}

bool Client::operator==(const Client& other) const {
    return (socketfd == other.socketfd);
}

bool Client::operator!=(const Client& other) const {
    return !(*this == other);
}