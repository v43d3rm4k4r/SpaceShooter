#include <iostream>
#include <string>
#include <conio.h>

using std::cout;
using std::endl;
using std::wstring;

static constexpr auto MAXIMUM_WIDTH = 100;
static constexpr auto MINIMUM_WIDTH = 5;
static constexpr auto MAXIMUM_HEIGHT = 50;
static constexpr auto MINIMUM_HEIGHT = 10;


static constexpr auto DEFAULT_GLOBAL_WIDTH = 15;
static constexpr auto DEFAULT_GLOBAL_HEIGHT = 10;

// TODO: UNICODE TEXT
// TODO: 2 PLAYERS (Qt)
//===============================================================================
class GameEngine
{
	static size_t _width;
	static size_t _height;

	static void showSettings()
	{
		cout << "1.Set field width" << endl;
		cout << "2.Set field height" << endl;
		cout << "3.Exit" << endl;

		int ans = 0;

		do{
			ans = getch();
			switch (ans)
			{
				case '1':
					system("cls");
					cout << "Enter field width (from " << MINIMUM_WIDTH << " to " << MAXIMUM_WIDTH << ")" << endl;
					cout << ">";
					while (scanf(" %u", &_width) != 1 || _width > MAXIMUM_WIDTH || _width < MINIMUM_WIDTH)
					{
						cout << "Invalid input." << endl;
						_width = DEFAULT_GLOBAL_WIDTH;
						system("pause>0");
						system("cls");
						cout << "Enter field width (from " << MINIMUM_WIDTH << " to " << MAXIMUM_WIDTH << ")" << endl;
						cout << ">";
					}
					system("cls");
					cout << "Value successfully updated." << endl;
					system("pause>0");
					system("cls");
					return;

				case '2':
					system("cls");
					cout << "Enter field height (from " << MINIMUM_HEIGHT << " to " << MAXIMUM_HEIGHT << ")" << endl;
					cout << ">";
					while (scanf(" %u", &_height) != 1 || _height > MAXIMUM_HEIGHT || _height < MINIMUM_HEIGHT)
					{
						cout << "Invalid input." << endl;
						_height = DEFAULT_GLOBAL_HEIGHT;
						system("pause>0");
						system("cls");
						cout << "Enter field height (from " << MINIMUM_HEIGHT << " to " << MAXIMUM_HEIGHT << ")" << endl;
						cout << ">";
					}
					system("cls");
					cout << "Value successfully updated." << endl;
					system("pause>0");
					system("cls");
					return;

				case '3':
					system("cls");
					return;
			}
		} while (ans != '1' || ans != '2' || ans != '3');
	}

	//===============================================================================
	static void printField()
	{
		for (size_t y = 0; y < _height; ++y)
		{
			for (size_t x = 0; x < _width; ++x)
			{
				if (y == 0 || x == 0 || y == _height-1 || x == _width-1)
				{
					cout << '#';
				}
				else
				{
					cout << ' ';
				}
			}
			cout << endl;
		}
	}
	//===============================================================================
	static int initMainLoop()
	{
	    system("cls");

		printField();

		return 0;
	}
	//===============================================================================
public:
	static void showMenu()
	{
		cout << "1.New game" << endl;
		cout << "2.Settings" << endl;
		cout << "3.Exit" << endl;
		int ans = 0;

		do{
			ans = getch();
			switch (ans)
			{
				case '1':
					initMainLoop();
					break;

				case '2':
					system("cls");
					showSettings();
					system("cls");
					showMenu();
					break;

				case '3':
					system("cls");
					cout << "Bye!" << endl;
					system("pause>0");
					exit(EXIT_SUCCESS);
					break;
			}
		} while (ans != '1' || ans != '2' || ans != '3');
	}
};
//===============================================================================
size_t GameEngine::_width = DEFAULT_GLOBAL_WIDTH;
size_t GameEngine::_height = DEFAULT_GLOBAL_HEIGHT;
//===============================================================================
int main()
{
	//GameEngine gEngine;

    GameEngine::showMenu();




	return 0;
}
