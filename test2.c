#include <ncurses.h>
#include <iostream>


class MainWindow
{
public:
	//sets the size of the window//
	int height;
	int width;
	//sets the position of the window//
	int start_y;
	int start_x;
	//actual pointer for the window//
	WINDOW * win;
	//a string that will be displayed
	std::string title;

	void SetUp()
	{	
		//initialize var//
		height = 50;
		width = 110;
		start_y = 1;
		start_x = 1;
		title = "Title";
		win = newwin(height,width,start_y,start_x);
		//refresh screen//
		refresh();
		//adds the text to the window//
		//takes in the pointer to the window//
		//aswell as the position inside the window//
		//and the text to display//
		mvwprintw(win, 1, 1, title.c_str());
		//adds a box border to the window//
		box(win,0,0);
		//refresh window//
		wrefresh(win);
	}
};




int main(void)
{
	// initialize screen //
	initscr();

	// initialize the class //
	MainWindow MainWin;

	// run the setup function for this class //
	MainWin.SetUp();
	//wrefresh(MainWin.win);

	refresh();

	// wait for a character then exit //
	getch();
	endwin();
	return 0;

}