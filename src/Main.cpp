#include "GameFactory.h"
//#include "Windows.h"

int main()
{
    auto game = game::GameFactory::createGame();
    game->run();
    return 0;
}

//#ifdef _WIN32
// int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//{
//    return main();
//}
//#endif