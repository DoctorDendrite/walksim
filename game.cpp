
#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <conio.h>

const int VISION_RADIUS = 15;
const char BLOCK = 'x';
const char LOCATOR = 'o';

void update_display(const std::vector<std::string> & map, int x, int y)
{
	system("CLS");
	
	for (int j = y - VISION_RADIUS; j < y + VISION_RADIUS; ++j)
	{
		for (int i = x - VISION_RADIUS; i < x + VISION_RADIUS; ++i)
		{
			if (i == x && j == y)
				putchar(LOCATOR);
			else
				putchar(map[j][i]);
		}
		
		putchar('\n');
	}
}

void attempt_up(const std::vector<std::string> & map, int & x, int & y)
{
	if (map[y - 1][x] != BLOCK)
		y = y - 1;
}

void attempt_down(const std::vector<std::string> & map, int & x, int & y)
{
	if (map[y + 1][x] != BLOCK)
		y = y + 1;
}

void attempt_left(const std::vector<std::string> & map, int & x, int & y)
{
	if (map[y][x - 1] != BLOCK)
		x = x - 1;
}

void attempt_right(const std::vector<std::string> & map, int & x, int & y)
{
	if (map[y][x + 1] != BLOCK)
		x = x + 1;
}

int main()
{
	std::ifstream infile("map.txt");
	std::vector<std::string> map;
	
	std::string line;
	getline(infile, line);
	
	while (infile)
	{
		map.push_back(line);
		getline(infile, line);
	}
	
	infile.close();
	
	// for (int j = 0; j < map.size(); ++j)
	// {
	// 	for (int i = 0; i < map[j].length(); ++i)
	// 		std::cout << map[j][i];
	// 	
	// 	std::cout << std::endl;
	// }
	// 
	// std::cout << "\nSuccess!\n";
	
	int x, y;
	
	x = 31;
	y = 31;
	
	update_display(map, x, y);
	
	char key_press;
    int ascii_value;
	bool exit = false;
	
	while (!exit)
	{
		key_press = getch();
		ascii_value = key_press;
		
		if(ascii_value == 27) // For ESC
			break;
			
		switch (ascii_value)
		{
			case 27:
				exit = true;
				break;
			case 72:
				attempt_up(map, x, y);
				update_display(map, x, y);
				break;
			case 80:
				attempt_down(map, x, y);
				update_display(map, x, y);
				break;
			case 77:
				attempt_right(map, x, y);
				update_display(map, x, y);
				break;
			case 75:
				attempt_left(map, x, y);
				update_display(map, x, y);
				break;
		}
	}
	
	return 0;
}
