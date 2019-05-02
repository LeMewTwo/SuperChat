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
#include <vector>
#include "asio.hpp"
#include "chat_message.hpp"
#include "chat_ncurses.hpp"
#include "chat_login.hpp"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

int g_Color;

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

// should hold the names of the participants //
std::vector<std::string> Names;

// list of blocked users //
std::vector<std::string> Blocked;


void addBlockName(std::string name)
{
  Blocked.push_back(name);
}

int checkNames(std::string Name)
{

  int count = Names.size();
  int size = Name.length();

  for(int i; i < count; i++)
  {
    if(Name == Names.at(i))
    {
      return 1;
    }
  }

  if(size > 11)
  {
    return 2;
  }

  Names.push_back(Name);

  return 0;
}


// fixes the bug that we had with appending the messages //
std::string cleanMessage(std::string msg)
{
  std::string returnString;
  int size = msg.length();
  for(int i=0; i<size; i++)
  {
      if(msg[i] == '$')
      {
        break;
      }
      returnString += msg[i];
  }
  return returnString;
}


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

            if(read_msg_.body() != NULL)
            {
              wclear(ChatWin.Win);
              box(ChatWin.Win,0,0);
              wrefresh(ChatWin.Win);

              std::string msgNcurses(read_msg_.body(), read_msg_.body_length());

              std::string blockName;

              int size = msgNcurses.length();
              for(int i=0; i<size; i++)
              {
                  if(msgNcurses[i] == ':')
                  {
                    break;
                  }
                  blockName += msgNcurses[i];
              }




              ChatWin.AddMessage(msgNcurses, g_Color);
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
    g_Color = LogWin.GetColor();
    LogWin.ExitLogin();

    // assuming most of this initializes the needs for the client to server //
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    // ERROR CHECKING FOR NICKNAME //
    int errorStatus = c.checkNames(NickName);
    if(errorStatus > 0)
    {
      std::string errorString;

      if(errorStatus == 1)
      {
        errorString = "ERROR: Name Is Already Chosen.";
      }else if(errorStatus == 2)
      {
        errorString = "ERROR: Name is longer than 10 characters.";
      }else if(errorStatus == 3)
      {
        errorString = "ERROR: Number of users is at 50.";
      }

      LoginWindow LogWin;
      LogWin.SetUp();
      LogWin.ErrorPop(errorString);
      NickName = LogWin.GetUser();
      g_Color = LogWin.GetColor();
      LogWin.ExitLogin();
      errorStatus = c.checkNames(NickName);
    }

    // initialize the window classes //
    MainWindow MainWin;
    TextWindow TextWin;

    // run the setup function for these classes //
    MainWin.SetUp();
    TextWin.SetUp(NickName);

    //ChatWin.SetUp();// moved to inside the chat client class //
    refresh();

    c.ChatWin.SetUp(); // set up chat window

    // runs a thread with the important thingy above //
    std::thread t([&io_context](){ io_context.run(); });

    // used to grab the line //
    char line[chat_message::max_body_length + 1];
    bool exit = false;
    while (1)
    {
      // grabs line //
      std::string Message = TextWin.GetText();
      
      Message = c.cleanMessage(Message);

      // error checking //
      if((Message.length()) > 500)
      {
        Message = "ERROR: message longer than 500 characters.";
      }
      if(Message == "[Exit]")
      {
        exit = true;
        Message = "*Left the room*";
      }

      std::string msgNcurses = NickName + ":" + Message;

      // convert msgNcurses from string to char[] //
      std::strcpy(line, msgNcurses.c_str());
      TextWin.ClearText();

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
      TextWin.CurserReturn();

      if(exit == true)
      {
        break;
      }
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
