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

	// sizes of the error window //
	int ErrorHieght;
	int ErrorWidth;
	int ErrorYStart;
	int ErrorXStart;

	// pointer for the windows //
	WINDOW * Win;
	WINDOW * InputWin;
	WINDOW * InputWinColor;
	WINDOW * ColorOptions;
	WINDOW * ErrorWin;

	// a string that will be displayed //
	std::string Title;
	std::string Title2;
	std::string Colors;
	std::string curr_color;

	// this function initializes the window //
	void SetUp()
	{
		//initialize var//
		getbegyx(stdscr, HeightStart, WidthStart);
		getmaxyx(stdscr, HeightMax, WidthMax);
		HeightMax = HeightMax/2;
		Title = "NickName:";
		Title2 = "Text Color:";
		Colors = "Color Menu:";
		curr_color = "Black - 0";

		InputHeight = 3; // firmly set the input height
		InputWidth = WidthMax/3; // set the input width 1/3 of the width of window
		InputYStart = HeightStart + 2;
		InputXStart = WidthStart + 2;

		Win = newwin(HeightMax+2,WidthMax,HeightStart,WidthStart);
		InputWin = newwin(InputHeight,InputWidth,InputYStart,InputXStart);
		InputWinColor = newwin(InputHeight, InputWidth, InputYStart+5, InputXStart);
		ColorOptions = newwin(10, InputWidth/3, InputYStart+10, InputXStart);
		//refresh screen//
		refresh();

		//adds the text to the window//
		//takes in the pointer to the window//
		//aswell as the position inside the window//
		//and the text to display//
		mvwprintw(Win, 1, 1, Title.c_str());
		mvwprintw(Win,6,1,Title2.c_str());
		mvwprintw(Win,11,1,Colors.c_str());

		// use_default_colors() assigns terminal default background colors
		// Will not print text without this
        use_default_colors();

        // Allows text color to be changed
        start_color();

        // Initializing Colors to a pair #
        init_pair(1, COLOR_BLACK, -1);
        init_pair(2,COLOR_RED,-1);
        init_pair(3,COLOR_GREEN,-1);
        init_pair(4,COLOR_YELLOW,-1);
        init_pair(5,COLOR_BLUE,-1);
        init_pair(6,COLOR_MAGENTA,-1);
        init_pair(7,COLOR_CYAN,-1);
        init_pair(8,COLOR_WHITE,-1);

        // Printing Color Menu in Different colors
        wattron(ColorOptions, COLOR_PAIR(1));
        mvwprintw(ColorOptions,1,1,curr_color.c_str());
        curr_color = "Red - 1";
        wattron(ColorOptions,COLOR_PAIR(2));
        mvwprintw(ColorOptions,2,1,curr_color.c_str());
        curr_color = "Green - 2";
        wattron(ColorOptions,COLOR_PAIR(3));
        mvwprintw(ColorOptions,3,1,curr_color.c_str());
        curr_color = "Yellow - 3";
        wattron(ColorOptions,COLOR_PAIR(4));
        mvwprintw(ColorOptions,4,1,curr_color.c_str());
        curr_color = "Blue - 4";
        wattron(ColorOptions,COLOR_PAIR(5));
        mvwprintw(ColorOptions,5,1,curr_color.c_str());
        curr_color = "Magenta - 5";
        wattron(ColorOptions,COLOR_PAIR(6));
        mvwprintw(ColorOptions,6,1,curr_color.c_str());
        curr_color = "Cyan - 6";
        wattron(ColorOptions,COLOR_PAIR(7));
        mvwprintw(ColorOptions,7,1,curr_color.c_str());
        curr_color = "White - 7";
        wattron(ColorOptions,COLOR_PAIR(8));
        mvwprintw(ColorOptions,8,1,curr_color.c_str());

		//movew();
		//adds a box border to the window//
		box(Win,0,0);
		box(InputWin,0,0);
		box(InputWinColor,0,0);
		box(ColorOptions,0,0);

		//refresh window//
		wrefresh(Win);
		wrefresh(InputWin);
		wrefresh(InputWinColor);
		wrefresh(ColorOptions);
	}

	void ErrorPop(std::string error)
	{
		int size = error.length();
		ErrorWin = newwin(HeightMax/6,size + 2,HeightMax/2,WidthMax/3);
		refresh();
		mvwprintw(ErrorWin, 1, 1, error.c_str());
		box(ErrorWin,0,0);
		wrefresh(ErrorWin);
	}


	// like get text but gets the nickname //
	std::string GetUser()
	{
		// sets the curser to the input window //
		move(InputYStart+1,InputXStart+1);
		char name[200];
		getstr(name);
		std::string Sname = name;
		return Sname;
	}
    // Gets text color
	int GetColor()
	{
	    move(InputYStart+6, InputXStart+1);
	    char color[11];
	    getstr(color);
	    int Color = atoi(color);
	    return Color;
	}

	// finish with user login //
	void ExitLogin()
	{
		// free up the windows used //
		delwin(Win);
		delwin(InputWin);
		delwin(InputWinColor);
		delwin(ColorOptions);

	}
};
