/**
 * TODO
 */

#ifndef SIMPLE_SERVER_H
    #define SIMPLE_SERVER_H
    #include <vector>
    #include <atomic>
    #include <string>
    #include <mutex>

    /**
     * TODO
     */
    enum class Protocol {TCP, UDP};

    /**
     * TODO 
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
     * TODO
     * only works with internet protocols (TCP and UDP)
     */
    class SimpleServer {
        private:
            const Protocol protocol;
            const int port;
            const int queue_len;
            int socketfd;

        public:
            SimpleServer(Protocol protocol, int port, int queue_len);
            void run(void (*job)(int));
            void shutdown();
    };
#endif