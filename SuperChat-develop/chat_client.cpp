//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"
#include "chat_ncurses.hpp"
#include "chat_login.hpp"
//#include "chat_server.cpp"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
  // client contstructor //
  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    // does some important stuff i dont know how to explain //
    do_connect(endpoints, ChatWin);

    // chat window initialized //
    ChatWindow ChatWin;
    
  }

// chat window needs to be initialized here so that //
// there can be interactions between clients //
ChatWindow ChatWin;

  // does some important stuff i dont know how to explain //
  void write(const chat_message& msg)
  {
    asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  // closes the client //
  void close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }

// lol, private but i can see it with my eyes //
private:

  // does some important stuff i dont know how to explain //
  void do_connect(const tcp::resolver::results_type& endpoints,
    ChatWindow ChatWin)
  {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  // does some important stuff i dont know how to explain //
  void do_read_header()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  // this is where the client and the server pretty much meet to send things //
  // between multiple clients //
  void do_read_body()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            // original code that writes what is sent to the server //
            /*std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";*/

            if(read_msg_.body() != NULL)
            {
              std::string msgNcurses(read_msg_.body(), read_msg_.body_length());
              //std::cout << msgNcurses + "!" << std::endl; // debug statement 
              //msgNcurses = Name + ":" + msgNcurses; // doesnt work
              ChatWin.AddMessage(msgNcurses);
            }

            do_read_header();
          }
          else
          {
            socket_.close();
          }
        });
  }

  // does some important stuff i dont know how to explain //
  void do_write()
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
  }

// lol, more privates //
private:
  asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};



// if you dont know what a main is, you should probably switch majors //
int main(int argc, char* argv[])
{
  try
  {
    // Error Checking for client initialization //
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }
    // no idea what this is, but seems important //
    asio::io_context io_context; // important thingy


    // NCURSES SET UP //
    // initialize screen //
    initscr();

    // LOGIN //
    LoginWindow LogWin;
    LogWin.SetUp();
    std::string NickName;
    NickName = LogWin.GetUser();
    LogWin.ExitLogin();
 


    // initialize the window classes //
    MainWindow MainWin;
    TextWindow TextWin;

    // run the setup function for these classes //
    MainWin.SetUp();
    TextWin.SetUp(NickName);

    //ChatWin.SetUp();// moved to inside the chat client class //
    refresh();


    // assuming most of this initializes the needs for the client to server //
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);
    c.ChatWin.SetUp(); // set up chat window

    // runs a thread with the important thingy above //
    std::thread t([&io_context](){ io_context.run(); });

    // used to grab the line //
    char line[chat_message::max_body_length + 1];
    while (1)
    {
      // int ch;

      // ch = getch();
      // grabs line //
      // given initially, commented out to use ncurses //
      //std::cin.getline(line, chat_message::max_body_length + 1); // dont need no mo

      // get the text from user //
      std::string msgNcurses = NickName + ": " + TextWin.GetText();

      // if (msgNcurses == (NickName + ": " + "next"))
      // if(ch == KEY_LEFT)
      if(i == 1)
      {
        //give chat_server main(argc, argv[]) new port number
        system("./chat_client 127.0.0.1 9001");
        system("xdotool key ctrl+c"); //closes program
        //system("xdotool key ctrl+shift+t"); //allows you to open a new tab
        //system("xdotool key alt+3"); // ctrl+PageDown -> keydown alt key Tab
        //system("./chat_server 9001");
        //system("./chat_client 127.0.0.1 9001");
      }

      // convert msgNcurses from string to char[] //
      std::strcpy(line, msgNcurses.c_str());

      TextWin.ClearText(msgNcurses.length());
      
      // the chat message class initialization //
      chat_message msg;

      // makes the chat message class's body length equal to input //
      msg.body_length(std::strlen(line));

      // places input into chat message //
      std::memcpy(msg.body(), line, msg.body_length());

      // encodes the message, dont know what that means but it's necessary //
      msg.encode_header();

      // writes message //
      c.write(msg);
      //std::cout << msg.body() << std::endl; // the .body() gets the string from msg, also debug statement

    }

    // client cleanup //
    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  // ncurses cleanup //
  endwin();
  return 0;
}
