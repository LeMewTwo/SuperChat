CXX=g++

CXXFLAGS=-Wall -O0 -g -std=c++11

CPPFLAGS=-I/usr/local/include/asio

all:chat_client chat_server cardfile

chat_client.o: chat_client.cpp chat_message.hpp

chat_server.o: chat_server.cpp chat_message.hpp

cardfile.o: cardfile.c cardfile.dat

chat_client: chat_client.o
	${CXX} -o chat_client chat_client.o -lpthread -lncurses

chat_server: chat_server.o
	${CXX} -o chat_server chat_server.o -lpthread -lncurses

cardfile: cardfile.o
	${CXX} -o cardfile cardfile.o -lpthread -lncurses -lform -lmenu -lpanel

clean:
	-rm -f chat_server chat_client chat_server.o chat_client.o cardfile cardfile.o
