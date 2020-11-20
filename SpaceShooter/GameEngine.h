#pragma once

#include <iostream>
#include <vector>
#include <cctype>
#include <thread>
#include <chrono>

#if defined (_WIN32)
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#elif defined (__linux__)
#include <termios.h>
#endif

using std::cout;
using std::endl;
using std::wstring;
using std::vector;

static constexpr auto MAXIMUM_WIDTH         = 100;
static constexpr auto MINIMUM_WIDTH         = 5;
static constexpr auto MAXIMUM_HEIGHT        = 50;
static constexpr auto MINIMUM_HEIGHT        = 10;

static constexpr auto DEFAULT_GLOBAL_WIDTH  = 20;
static constexpr auto DEFAULT_GLOBAL_HEIGHT = 20;
static constexpr auto DEFAULT_DELAY         = 3000;

#define ENEMY '@'
#define ESC 27

// TODO: настройка аргументами командной строки
// TODO: UNICODE TEXT
// TODO: 2 PLAYERS (Qt)
// TODO: triangle as player
//===============================================================================
void clearConsole()
{
#if defined (_WIN32)
    system("cls");
#elif defined (__linux__)
    system("clear");
#endif
}
//===============================================================================
#if defined (__linux__)
static struct termios old, current;
/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    current = old; /* make new settings same as old settings */
    current.c_lflag &= ~ICANON; /* disable buffered i/o */
    if (echo)
    {
        current.c_lflag |= ECHO; /* set echo mode */
    }
    else
    {
        current.c_lflag &= ~ECHO; /* set no echo mode */
    }
    tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}
/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}
/* Read 1 character without echo */
char getch()
{
    return getch_(0);
}
#endif // only for linux
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
    static bool _play_sounds;

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

        Player& setX(uint32_t x)
        {
            coords.x = x;
            return *this;
        }

        Player& setY(uint32_t y)
        {
            coords.y = y;
            return *this;
        }
    };

    static Player player;
    static vector<vector<char>> field;

    static void showSettings()
    {
        cout << "1.Set field width" << endl;
        cout << "2.Set field height" << endl;
        cout << "3.Set enemies appearing mode" << endl;
        cout << "4.Set enemies appearing delay" << endl;
        cout << "5.Exit" << endl;

        int ans = 0;

        do{
            ans = getch();
            switch (ans)
            {
            case '1':
                clearConsole();
                cout << "Enter field width (from " << MINIMUM_WIDTH << " to " << MAXIMUM_WIDTH << ")" << endl;
                cout << ">";
                while (scanf(" %u", &_global_width) != 1 || _global_width > MAXIMUM_WIDTH || _global_width < MINIMUM_WIDTH)
                {
                    clearConsole();
                    cout << "Invalid input." << endl;
                    _global_width = DEFAULT_GLOBAL_WIDTH;
                    getch();
                    clearConsole();
                    cout << "Enter field width (from " << MINIMUM_WIDTH << " to " << MAXIMUM_WIDTH << ")" << endl;
                    cout << ">";
                }
                clearConsole();
                //player.setX(_global_width);
                is_first_call = true;
                cout << "Value successfully updated." << endl;
                getch();
                clearConsole();
                return;

            case '2':
                clearConsole();
                cout << "Enter field height (from " << MINIMUM_HEIGHT << " to " << MAXIMUM_HEIGHT << ")" << endl;
                cout << ">";
                while (scanf(" %u", &_global_height) != 1 || _global_height > MAXIMUM_HEIGHT || _global_height < MINIMUM_HEIGHT)
                {
                    clearConsole();
                    cout << "Invalid input." << endl;
                    _global_height = DEFAULT_GLOBAL_HEIGHT;
                    getch();
                    clearConsole();
                    cout << "Enter field height (from " << MINIMUM_HEIGHT << " to " << MAXIMUM_HEIGHT << ")" << endl;
                    cout << ">";
                }
                clearConsole();
                player.setY(_global_height-2);
                is_first_call = true;
                cout << "Value successfully updated." << endl;
                getch();
                clearConsole();
                return;

            case '3':
            {
                clearConsole();
                cout << "1.Random mode" << endl;
                cout << "2.Wave mode" << endl;
                int mode = getch();

                while (mode != '1' && mode != '2')
                {
                    cout << mode << endl;
                    clearConsole();
                    cout << "Invalid input." << endl;
                    getch();
                    clearConsole();
                    cout << "1.Random mode" << endl;
                    cout << "2.Wave mode" << endl;
                    mode = getch();
                }
                clearConsole();

                if (mode == '1')
                    _mode = Mode::default_mode;
                if (mode == '2')
                    _mode = Mode::wave_mode;

                cout << "Mode successfully updated." << endl;
                getch();
                clearConsole();
                return;
            }

            case '4':
            {
                clearConsole();
                cout << "Enter denemies appearing delay (in milliseconds)" << endl;
                cout << ">";
                while (scanf(" %u", &_delay) != 1)
                {
                    clearConsole();
                    cout << "Invalid input." << endl;
                    _delay = DEFAULT_DELAY;
                    getch();
                    clearConsole();
                    cout << "Enter delay(in milliseconds)" << endl;
                    cout << ">";
                }
                clearConsole();
                cout << "Value successfully updated." << endl;
                getch();
                clearConsole();
                return;
            }

            case '5':
                clearConsole();
                return;
            }
        } while (ans != '1' && ans != '2' && ans != '3' && ans != '4' && ans != '5');
    }
    //===============================================================================
    static void resizeFieldModel()
    {
        field.resize(_global_height);
        for (uint32_t i = 0; i < _global_height; ++i)
            field[i].resize(_global_width);
        player.setX(_global_width / 2).setY(_global_height - 2);
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
            is_first_call = false;
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
    }
    //===============================================================================
    static void drawField(bool is_shooting = 0)
    {
        /// ДЛЯ ОТРИСОВКИ ЛАЗЕРА (И ПОПАДАНИЯ) рекурсивно вызываем метод с параметром

        static bool is_enemy_killed = false;
        is_enemy_killed = false; // чтоб не убивать всех в ряду
        Point killed_enemy_coords = {0, 0};

        // стираем предыдущее поле перед рисованием нового
        if (is_shooting)
        {
            clearConsole();

        }


        for (uint32_t y = 0; y < _global_height; ++y)
        {
            for (uint32_t x = 0; x < _global_width; ++x)
            {
                if (is_shooting && !is_enemy_killed && player.x() == x && field[y][x] == ENEMY)
                {

                    for (uint32_t height = _global_height-2; height > 0; --height)
                    {
                        if (!is_enemy_killed && field[height][x] == ENEMY)
                        {
                            //cout << 'X';
                            killed_enemy_coords = {height, x};
                            deleteKilledEnemy(killed_enemy_coords);
                            is_enemy_killed = true;
                            break;
                        }
                    }
                }


                /*if (is_shooting && player.x() == x && player.y() != 0 && player.y() != _global_height-1 && y != 0 &&
                    y != _global_height-1 && y != _global_height-2 && y != _global_height-3)*/

                if (is_shooting && player.x() == x)
                {
                    if (y < player.y() && y > killed_enemy_coords.y && y != _global_height-1 && y != _global_height-2 && y != _global_height-3
                        && y != 0 && y != 1)
                    {
                        cout << '|';
                        continue;
                    }

                }

                cout << field[y][x];
            }
            cout << endl;
        }
        cout << endl << endl;

        for (uint32_t i = 0; i < _global_width - 5; ++i)
            cout << ' ';

        cout << "SCORE = " << _score << endl;


        // выходим здесь если рекурсивно вошли "выстрелить"
        if (is_shooting)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return;
        }


        // здесь дать время(delay) на действие
        //SetTimer(0, nullptr, 1000, (TIMERPROC)addInvaders)


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
            if (_play_sounds)
            {
                PlaySound(TEXT("..\\sounds\\shoot_sound1.wav"), nullptr, SND_FILENAME);
            }
            drawField(true);
            break;

        case ESC:
            _game_over = true;
            if (_play_sounds)
            {
                PlaySound(TEXT("..\\sounds\\fail_sound1.wav"), nullptr, SND_FILENAME);
            }
            break;
        }

        //std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        //std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);


        clearConsole();
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
        clearConsole();
        std::thread enemies(addInvaders);

        while (true)
        {
            if (_game_over)
            {
                enemies.join(); // возможно нужно поставить до цикла
                clearConsole();
                cout << "GAME OVER" << endl;
                cout << "SCORE = " << _score << endl;
                getch();
                break;
            }
            updateFieldModel();
            drawField();
        }


    }
    static void clearField()
    {
        for (uint32_t y = _global_height-2; y >= 1; --y)
        {
            for (uint32_t x = 0; x < _global_width; ++x)
            {
                field[y][x] = ' ';
            }
        }
    }

    //===============================================================================
public:
    static void showMenu()
    {
        clearConsole();
        int ans = 0;

        do{
            clearConsole();
            cout << "1.New game" << endl;
            cout << "2.Settings" << endl;
            cout << "3.Exit" << endl;

            ans = getch();
            switch (ans)
            {
            case '1':
                initGameLoop();
                clearField();
                _game_over = false;
                is_first_call = true;
                break;

            case '2':
                clearConsole();
                showSettings();
                clearConsole();
                //showMenu();
                break;

            case '3':
                clearConsole();
                cout << "Bye!" << endl;
                getch();
                exit(EXIT_SUCCESS);
                break;
            }
        } while (ans != '1' && ans != '2' && ans != '3');
    }

    static void setSounds(bool state)
    {
        _play_sounds = state;
    }
};
//===============================================================================
uint32_t GameEngine::_global_width     = DEFAULT_GLOBAL_WIDTH;
uint32_t GameEngine::_global_height    = DEFAULT_GLOBAL_HEIGHT;
uint32_t GameEngine::_score            = 0;
uint32_t GameEngine::_delay            = DEFAULT_DELAY;
GameEngine::Player GameEngine::player;
vector<vector<char>> GameEngine::field;
bool GameEngine::is_first_call         = true;
bool GameEngine::_game_over            = false;
GameEngine::Mode GameEngine::_mode = Mode::default_mode;
bool GameEngine::_play_sounds          = false;
