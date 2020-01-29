#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>    // std::find
#include <conio.h>
#include <Windows.h>

using namespace std;

// Function declarations
void InitializeGame(vector<string>& currMap);
void MainMenu();
void DisplayTitle(string title);
void changeKeyBindings();
void UpdateScreen(vector<string>& currMap, pair<int, int>& playerPos);
bool canMove(char& currChar);
void cls();


// Global variables
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
vector<int> keyBinds;
vector<const char*> CharList;
const vector<int> DefaultControls = { int('w'), int('s'), int('a'), int('d'), int('f'), int(' ') };
enum keyCodes { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, INTERACT, OPEN_INVENTORY, NUM_KEYCODES	};
enum ColorCodes{	BLUE = 9, GREEN, AQUA, RED, MAGENTA, YELLOW, WHITE	};
enum UnicodeChar
{
	Player, Floor, Horizontal, Vertical, TopLeftCorner, TopRightCorner, BottomLeftCorner, BottomRightCorner,
	Door, Corridor, SIZE_OF_UNICODECHAR
};

bool canMoveUp, canMoveDown, canMoveLeft, canMoveRight;

int main() {

	// Set the output to UTF-8, so we can draw symbols like boxes
	SetConsoleOutputCP(CP_UTF8);
		
	vector<string> currMap;

	bool stillPlaying = true, stillGaming = false; bool keepLooping = true;
	int userInput; char currChar;
	
	// X, Y
	pair<int, int> playerPos = {6, 3};

	// Initialize the game's maps and data
	InitializeGame(currMap);
	
	/*const char* s = u8"\u2592";
	cout << s << endl << endl;
	SetConsoleTextAttribute(hConsole, WHITE);
	for (int i = 0; i < SIZE_OF_UNICODECHAR; i++) {
		cout << CharList[i] << endl;

	}*/

	// 
	while (stillPlaying) {
		
		MainMenu();
		keepLooping = true;

		while (keepLooping) {	
			
			userInput = _getch();

			switch (userInput) {

				// Play game
			case int('4'):

				stillGaming = true;
				keepLooping = false;
				break;

				// Key Binding
			case int('5') :

				changeKeyBindings();
				keepLooping = true;
				cls();
				MainMenu();
				break;

				// Quit
			case int('6') :

				stillGaming = false;
				keepLooping = false;
				stillPlaying = false;
				break;

			default:
				// DEBUG test for ASCII lol
				// cout << userInput << endl;
				break;
			}
			
			
		}

		// Game Loop
		while (stillGaming) {

			UpdateScreen(currMap, playerPos);

			// Update booleans
			currChar = currMap[playerPos.second - 1][playerPos.first];
			canMoveUp = canMove(currChar);

			currChar = currMap[playerPos.second + 1][playerPos.first];
			canMoveDown = canMove(currChar);

			currChar = currMap[playerPos.second][playerPos.first - 1];
			canMoveLeft = canMove(currChar);

			currChar = currMap[playerPos.second][playerPos.first + 1];
			canMoveRight = canMove(currChar);

			// Wait for user input key before the world updates
			userInput = _getch();

			// Force userInput to be lowercase
			tolower(char(userInput));


			// Detect user input
			if (userInput == keyBinds[MOVE_UP]) {
				if (canMoveUp) {
					playerPos.second--;
				}
			}
			if (userInput == keyBinds[MOVE_DOWN]) {
				if (canMoveDown) {
					playerPos.second++;
				}
			}
			if (userInput == keyBinds[MOVE_LEFT]) {
				if (canMoveLeft) {
					playerPos.first--;
				}
			}
			if (userInput == keyBinds[MOVE_RIGHT]) {
				if (canMoveRight) {
					playerPos.first++;
				}
			}
			// TEMP - Esc (returns to main menu, should bring up Pause)
			if (userInput == 27) {
				stillGaming = false;
			}// End detecting user input
						
		}	// End game loop, right before screen update

	}	// End stillPlaying game loop

	std::cout << "Quitting the game. Press any key to leave." << endl;
	userInput = _getch();

	return 0;
}


void UpdateScreen(vector<string>& currMap, pair<int, int>& playerPos) {

	char currChar; string currPrint, currAction = "";
	
	cls();

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


bool canMove(char& currChar) {
	return (currChar == '.' || currChar == 'x' || currChar == '+');
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

void InitializeGame(vector<string>& currMap)
{
	// Store all UTF-8 codes for symbols to print on screen
	CharList = {

	//	0 Player 		1 Floor			2 Horizontal	3 Vertical		4 TopLeft		5 TopRight		6 BottomLeft	7 BottomRight
		u8"\u263A",		u8"\u00B7",		u8"\u2550",		u8"\u2551",		u8"\u2554",		u8"\u2557",		u8"\u255A",		u8"\u255D",

	//	8 Door			9 Corridor		
		u8"\u256C",		u8"\u2592",

	};

	// Temporary test map
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

	// 
	ifstream inFile; bool loadDefaults = false;
	vector<int> loadedCodes;
	int aKeyCode, count = 0;
	keyBinds.resize(NUM_KEYCODES);

	inFile.open("keyBindings.txt");
	// If there is a saved file of key bindings
	if (inFile) {
		
		while (inFile >> aKeyCode) {
			aKeyCode = loadedCodes[count];
			count++;
		}

		inFile.close();

		// If we had loaded in the correct amount, send it in to our keyBinds vector
		if (count == NUM_KEYCODES-1) {
			keyBinds = loadedCodes;
			loadDefaults = false;
		}
		// If there isn't the correct amount
		else {
			
			loadDefaults = true;
			
		}
	}
	else { loadDefaults = true;  }
	
	// If there isn't a VALID saved file of key bindings, then we need to use the defaults
	if (loadDefaults) {
		// DEBUG
		//cout << "Saving defaults.\n\n";

		// enum keyCodes { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, INTERACT, OPEN_INVENTORY, NUM_KEYCODES};
		for (int j = 0; j < NUM_KEYCODES; j++) {

			keyBinds[j] = DefaultControls[j];
		}
	}
	// Done	
}

void changeKeyBindings()
{
	int userInput; bool currentlySwitching = false;
	vector<bool> keyOptions(NUM_KEYCODES, false);
	vector<string> controlNames = { "Move Up" , "Move Down", "Move Left", "Move Right", "Interact", "Open Inventory" };
	keyOptions.resize(NUM_KEYCODES);
	int selectedOption = 0;	
	
	//vector<int>::iterator keyBinds_It;

	keyOptions[selectedOption] = true;

	while (1) {
		
		cls();

		DisplayTitle("Change Key Bindings");

		currentlySwitching ? cout << " Enter a key to change:  " << controlNames[selectedOption] : cout << " W/S - Navigate,\t Spacebar - Select";
		cout << endl << endl;

		cout << "   Key | Action" << endl;
		cout << " ------------------------" << endl;

		for (int i = 0; i < NUM_KEYCODES; i++) {

			keyOptions[i] ? cout << " > " : cout << "   ";
			cout << "[";
			currentlySwitching && (i == selectedOption) ? cout << "?" : cout << char(toupper(keyBinds[i]));
			cout << "] | " << controlNames[i] << endl;
		}

		cout << endl << " Backspace - Restore defaults" << endl << endl;

		userInput = _getch();

		

		// If currently switching a bound key,
		if (currentlySwitching) {

			// If the player inputted Esc or Backspace, ignore the key and don't change it
			if (userInput != char(27) && userInput != char(8)) {
								
				// Then simply replace the currently selected option's key with the key just selected, and toggle it back to false
				keyBinds[selectedOption] = tolower(userInput);
				
			}
			// At this point, we are no longer in the "switching" mode
			currentlySwitching = false;
		}
		else {

			switch (char(userInput)) {

				// Move up option (W)
			case 'w':
			case 'W':

				// Deselect the current select option as being true, shift it (wrap around if necessary), and then set the upper one to true (currently selected)
				keyOptions[selectedOption] = false;
				selectedOption == 0 ? selectedOption = NUM_KEYCODES - 1 : selectedOption--;
				keyOptions[selectedOption] = true;
				break;

				// Move down option (S)
			case 's':
			case 'S':

				keyOptions[selectedOption] = false;
				selectedOption == NUM_KEYCODES - 1 ? selectedOption = 0 : selectedOption++;
				keyOptions[selectedOption] = true;
				break;

				// Select option (spacebar)
			case ' ':

				currentlySwitching = true;
				break;

				// Esc (return)
			case char(27) :
				return;
				break;

				// Backspace (restore default controls)
			case char(8) :
				// enum keyCodes { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, INTERACT, OPEN_INVENTORY, NUM_KEYCODES};
				for (int i = 0; i < NUM_KEYCODES; i++) {

					for (int j = 0; j < NUM_KEYCODES; j++) {

						keyBinds[j] = DefaultControls[j];
					}

				}
				break;

			default:
				break;

			}	// end while
		}

	}
	
}

void MainMenu()
{
	const short int BAR_WIDTH = 32;

	int userInput;

	cls();
	SetConsoleTextAttribute(hConsole, WHITE);
	DisplayTitle("Roguelike Demo");

	cout << "[4] Play Game" << endl;
	cout << "[5] Key Bindings" << endl;
	cout << "[6] Quit" << endl;
	cout << endl;

}

void DisplayTitle(string title)
{
	const int WfromS = 11;
	// To center it, make within the box constraints, the title always has 11 empty spaces on either side
	// Therefore the BAR_WIDTH calculation (length of the outer border) will be:
	// title.length() + 11*2 + 2
	short int BAR_WIDTH = title.length() + (WfromS * 2);
	
	cout << CharList[TopLeftCorner];
	for (int i = 0; i < BAR_WIDTH; i++) {
		cout << CharList[Horizontal];
	}
	cout << CharList[TopRightCorner] << endl;

	cout << CharList[Vertical];
	for (int j = 0; j < WfromS; j++) {
		if (j > 0 && j < 4) {
			cout << "~";
		}
		else {
			cout << " ";
		}
	}
	cout << title;
	for (int j = 0; j < WfromS; j++) {
		if (j > WfromS -  5 && j < WfromS - 1) {
			cout << "~";
		}
		else {
			cout << " ";
		}
	}

	//cout << " ~~~      ROGUELIKE       ~~~ ";
	cout << CharList[Vertical] << endl;

	cout << CharList[BottomLeftCorner];
	for (int i = 0; i < BAR_WIDTH; i++) {
		cout << CharList[Horizontal];
	}
	cout << CharList[BottomRightCorner] << endl;

	cout << endl;
}