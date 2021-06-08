# Settings
FLAGS = -lstdc++ -std=c++11 -Wall -g -pthread -lncurses
BIN = ./bin
LIB_PATH = ./lib
OBJ_PATH = ./obj


# Tasks
server:
	$(BIN)/server

client:
	$(BIN)/client

all: clean server.o client.o simple_server.o window.o
	mkdir -p $(BIN) $(OBJ_PATH)
	gcc server.o simple_server.o -o $(BIN)/server $(FLAGS)
	gcc client.o window.o -o $(BIN)/client $(FLAGS)
	mv *.o $(OBJ_PATH)

server.o: server.cpp
	gcc -c server.cpp

simple_server.o: $(LIB_PATH)/simple_server.h $(LIB_PATH)/simple_server.cpp
	gcc -c $(LIB_PATH)/simple_server.cpp

window.o: $(LIB_PATH)/window.h $(LIB_PATH)/window.cpp
	gcc -c $(LIB_PATH)/window.cpp
    
client.o: client.cpp
	gcc -c client.cpp

clean:
	rm -rf $(BIN) $(OBJ_PATH)
