#include <iostream>
#include <map>
#include <sstream>
#include <thread>
#include "lib/simple_server.h"
#include "lib/color.h"

#define PORT 12345
#define CONNECTION_QUEUE_LEN 10

#define SERVER_TAG Color::Bold + Color::Yellow + "[Server]" + Color::Reset
#define LOG_TAG Color::Bold + Color::Red + "[LOG]" + Color::Reset
#define MSG_TAG Color::Bold + Color::Cyan + "[MSG]" + Color::Reset

std::mutex clients_mutex;
std::map<int, Client> clients;


/**
 * Sends a message to all the clients, excluding the one with the specified
 * file descriptor (handle for the socket). Thread-safe.
 */
void server_broadcast(std::string msg, int exclude_fd) {
    clients_mutex.lock();
    for (auto const& it : clients) {
        int client_fd = it.first;
        if(client_fd != exclude_fd) {
            Client client = it.second;
            client.send_msg(msg);
        }
    }
    clients_mutex.unlock();
}


/**
 * Main function. Starts the server.
 */
int main(void) {
    SimpleServer server(Protocol::TCP, PORT, CONNECTION_QUEUE_LEN);
    std::cout << LOG_TAG << " The server is online!" << std::endl
              << LOG_TAG << " Listening to port " << PORT << std::endl;

    server.run([&server](int client_socketfd) {
        std::cout << LOG_TAG << " New client!" << std::endl;

        // Creating a new client object:
        Client client(client_socketfd);

        // Welcoming the new client and receiving an username:
        client.send_msg("[Server] You're online! What's your username?");

        std::string username = client.receive_msg();
        std::string user_tag = "[" + username + "]";

        // Adding the client to the clients dictionary:
        clients_mutex.lock();
        clients.insert(std::pair<int, Client>(client_socketfd, client));
        clients_mutex.unlock();

        // Announcing the new client:
        server_broadcast("[Server] User " + username + " " + 
                  "connected" + " to the chat!", client_socketfd);
        std::cout << LOG_TAG << " Starting interaction with "
                  << username << std::endl
                  << LOG_TAG << " Online users: " << clients.size() << std::endl;
        
        // Starting main interaction with the client:
        while(server.is_running()) {
            // Receiving a message:
            std::string received_msg = client.receive_msg();

            // Checking if the message is valid:
            // (empty messages mean that the client has disconnected)
            if(received_msg.length() == 0) {
                // Removing the client from the list and breaking from the
                // interaction loop
                clients_mutex.lock();
                clients.erase(clients.find(client_socketfd));
                clients_mutex.unlock();
                break;
            }

            // Formatting and logging the message
            std::string msg = user_tag + " " + received_msg;
            std::cout << MSG_TAG << msg << std::endl;

            // Sending the message to the other clients:
            server_broadcast(msg, client_socketfd);
        }

        // Closing the connection
        client.close_connection();

        // Announcing the client's disconnection:
        server_broadcast("[Server] User " + username + "disconnected"
                         + " from the chat!", client_socketfd);

        std::cout << LOG_TAG << " User " << username << " "
                  << Color::Red + "disconnected" + Color::Reset
                  << " from the server." << std::endl
                  << LOG_TAG << " Online users: " << clients.size() << std::endl;
    });

    std::cout << SERVER_TAG << " Closing the server..." << std::endl;

    return 0;
}
