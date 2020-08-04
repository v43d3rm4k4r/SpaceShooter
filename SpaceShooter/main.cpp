#include <iostream>
#include <vector>
#include <conio.h>
#include <cctype>
#include <thread>
#include <chrono>

using std::cout;
using std::endl;
using std::wstring;
using std::vector;

static constexpr auto MAXIMUM_WIDTH = 100;
static constexpr auto MINIMUM_WIDTH = 5;
static constexpr auto MAXIMUM_HEIGHT = 50;
static constexpr auto MINIMUM_HEIGHT = 10;

static constexpr auto DEFAULT_GLOBAL_WIDTH = 15;
static constexpr auto DEFAULT_GLOBAL_HEIGHT = 10;

// TODO: UNICODE TEXT
// TODO: 2 PLAYERS (Qt)
// TODO: triangle as player
//===============================================================================
struct Point
{
    uint32_t x;
    uint32_t y;
};
//===============================================================================
class GameEngine
{
    static uint32_t _global_width;
    static uint32_t _global_height;
    static uint32_t _score;
    static uint32_t _delay;



    class Player
    {
        Point coords;
    public:
        Player()
            : coords{_global_width / 2, _global_height - 2}
        {}

        uint32_t x() const
        {
            return coords.x;
        }

        uint32_t y() const
        {
            return coords.y;
        }

        void moveLeft()
        {
            coords.x -= 1;
        }

        void moveRight()
        {
            coords.x += 1;
        }

        void setX(uint32_t x)
        {
            coords.x = x;
        }

        void setY(uint32_t y)
        {
            coords.y = y;
        }
    };

    static Player player;
    static vector<vector<char>> field;

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
                //player.setX(_global_width);
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
                player.setY(_global_height-2);
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
    /*static void updateEnemies()
    {
        //field.reserve()
        // создаём структуру данных в соответствии с_global_width и _global_height
        for (uint32_t y = 0; y < _global_height; ++y)
        {
            for (uint32_t x = 0; x < _global_width; ++x)
            {
                field[x][y];
            }
        }

    }*/
    //===============================================================================
    static void printField(bool is_shooting = 0)
    {
        // стираем предыдущее поле перед рисованием нового
        if (is_shooting)
            system("cls");

        for (uint32_t y = 0; y < _global_height; ++y)
        {
            for (uint32_t x = 0; x < _global_width; ++x)
            {


                // лазер не должен рисоваться на месте врагов, поэтому сначала враги

                if (is_shooting && player.x() == x && player.y() != 0 && player.y() != _global_height-1 && y != 0 &&
                                    y != _global_height-1 && y != _global_height-2 && y != _global_height-3)
                {
                    cout << "|";
                    continue;
                }

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

        for (uint32_t i = 0; i < _global_width - 5; ++i) cout << ' ';
        cout << "SCORE = " << _score << endl;




        // выходим здесь если рекурсивно вошли "выстрелить"
        if (is_shooting)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return;
        }



        // здесь дать время(delay) на действие

        int action = toupper(getch());
        //if (action == 'Ф') action = 'A';
        //if (action == 'В') action = 'D';
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        switch (action)
        {
        case 'A':
            if (player.x() != 1)
                player.moveLeft();
            break;

        case 'D':
            if (player.x() != _global_width-2)
                player.moveRight();
            break;

        case ' ':
            //player.shoot();
            printField(true);
            break;
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);


        system("cls");
    }
    //===============================================================================
    static int initMainLoop()
    {
        system("cls");

        while (true)
        {
            printField();
            //updateEnemies();
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
uint32_t GameEngine::_global_width  = DEFAULT_GLOBAL_WIDTH;
uint32_t GameEngine::_global_height = DEFAULT_GLOBAL_HEIGHT;
uint32_t GameEngine::_score         = 0;
uint32_t GameEngine::_delay         = 500;
GameEngine::Player GameEngine::player;
//===============================================================================
int main()
{
    //GameEngine gEngine;

    GameEngine::showMenu();




    return 0;
}
