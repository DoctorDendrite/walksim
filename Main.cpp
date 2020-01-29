#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <Windows.h>

using namespace std;

void InitializeGame(vector<const char*>& CharList, vector<string>& currMap);
void UpdateScreen(vector<const char*>& CharList, vector<string>& currMap, pair<int, int>& playerPos, HANDLE& hConsole);
void cls();

enum ColorCodes
{
	BLUE = 9, GREEN, AQUA, RED, MAGENTA, YELLOW, WHITE
};

enum UnicodeChar
{
	Player, Floor, Horizontal, Vertical, TopLeftCorner, TopRightCorner, BottomLeftCorner, BottomRightCorner,
	Door, Corridor, SIZE_OF_UNICODECHAR
};

bool canMoveUp, canMoveDown, canMoveLeft, canMoveRight;

bool IsNonBlocking(char currChar)
{
	return currChar == '.' || currChar == 'x' || currChar == '+';
}

int main() {

	// Set the output to UTF-8, so we can draw symbols like boxes
	SetConsoleOutputCP(CP_UTF8);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	vector<const char*> CharList;
	vector<string> currMap;

	int userInput; char currChar;
	
	// X, Y
	pair<int, int> playerPos = {6, 3};

	InitializeGame(CharList, currMap);
	
	/*const char* s = u8"\u2592";
	cout << s << endl << endl;
	SetConsoleTextAttribute(hConsole, WHITE);
	for (int i = 0; i < SIZE_OF_UNICODECHAR; i++) {
		cout << CharList[i] << endl;

	}*/

	// Game Loop
	while(1){
		
		UpdateScreen(CharList, currMap, playerPos, hConsole);
		
		// Update booleans
		canMoveUp = IsNonBlocking(currMap[playerPos.second - 1][playerPos.first]);
		canMoveDown = IsNonBlocking(currMap[playerPos.second + 1][playerPos.first]);
		canMoveLeft = IsNonBlocking(currMap[playerPos.second][playerPos.first - 1]);
		canMoveRight = IsNonBlocking(currMap[playerPos.second][playerPos.first + 1]);
		
		// Wait for user input key before the world updates
		userInput = _getch();
		
		// Replace with an if-else when we create a custom key binding function
		switch (char(userInput)) {

			// W
		case 'w':
		case 'W':
		case 72:

			if (canMoveUp) {
				playerPos.second--;
			}
			break;

			// S
		case 's':
		case 'S':
		case 80:

			if (canMoveDown) {
				playerPos.second++;
			}
			break;

			// A
		case 'a':
		case 'A':
		case 75:

			if (canMoveLeft) {
				playerPos.first--;
			}
			break;

			// D
		case 'd':
		case 'D':
		case 77:

			if (canMoveRight) {
				playerPos.first++;
			}
			break;

		}
	}
	
	_getch();	
	
	return 0;
}


void UpdateScreen(vector<const char*>& CharList, vector<string>& currMap, pair<int, int>& playerPos, HANDLE& hConsole) {

	char currChar; string currPrint, currAction = "";
	
	system("cls");

	// DEBUG MENU
	/*SetConsoleTextAttribute(hConsole, WHITE);
	cout << "X: " << playerPos.first << endl << "Y: " << playerPos.second << endl;
	cout << "canMoveUp: " << canMoveUp << endl;
	cout << "canMoveDown: " << canMoveDown << endl;*/

	// Current action HUD text
	SetConsoleTextAttribute(hConsole, YELLOW);
	currAction = "Your attack just did 15 damage";
	std::cout << currAction << endl;
		
	// Read in the currMap vector, string by string
	for (int row = 0; row < currMap.size(); row++) {

		// Parse each character per row
		for (int CharNum = 0; CharNum < currMap[row].length(); CharNum++) {

			if (row == playerPos.second && CharNum == playerPos.first) {
				SetConsoleTextAttribute(hConsole, YELLOW);
				currPrint = CharList[Player];
			}
			else {

				currChar = currMap[row][CharNum];
				switch (currChar) {

					// Empty space
				case ' ':

					currPrint = ' ';
					break;

				//	// Player character (DELETE THIS, ONLY FOR TESTING PURPOSES)
				//case 'P':
				//	SetConsoleTextAttribute(hConsole, YELLOW);
				//	currPrint = CharList[Player];
				//	break;

					// Floor
				case '.':
					SetConsoleTextAttribute(hConsole, GREEN);
					currPrint = CharList[Floor];
					break;

					// Horizontal
				case '-':
					SetConsoleTextAttribute(hConsole, MAGENTA);
					currPrint = CharList[Horizontal];
					break;

					// Vertical
				case '|':
					SetConsoleTextAttribute(hConsole, MAGENTA);
					currPrint = CharList[Vertical];
					break;

					// TopLeftCorner
				case '{':
					SetConsoleTextAttribute(hConsole, MAGENTA);
					currPrint = CharList[TopLeftCorner];
					break;

					// TopRightCorner
				case '}':
					SetConsoleTextAttribute(hConsole, MAGENTA);
					currPrint = CharList[TopRightCorner];
					break;

					// BottomLeftCorner
				case '[':
					SetConsoleTextAttribute(hConsole, MAGENTA);
					currPrint = CharList[BottomLeftCorner];
					break;

					// BottomRightCorner
				case ']':
					SetConsoleTextAttribute(hConsole, MAGENTA);
					currPrint = CharList[BottomRightCorner];
					break;

					// Door
				case '+':
					SetConsoleTextAttribute(hConsole, MAGENTA);
					currPrint = CharList[Door];
					break;

					// Corridor
				case 'x':
					SetConsoleTextAttribute(hConsole, WHITE);
					currPrint = CharList[Corridor];
					break;

					// If unrecognized, then print the symbol itself, for debugging purposes
				default:
					SetConsoleTextAttribute(hConsole, 207);
					currPrint = currMap[row][CharNum];
					break;
				}
				// End switch
			}
			cout << currPrint;

		}

		std::cout << endl;

	}	// end switch

	// Display Rogue's stats in HUD
	cout << endl;
	SetConsoleTextAttribute(hConsole, YELLOW);
	cout << "Level: 4" << "   " << "Hits: 29" << "   " << "Str: 16" << "   " << "Gold: 718" << "   " << "Armor: 5" << "   " << "Exp: 4" << endl;

}

// https://stackoverflow.com/questions/34842526/update-console-without-flickering-c
void cls()
{
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}

void InitializeGame(vector<const char*>& CharList, vector<string>& currMap)
{
	CharList = {

		//	0 Player 		1 Floor			2 Horizontal	3 Vertical		4 TopLeft		5 TopRight		6 BottomLeft	7 BottomRight
		u8"\u263A",		u8"\u00B7",		u8"\u2550",		u8"\u2551",		u8"\u2554",		u8"\u2557",		u8"\u255A",		u8"\u255D",

		//	8 Door			9 Corridor		
		u8"\u256C",		u8"\u2592",

	};

	currMap =
	{

		"                                         ",
		"  {-------}                              ",
		"  |.......|                 {---------}  ",
		"  |.......+xxxxxxxxxxxxx    |.........|  ",
		"  |.......|            x    |....?....|  ",
		"  [---+---]            x    |.........|  ",
		"      x                x    [---+++---]  ",
		"      x    {-------}   x        xxx      ",
		"      x    |.......+xxxxxxxxxxxxxxx      ",
		"      xxxxx+.......|                     ",
		"           [-------]                     ",
		"                                         ",

	};
}
