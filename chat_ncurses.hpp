/*
Implementation done by group 5

CSE 3310-003 spring 2019
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
	int HeightStart;
	int WidthStart;
	int HeightMax;
	int WidthMax;

	// actual pointer for the window //
	WINDOW * Win;

	// a string that will be displayed //
	std::string Title;

	void SetUp(std::string NickName)
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
		Title = NickName + ":";

		// set the window //
		Win = newwin(HeightMax,WidthMax,HeightStart,WidthStart);

		//refresh screen//
		refresh();

		//adds the text to the window//
		//takes in the pointer to the window//
		//aswell as the position inside the window//
		//and the text to display//

		mvwprintw(Win, 1, 1, Title.c_str());

		//sets position of the curser to text box//
		move(HeightStart+1, Title.length()+3);

		//adds a box border to the window//
		box(Win,0,0);

		//refresh window//
		wrefresh(Win);
	}


	// gets text from the text box //
	std::string GetText(int prevMessageLength)
	{
		// the message, size of max_body_length from //
		// chat_message.hpp //
		char message[500];

		// gets message from where curser is in text box //
		getstr(message);

		// convert to string //
		std::string Smsg = message;

		//fix for character appending bug//
		int length = Smsg.length();
		int difference = prevMessageLength - length;
		std::string space = "";
		if(difference > 0)
		{
			for(int i = 0; i < difference; i++)
			{
				space = space + " ";
			}
		}

		std::string finalMessage = Smsg + space;
		CurserReturn();
		return finalMessage;
	}

	// moves the curser back to the text box //
	void CurserReturn()
	{
		//sets position of the curser to text box//
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
			mvaddch(HeightStart+1, (Title.length()+3+i),space);
		}
		// return the curser back to start of text box //
		CurserReturn();
	}

	void ExitText(std::string NickName)
	{
		// free up the windows used //
		delwin(Win);
		SetUp(NickName);
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
	void AddMessage(std::string message, int color)
	{
		// add message to box's message list //
		Messages.push_back(message);

		// going through the messages //
		// then printing to the window //
		for(unsigned int i = 0; i < Messages.size(); i++)
		{
			// get the message //
			std::string MsgPrint = Messages.at( (Messages.size() - (i+1) ));

			// print the message //
			// if there are multiple messages, then it will show one //
			// after the other //

			// assigns default foreground&background color to terminal
			// text will not print w/o this
			use_default_colors();

			// allows text color to be changed
            start_color();

            // Choose text color based on user input
            switch(color)
            {
            case 0:
                init_pair(1, COLOR_BLACK, -1);
                break;
            case 1:
                init_pair(1, COLOR_RED, -1);
                break;
            case 2:
                init_pair(1, COLOR_GREEN, -1);
                break;
            case 3:
                init_pair(1, COLOR_YELLOW, -1);
                break;
            case 4:
                init_pair(1, COLOR_BLUE, -1);
                break;
            case 5:
                init_pair(1, COLOR_MAGENTA, -1);
                break;
            case 6:
                init_pair(1, COLOR_CYAN, -1);
                break;
            case 7:
                init_pair(1, COLOR_WHITE, -1);
                break;
            default:
                init_pair(1, COLOR_WHITE, -1);
                break;
            }

            wattron(Win, COLOR_PAIR(1));

			mvwprintw(Win, MessageY - i, MessageX, MsgPrint.c_str());
			wrefresh(Win);
		}
	}
};
