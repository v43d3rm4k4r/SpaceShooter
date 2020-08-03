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
struct Point
{
	size_t x;
	size_t y;
};
//===============================================================================
class GameEngine
{
	static size_t _global_width;
	static size_t _global_height;
	static size_t _score;

	class Player
	{
		Point coords;
	public:
		Player()
			: coords{_global_width / 2, _global_height - 2}
			{}

		size_t x() const
		{
			return coords.x;
		}

		size_t y() const
		{
			return coords.y;
		}

		void shoot()
		{

		}
	};


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
					while (scanf(" %u", &_global_width) != 1 || _global_width > MAXIMUM_WIDTH || _global_width < MINIMUM_WIDTH)
					{
						cout << "Invalid input." << endl;
						_global_width = DEFAULT_GLOBAL_WIDTH;
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
					while (scanf(" %u", &_global_height) != 1 || _global_height > MAXIMUM_HEIGHT || _global_height < MINIMUM_HEIGHT)
					{
						cout << "Invalid input." << endl;
						_global_height = DEFAULT_GLOBAL_HEIGHT;
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
		Player player;

		for (size_t y = 0; y < _global_height; ++y)
		{
			for (size_t x = 0; x < _global_width; ++x)
			{
				if (player.x() == x && player.y()-1 == y)
				{
					cout << "^";
					continue;
				}
				if (player.x() == x && player.y() == y)
				{
					cout << "v";
					continue;
				}
				if (player.x()-1 == x && player.y() == y)
				{
					cout << "<";
					continue;
				}
				if (player.x()+1 == x && player.y() == y)
				{
					cout << ">";
					continue;
				}

				if (y == 0 || x == 0 || y == _global_height-1 || x == _global_width-1)
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
		cout << endl << endl;

		for (int i = 0; i < _global_width - 5; ++i) cout << ' ';
		cout << "SCORE = " << _score << endl;
		system("cls");

	}
	//===============================================================================
	static int initMainLoop()
	{
	    system("cls");

	    while(true)
	    {
	    	printField();
	    }



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
size_t GameEngine::_global_width = DEFAULT_GLOBAL_WIDTH;
size_t GameEngine::_global_height = DEFAULT_GLOBAL_HEIGHT;
size_t GameEngine::_score = 0;
//===============================================================================
int main()
{
	//GameEngine gEngine;

    GameEngine::showMenu();




	return 0;
}
