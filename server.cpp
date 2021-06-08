#include <iostream>
#include <vector>
#include <sstream>
#include "lib/simple_server.h"

#define PORT 12345
#define CONNECTION_QUEUE_LEN 10


std::vector<Client*> clients;

int main(void) {
    SimpleServer server(Protocol::TCP, PORT, CONNECTION_QUEUE_LEN);

    server.run([](Client *client) {
        std::cout << "[LOG] New client!" << std::endl;

        // Welcoming the new client:
        client->send_msg("[Server] You're online! What's your username?");

        // Receiving an username:
        std::string username = client->receive_msg();

        // Adding the client to the clients list:
        clients.push_back(client);

        // Client interaction:
        std::cout << "[LOG] Starting interaction with " << username << std::endl;
        while(true) {
            // Receiving a message:
            std::string received_msg = client->receive_msg();
            std::string msg = "[" + username + "] " + received_msg;

            // Logging the message
            std::cout << "[MSG]" << msg << std::endl;

            // Sending the message to the other clients:
            for(Client *c: clients) {
                if(c != client)
                    c->send_msg(msg);
            }
        }

        // Closing the connection
        client->close_connection();
        delete client;
    });

    server.shutdown();
    return 0;
}