/**
 * TODO
 */

#ifndef SIMPLE_SERVER_H
    #define SIMPLE_SERVER_H
    #include <vector>
    #include <atomic>
    #include <string>
    #include <mutex>
    #include <functional>

    /**
     * Protocols supported by the server.
     */
    enum class Protocol {TCP, UDP};

    /**
     * Encapsulates a socket for the purpose of communicating with a client. 
     */
    class Client {
        private:
            const int recv_buffer_len;

        public:
            const int socketfd;
            Client(int socketfd, int recv_buffer_len=4096);
            void close_connection();

            int send_msg(std::string msg);
            std::string receive_msg();

            bool operator==(const Client& other) const;
            bool operator!=(const Client& other) const;
    };

    /**
     * Simple general-purpose server that can communicate with clients.
     * Note: only works with internet protocols (TCP and UDP).
     */
    class SimpleServer {
        private:
            const Protocol protocol;
            const int port;
            const int queue_len;
            int socketfd;
            std::atomic<bool> running;

        public:
            SimpleServer(Protocol protocol, int port, int queue_len);
            void run(std::function<void(int)> job);
            void shutdown();
            bool is_running();
    };
#endif