#include <ncurses.h>
#include <iostream>

// This class makes up the MainWindow //
class LoginWindow
{
public:
	// sizes of the window //
	int HeightStart;// starting y
	int WidthStart;// starting x
	int HeightMax;// actual hieght in characters
	int WidthMax;// actual width in characters

	// sizes of the input window //
	int InputHeight;
	int InputWidth;
	int InputYStart;
	int InputXStart;

	// pointer for the windows //
	WINDOW * Win;
	WINDOW * InputWin;

	// a string that will be displayed //
	std::string Title;

	// this function initializes the window //
	void SetUp()
	{	
		//initialize var//
		getbegyx(stdscr, HeightStart, WidthStart);
		getmaxyx(stdscr, HeightMax, WidthMax);
		HeightMax = HeightMax/2;
		Title = "NickName:";

		InputHeight = 3; // firmly set the input height
		InputWidth = WidthMax/3; // set the input width 1/3 of the width of window
		InputYStart = HeightStart + 2;
		InputXStart = WidthStart + 2;

		Win = newwin(HeightMax,WidthMax,HeightStart,WidthStart);
		InputWin = newwin(InputHeight,InputWidth,InputYStart,InputXStart);
		//refresh screen//
		refresh();

		//adds the text to the window//
		//takes in the pointer to the window//
		//aswell as the position inside the window//
		//and the text to display//
		mvwprintw(Win, 1, 1, Title.c_str());
		//movew();
		//adds a box border to the window//
		box(Win,0,0);
		box(InputWin,0,0);

		//refresh window//
		wrefresh(Win);
		wrefresh(InputWin);
	}

	// like get text but gets the nickname //
	std::string GetUser()
	{
		// sets the curser to the input window //
		move(InputYStart+1,InputXStart+1);
		char name[11];
		getstr(name);
		std::string Sname = name;
		return Sname;
	}	

	// finish with user login //
	void ExitLogin()
	{
		// free up the windows used //
		delwin(Win);
		delwin(InputWin);
	}
};