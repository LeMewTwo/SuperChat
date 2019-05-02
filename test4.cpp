#include <iostream>
#include <vector>


int main()
{
	std::cout << "enter a color" << std::endl;

	std::string color;
	std::string blue = "blue";
	std::string red = "red";
	std::cin >> color;

	switch(color)
	{
		case blue:
			std::cout << "you chose blue" << std::endl;
			break;
		case red:
			std::cout << "you chose red" << std::endl;
		default:
			std::cout << "ERROR" << std::endl;
	}

	return 0;
}