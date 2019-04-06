/*
Example done by Joshua Tran
From group 5
CSE 3310-003 spring 2019

PROTOTYPE 2

purpose of this program is to demonstrate the use of Ncurses
in implementation for classes and testing
*/


#include <ncurses.h>
#include <iostream>
#include <vector>

// This class makes up the MainWindow //
class MainWindow
{
public:
	// sizes of the window //
	int HeightStart;// starting y
	int WidthStart;// starting x
	int HeightMax;// actual hieght in characters
	int WidthMax;// actual width in characters

	// pointer for the window //
	WINDOW * Win;

	// a string that will be displayed //
	std::string Title;

	// this function initializes the window //
	void SetUp()
	{	
		//initialize var//
		getbegyx(stdscr, HeightStart, WidthStart);
		getmaxyx(stdscr, HeightMax, WidthMax);
		Title = "Main Lobby";
		Win = newwin(HeightMax,WidthMax,HeightStart,WidthStart);

		//refresh screen//
		refresh();

		//adds the text to the window//
		//takes in the pointer to the window//
		//aswell as the position inside the window//
		//and the text to display//
		mvwprintw(Win, 1, 1, Title.c_str());

		//adds a box border to the window//
		box(Win,0,0);

		//refresh window//
		wrefresh(Win);
	}
};

class TextWindow
{
public:
	// sizes of the window //
	// same as main window //
	int HeightStart;
	int WidthStart;
	int HeightMax;
	int WidthMax;

	// actual pointer for the window //
	WINDOW * Win;

	// a string that will be displayed //
	std::string Title;

	void SetUp()
	{	
		//initialize var//
		getbegyx(stdscr, HeightStart, WidthStart);
		getmaxyx(stdscr, HeightMax, WidthMax);
		// These variables are set to have specific //
		// proportions to the screen //
		HeightStart = HeightMax - 5; 
		WidthStart = WidthStart + 1;
		HeightMax = 4; // hieght needs to be firmly set//
		WidthMax = WidthMax - 2; // leaving some spacing for border//

		// need to replace with user information //
		// probably need to pass that to this and //
		// add some padding for the ":" //
		Title = "TextTitle:"; 

		// set the window //
		Win = newwin(HeightMax,WidthMax,HeightStart,WidthStart);

		//refresh screen//
		refresh();

		//adds the text to the window//
		//takes in the pointer to the window//
		//aswell as the position inside the window//
		//and the text to display//
		mvwprintw(Win, 1, 1, Title.c_str());

		//sets position of the curser//
		move(HeightStart+1, Title.length()+3);

		//adds a box border to the window//
		box(Win,0,0);

		//refresh window//
		wrefresh(Win);
	}


	// gets text from the text box //
	std::string GetText()
	{
		char message[500];
		getstr(message);
		std::string Smsg = message;
		CurserReturn();
		return Smsg;
	}

	// moves the curser back to the text box //
	void CurserReturn()
	{
		move(HeightStart+1, Title.length()+3);
		wrefresh(Win);
	}

	// clears the text box //
	// there is an issue here though and probably in get text //
	// when we send the text, remenesants of the previous texts still show //
	void ClearText(int length)
	{
		for(int i = 0; i < length; i++)
		{
			int space = 32; // int value for ascii space
			//std::string StrSpace = " ";

			// covers up the previous message //
			mvaddch(HeightStart+1, (Title.length()+3+i),32);
		}
		// return the curser back to start of text box //
		CurserReturn(); 
	}
};

class ChatWindow
{
public:
	// sizes of the window //
	int HeightStart;
	int WidthStart;
	int HeightMax;
	int WidthMax;

	// to help keep track of where we should put the messages //
	int MessageX; 
	int MessageY;

	// actual pointer for the window //
	WINDOW * Win;

	// a strings that will be displayed //
	//std::string Title; // might not need this no more
	std::vector<std::string> Messages;

	void SetUp()
	{	
		//initialize var//
		getbegyx(stdscr, HeightStart, WidthStart);
		getmaxyx(stdscr, HeightMax, WidthMax);

		// These variables are set to have specific //
		// proportions to the screen //
		HeightStart = HeightStart + 3;
		WidthStart = WidthStart + 1;
		HeightMax = (5*HeightMax)/6; // hieght needs to be firmly set
		WidthMax = WidthMax - 2; // leaving some spacing for border

		//Title = "ChatTitle:";
		Win = newwin(HeightMax,WidthMax,HeightStart,WidthStart);
		//refresh screen//
		refresh();

		// starting position of messages //
		MessageY = HeightMax - 3; 
		MessageX = WidthStart + 1; 

		//adds the text to the window//
		//takes in the pointer to the window//
		//aswell as the position inside the window//
		//and the text to display//
		//mvwprintw(Win, MessageY, MessageX, Title.c_str()); // might not need this

		//adds a box border to the window//
		box(Win,0,0);

		//refresh window//
		wrefresh(Win);
	}

	// takes in a message and pushes it to the window //
	void AddMessage(std::string message)
	{
		// add message to box's message list //
		Messages.push_back(message);

		// going through the messages //
		// then printing to the window //
		for(int i = 0; i < Messages.size(); i++)
		{
			// get the message //
			std::string MsgPrint = Messages.at( (Messages.size() - (i+1) ));
			mvwprintw(Win, MessageY - i, MessageX, MsgPrint.c_str());
			wrefresh(Win);
		}
	}
};


int main(void)
{
	// initialize screen //
	initscr();

	// initialize the class //
	MainWindow MainWin;
	TextWindow TextWin;
	ChatWindow ChatWin;

	// run the setup function for these classes //
	MainWin.SetUp();
	TextWin.SetUp();
	ChatWin.SetUp();
	refresh();


	// this will be replaced with a thread in the main program //
	while(1)
	{
		// get the text from user //
		std::string msg = TextWin.GetText();

		// add the message to chat window //
		ChatWin.AddMessage(msg);

		// clear the text window //
		TextWin.ClearText(msg.length());
		
	}

	// wait for a character then exit //
	endwin();
	return 0;

}