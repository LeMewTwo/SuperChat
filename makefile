CXX=g++

CXXFLAGS = -Wall -O0 -g -std=c++11

CPPFLAGS=-I/usr/local/include/asio

all:chat_client chat_server

chat_client.o:chat_client.cpp chat_message.hpp chat_ncurses.hpp chat_login.hpp

chat_server.o:chat_server.cpp chat_message.hpp

chat_client: chat_client.o
	${CXX} -o chat_client chat_client.o -lpthread -lncurses

chat_server: chat_server.o
	${CXX} -o chat_server chat_server.o -lpthread

clean:
	-rm -f chat_server chat_client chat_server.o chat_client.o