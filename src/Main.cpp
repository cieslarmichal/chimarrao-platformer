#include <Windows.h>

#include "Game.h"

int main()
{
    game::Game game;
    game.run();
    return 0;
}

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    return main();
}
#endif