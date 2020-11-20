#include "GameEngine.h"

int main(int argc, char* argv[])
{
#if defined (_WIN32)
    SetConsoleTitleW(L"SPACE SHOOTER");
#endif

    if (argc == 2 && strstr(argv[1], "-s"))
    	GameEngine::setSounds(true);

    while (true)
        GameEngine::showMenu();

    return 0;
}