#include "Game.h"

#include <Windows.h>

int main()
{
    game::Game game;
    game.run();
    return 0;
}

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
return main();
}
#endif