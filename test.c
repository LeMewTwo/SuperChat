#include <ncurses.h>
#include <iostream>



void MainWindow(std::string msg)
{
	int height = 50;
	int width = 110;
	int start_y = 1;
	int start_x = 1; // where the cursor is going to be

	//initializes a window//
	WINDOW * win = newwin(height, width, start_y, start_x);
	refresh(); // refreshes

	//puts the window in a bordered box
	box(win, 0,0);
	mvwprintw(win, 1, 1, msg.c_str());
	wrefresh(win); // refreshes window

}


std::string TextWindow(std::string NickName)
{
	int height = 4;
	int width = 108;
	int start_y = 46;
	int start_x = 2; // where the cursor is going to be

	//initializes a window//
	WINDOW * win = newwin(height, width, start_y, start_x);
	refresh(); // refreshes

	//puts the window in a bordered box
	box(win, 0,0);
	mvwprintw(win, 1, 1, NickName.c_str());// prints user
	mvwprintw(win, 1, NickName.length() + 1, ":");
	move(start_y + 1, NickName.length() + 5);

	wrefresh(win); // refreshes window
	return "test";
}

void ChatWindow(std::string msg)
{
	int height = 42;
	int width = 108;
	int start_y = 4;
	int start_x = 2; // where the cursor is going to be

	//initializes a window//
	WINDOW * win = newwin(height, width, start_y, start_x);
	refresh(); // refreshes

	//puts the window in a bordered box
	box(win, 0,0);
	int bottom = 40;
	mvwprintw(win, bottom, 1, msg.c_str());

	wrefresh(win); // refreshes window

}


int main(void)
{
	// initializes the screen
	initscr();

	//prints to window
	//printw("hello world!");
	std::string message = "Main Window";
	MainWindow(message);

	message = "Josh";
	std::string send = TextWindow(message);
	std::cout << send << std::endl;
	
	message = "Chat Window";
	ChatWindow(message);

	//refreshes window
	refresh();

	//gets a character
	while(1)
	{
		getch();
	}

	//closes window
	endwin();
	return 0;
}


