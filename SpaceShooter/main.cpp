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

static constexpr auto MAXIMUM_WIDTH  		= 100;
static constexpr auto MINIMUM_WIDTH  		= 5;
static constexpr auto MAXIMUM_HEIGHT 		= 50;
static constexpr auto MINIMUM_HEIGHT 		= 10;

static constexpr auto DEFAULT_GLOBAL_WIDTH  = 15;
static constexpr auto DEFAULT_GLOBAL_HEIGHT = 10;
static constexpr auto DEFAULT_DELAY         = 1000;

#define ENEMY '@'

// TODO: настройка аргументами командной строки
// TODO: настройка _delay
// TODO: UNICODE TEXT
// TODO: 2 PLAYERS (Qt)
// TODO: triangle as player
// TODO: new game animation (wave)
//===============================================================================
struct Point
{
    uint32_t y;
    uint32_t x;
};
//===============================================================================
class GameEngine
{
    enum Mode
    {
        default_mode,
        wave_mode
    };


    static uint32_t _global_width;
    static uint32_t _global_height;
    static uint32_t _score;
    static uint32_t _delay;
    static bool is_first_call; // for resizing field model before game starts
    static bool _game_over;
    static Mode _mode;




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
                is_first_call = true;
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
                is_first_call = true;
                cout << "Value successfully updated." << endl;
                system("pause>0");
                system("cls");
                return;

            case '3':
                system("cls");
                return;
            }
        } while (ans != '1' && ans != '2' && ans != '3');
    }
    //===============================================================================
    static void resizeFieldModel()
    {
        field.resize(_global_height);
        for (uint32_t i = 0; i < _global_height; ++i)
            field[i].resize(_global_width);
    }
    //===============================================================================
    static void deleteKilledEnemy(const Point& killed)
    {
        field[killed.y][killed.x] = ' ';
        ++_score;
    }
    //===============================================================================
    static void updateFieldModel()
    {
        if (is_first_call)
        {
            resizeFieldModel();
            //is_first_call = false;
        }

        // создаём структуру данных в соответствии с_global_width и _global_height
        for (uint32_t y = 0; y < _global_height; ++y)
        {
            for (uint32_t x = 0; x < _global_width; ++x)
            {
                // не трогаем врагов
                if (field[y][x] == ENEMY)
                {
                    continue;
                }

                if (player.x() == x && player.y()-1 == y)
                {
                    field[y][x] = '^';
                    continue;
                }
                if (player.x() == x && player.y() == y)
                {
                    field[y][x] = 'v';
                    continue;
                }
                if (player.x()-1 == x && player.y() == y)
                {
                    field[y][x] = '<';
                    continue;
                }
                if (player.x()+1 == x && player.y() == y)
                {
                    field[y][x] = '>';
                    continue;
                }

                if (y == 0 || x == 0 || y == _global_height-1 || x == _global_width-1)
                {
                    field[y][x] = '#';
                }
                else
                {
                    field[y][x] = ' ';
                }
            }
        }

        if (is_first_call)
        {
            field[2][2] = ENEMY;
            is_first_call = false;
        }

    }
    //===============================================================================
    static void drawField(bool is_shooting = 0)
    {
        /// ДЛЯ ОТРИСОВКИ ЛАЗЕРА (И ПОПАДАНИЯ) рекурсивно вызываем метод с параметром

        static bool is_enemy_killed = false;
        is_enemy_killed = false;
        // стираем предыдущее поле перед рисованием нового
        if (is_shooting)
        {
            system("cls");

        }


        for (uint32_t y = 0; y < _global_height; ++y)
        {
            for (uint32_t x = 0; x < _global_width; ++x)
            {
                // добавить условие о попадании во врага и 'X' на месте попадания,
                // а также удаление врага из модели(вызов updateFieldModel() с параметром)

                if (is_shooting && !is_enemy_killed && player.x() == x && field[y][x] == ENEMY)// доделать этот if на удаление именно ближайшего врага
                {

                    for (uint32_t height = _global_height-2; height > 0; --height)
                    {
                        if (!is_enemy_killed && field[height][x] == ENEMY)
                        {
                            //cout << 'X';
                            deleteKilledEnemy(Point{height, x});
                            is_enemy_killed = true;
                            break;
                        }
                    }
                }



                if (is_shooting && field[y][player.x()] != ENEMY && player.x() == x && player.y() != 0 && player.y() != _global_height-1 && y != 0 &&
                    y != _global_height-1 && y != _global_height-2 && y != _global_height-3)
                {
                    cout << '|';
                    continue;
                }

                cout << field[y][x];
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
        //std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

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
            drawField(true);
            break;
        }

        //std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        //std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);


        system("cls");
    }
	//===============================================================================
    static void addInvaders()
    {
    	// метод передвигает всех врагов в сторону игрока и добавляет новый ряд
    	// супостатов (в зависимости от режима, по умолчанию рандомно)
    	// цикл проходится по модели снизу вверх и слева направо
    	while (!(_game_over))
    	{
    		for (uint32_t y = _global_height-2; y >= 1; --y)
        	{
            	for (uint32_t x = 0; x < _global_width; ++x)
            	{
            		if (field[_global_height-3][x] == ENEMY)
            		{
            			// враги дошли до игрока - выход из циклов и конец игры (анимация и всё такое)
                        _game_over = true;
                        break;
            		}

            		if (field[y][x] == ENEMY)
            		{
            			field[y][x] = ' ';
            			field[y+1][x] = ENEMY;
            		}

                    // добавляем челиков на последний ряд
                    if (_mode == Mode::default_mode)
                    {
                        if (y == 1 && x != 0 && x != _global_width-1 && rand() % 2)
                        {
                            field[y][x] = ENEMY;
                        }
                    }
                    else
                    {
                        if (y == 1 && x != 0 && x != _global_width-1)
                        {
                            field[y][x] = ENEMY;
                        }
                    }

            	}
            }
    		std::this_thread::sleep_for(std::chrono::milliseconds(_delay));
    	}
    }
    //===============================================================================
    static void initGameLoop()
    {
    	srand(time(nullptr));
        system("cls");
        std::thread enemies(addInvaders);


        while (true)
        {
            if (_game_over)
            {
                enemies.join(); // возможно нужно поставить до цикла
                system("cls");
                cout << "GAME OVER" << endl;
                cout << "SCORE = " << _score << endl;
                system("pause>0");
                break;
            }
            updateFieldModel();
            drawField();
        }


    }
    //===============================================================================
public:
    static void showMenu()
    {
        system("cls");
        int ans = 0;

        do{
            cout << "1.New game" << endl;
            cout << "2.Settings" << endl;
            cout << "3.Exit" << endl;

            ans = getch();
            switch (ans)
            {
            case '1':
                initGameLoop();
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
        } while (ans != '1' && ans != '2' && ans != '3');
    }
};
//===============================================================================
uint32_t GameEngine::_global_width     = DEFAULT_GLOBAL_WIDTH;
uint32_t GameEngine::_global_height    = DEFAULT_GLOBAL_HEIGHT;
uint32_t GameEngine::_score            = 0;
uint32_t GameEngine::_delay            = DEFAULT_DELAY;
GameEngine::Player GameEngine::player;
vector<vector<char>> GameEngine::field;
bool GameEngine::is_first_call 		   = true;
bool GameEngine::_game_over            = false;
GameEngine::Mode GameEngine::_mode = Mode::default_mode;
//===============================================================================
int main()
{
    //GameEngine gEngine;

    GameEngine::showMenu();




    return 0;
}
