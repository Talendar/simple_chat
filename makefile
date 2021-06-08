# Settings
FLAGS = -lstdc++ -std=c++11 -Wall -g -pthread
SERVER_BIN = ./bin/server
CLIENT_BIN = ./bin/client
LIB_PATH = ./lib
OBJ_PATH = ./obj


# Tasks
server:
	$(SERVER_BIN)

client:
	$(CLIENT_BIN)

all: clean server.o client.o simple_server.o
	gcc server.o simple_server.o -o $(SERVER_BIN) $(FLAGS)
	gcc client.o -o $(CLIENT_BIN) $(FLAGS)
	mv *.o $(OBJ_PATH)

server.o: server.cpp
	gcc -c server.cpp

simple_server.o: $(LIB_PATH)/simple_server.h $(LIB_PATH)/simple_server.cpp
	gcc -c $(LIB_PATH)/simple_server.cpp

client.o: client.cpp
	gcc -c client.cpp

clean:
	rm -rf $(OBJ_PATH)/*.o $(SERVER_BIN) $(CLIENT_BIN)
